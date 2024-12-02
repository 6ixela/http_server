include ../flags.mk

MODULE = config
OBJ = $(MODULE).o
LIB = lib$(MODULE).a

$(MODULE): $(LIB)($(OBJ))

include ../libs.mk

main:main.o $(OBJ)
	gcc -o $@ $^

clean:
	$(RM) $(OBJ) $(LIB) $(addprefix $(SRC_DIR), $(LIBS))

.PHONY: all clean $(MODULE)