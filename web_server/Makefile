export CC = gcc
export CFLAGS = `pkg-config --cflags glib-2.0` -Wall -Wextra -pthread -Werror -Wvla -std=c99 -pedantic -fsanitize=address -g
export CPPFLAGS = -Isrc -Iinclude
export LDFLAGS = -fsanitize=address
export LDLIBS = `pkg-config --libs glib-2.0`

EXE = http_server

all: $(EXE)

$(EXE):
	$(MAKE) -C src
	mv src/$(EXE) .

.PHONY: all

clean:
	$(RM) $(EXE)
	$(MAKE) -C src clean