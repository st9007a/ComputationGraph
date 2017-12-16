include ../global.mk

SRC = ../src
OUT = ../build

EXEC := \
	   test_demo
EXEC := $(addprefix $(OUT)/,$(EXEC))

.PHONY: all clean check
all: $(EXEC)

$(OUT):
	mkdir -p $@

$(OUT)/test_%: test_%.c $(OUT)
	$(CC) -o $@ $< ../$(LIB_PATH)/$(LIB_NAME) -I$(SRC) -Wl,-rpath=../$(LIB_PATH) -lm

check: $(EXEC)
	@for case in $^; do \
		./$$case; \
	done;

clean:
	$(RM) $(EXEC)
	rmdir $(OUT)
