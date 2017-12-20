include ../global.mk

SRC = ../src
EXEC = demo

.PHONY: all clean run
all: $(EXEC)

run: $(EXEC)
	@./$(EXEC)

$(EXEC): demo.c
	$(CC) -o $@ $< ../$(LIB_PATH)/$(LIB_NAME) -I$(SRC) -Wl,-rpath=../$(LIB_PATH) -lm

clean:
	$(RM) $(EXEC)
