#define _GNU_SOURCE

#include <err.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <glib-2.0/glib.h>
#include <err.h>

#include "shared_queue.h"

#define BUFFER_SIZE 512

const size_t THREAD_COUNT = 4;

static char* extract(char* str, const char* begin, const char* end)
{
    char* begin_pos = strstr(str, begin);
    if (begin_pos == NULL)
    {
        return NULL;
    }
    
    char* end_pos = strstr(begin_pos, end);
    if (!end_pos)
    {
        return NULL;
    }

    size_t len = end_pos - begin_pos - strlen(begin);

    char* result = malloc(len + 1);
    if (result)
    {
        strncpy(result, begin_pos + strlen(begin), len);
        result[len] = '\0';
    }

    return result;
}

static void send_404(int client_socket) {
    char* response = "HTTP/1.1 404 Not Found\r\n"
                     "Content-Type: text/html\r\n"
                     "Content-Length: 112\r\n"
                     "\r\n"
                     "<html><body><h1>404 Not Found</h1><p>The requested resource was not found on this server.</p></body></html>";

    send(client_socket, response, strlen(response), 0);
}

static int init_server(struct addrinfo* hint, struct addrinfo** res, char *port)
{
    int sfd;
    int err;

    struct addrinfo* p;

    memset(hint, 0, sizeof(*hint));

    hint->ai_family = AF_INET;
    hint->ai_flags = AI_PASSIVE;
    hint->ai_socktype = SOCK_STREAM;

    err = getaddrinfo(NULL, port, hint, res);

    if(err != 0)
    {
        errx(1, "[ERROR]: %s\n", gai_strerror(err));
    }
    for(p = *res; p != NULL; p = p->ai_next)
    {
        sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if(sfd == -1)
        {
            continue;
        }
        int optval = 1;
        err = setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
        if(err == -1)
        {
            errx(1, "[ERROR]: setsockopt");
        }

        err = bind(sfd, p->ai_addr, p->ai_addrlen);
        if(err == 0)
        {
            break;
        }
        else
        {
            close(sfd);
        }
    }

    freeaddrinfo(*res);
    
    if(p == NULL)
    {
        errx(EXIT_FAILURE, "[ERROR]: Couldn't connect");
    }
    err = listen(sfd, 5);
    if(err == -1)
    {
        close(sfd);
        errx(EXIT_FAILURE, "[ERROR]: Couln't listen");
    }
    return sfd;
}

static GString* get_message(int cfd, ssize_t* val)
{
    char buffer[512];
    GString* gstr = g_string_new(NULL);
    ssize_t k = *val;
    do
    {
        k = read(cfd, buffer, BUFFER_SIZE);
        if(k == -1)
        {
            errx(EXIT_FAILURE, "[ERROR] read");
        }
        g_string_append_len(gstr, buffer, k);

        if(g_str_has_suffix(gstr->str, "\r\n\r\n"))
        {
            break;
        }
    }
    while(k == BUFFER_SIZE);

    *val = k;
    return gstr;
}

void* worker(void* arg)
{
    shared_queue *queue = arg;
	while (TRUE)
	{
		int cfd;
		cfd = shared_queue_pop(queue);

        if (cfd != -1)
        {
            ssize_t k = 0;
            GString* gstr = get_message(cfd, &k);
            if(k == 0)
            {
                close(cfd);
                continue;
            }

            char* res = g_string_free(gstr, 0);
            char* resource = extract(res, "GET /", " HTTP/1.1");
            gchar *contents = NULL;
            gsize length = 0;
            GError *error = NULL;
            GString *str = g_string_new("www/");

            if (strcmp(resource, "") == 0)
            {
                resource = realloc(resource, 50);
                strcpy(resource, "index.html");
            }
            g_string_append(str, resource);
            
            if (g_file_get_contents(str->str, &contents, &length, &error))
            {
                if (strcmp("slow.html", resource) == 0)
                {
                    sleep(3);
                }
                
                send(cfd, "HTTP/1.1 200 OK\r\n\r\n", 19, MSG_MORE);
                write(cfd, contents, length);
                g_print("[ Socket: %i\nGetting: %s ]\n", cfd, resource);
                g_free(contents);
                free(resource);
            }
            else
            {
                send_404(cfd);
                g_error_free(error);
            }
            close(cfd);
        }
	}

	return NULL;
}

void rewrite(int fd, const void* buf, size_t count)
{
    ssize_t total = 0;
    while (total < (ssize_t) count)
    {
        ssize_t written = write(fd, buf + total, count-total);
        if (written == -1)
            errx(1,"rewrite failed\n");
        total += written;
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        errx(EXIT_FAILURE, "Usage:\n"
                "Arg 1 = Port number (e.g. 2048)");
    }

    struct addrinfo hint;
    struct addrinfo* res;

    int sfd;

    shared_queue* sharedQueue = shared_queue_new();
    pthread_t pthr;
    
    int err;
    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        err = pthread_create(&pthr, NULL, worker, (void *)sharedQueue);
        if (err != 0)
        {
            errx(EXIT_FAILURE, "[ERROR] When creating threads");
        }
    }

    sfd = init_server(&hint, &res, argv[1]);

    printf("Static server\n");
    printf("Listening to port %s...\n", argv[1]);

    while(TRUE)
    {
        int cfd = accept(sfd, NULL, NULL);
        if(cfd == -1)
        {
            errx(EXIT_FAILURE, "[ERROR] accept\n");
        }
        shared_queue_push(sharedQueue, cfd);
    }
    close(sfd);
    return 0;
}