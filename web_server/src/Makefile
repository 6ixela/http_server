include flags.mk

OBJ = main.o

DIR=config daemon server
LDFLAGS=-fsanitize=address
LIBS = config/libconfig.a server/libserver.a daemon/libdaemon.a 

all: http_server

http_server: $(OBJ) $(LIBS)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) -Wl,--start-group $(LIBS) -Wl,--end-group -lglib-2.0

include libs.mk

.PHONY: all

clean:
	$(RM) $(OBJ)
	$(RM) $(EXE)
	$(MAKE) -C server clean
	$(MAKE) -C daemon clean
	$(MAKE) -C config clean