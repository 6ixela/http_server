CC = gcc
CFLAGS = `pkg-config --cflags glib-2.0` -Wall -Wextra -pthread -Werror -Wvla -std=c99 -pedantic -fsanitize=address -g
CPPFLAGS = -Isrc 
# LDLIBS = `pkg-config --libs glib-2.0`

LDFLAGS = -fsanitize=address `pkg-config --libs glib-2.0`

