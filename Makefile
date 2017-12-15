CFLAGS = -std=gnu99 -Wall -lm
RM = rm -rf

OUT = build
EXEC = test

.PHONY: all clean run
all: $(OUT)/$(EXEC)

run: $(OUT)/$(EXEC)
	@./$(OUT)/$(EXEC)

$(OUT)/$(EXEC): cg.c test.c
	mkdir -p $(OUT)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	$(RM) $(OUT)
