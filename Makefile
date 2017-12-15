CFLAGS = -std=gnu99 -Wall -lm
RM = rm -rf

OUT = build
EXEC = test

.PHONY: all clean run
all: $(EXEC)

run: $(EXEC)
	@./$(OUT)/$(EXEC)

$(EXEC): cg.c test.c
	mkdir -p $(OUT)
	$(CC) -o $(OUT)/$@ $^ $(CFLAGS)

clean:
	$(RM) $(OUT)
