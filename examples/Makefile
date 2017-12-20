include ../global.mk

SRC = ../src
OUT = ../output
EXEC := demo
EXEC := $(addprefix $(OUT)/,$(EXEC))

.PHONY: all clean run
all: $(OUT) $(EXEC)

run: $(EXEC)
	@./$(EXEC)

$(OUT):
	mkdir -p $(OUT)

$(EXEC): demo.c
	$(CC) -o $@ $< ../$(LIB_PATH)/$(LIB_NAME) -I$(SRC) -Wl,-rpath=../$(LIB_PATH) -lm

clean:
	$(RM) $(EXEC)
