include ../flags.mk

MODULE = server
OBJ = $(MODULE).o queue.o shared_queue.o
LIB = lib$(MODULE).a

LIBS = libconfig.a
# $(MODULE): CFLAGS += `pkg-config --cflags glib-2.0`
# $(MODULE): LDLIBS = `pkg-config --libs glib-2.0`
$(MODULE): $(LIB)($(OBJ))

include ../libs.mk

main:main.o $(OBJ)
	gcc -o $@ $^

clean:
	$(RM) $(OBJ) $(LIB) $(addprefix $(SRC_DIR), $(LIBS))

.PHONY: all clean $(MODULE)