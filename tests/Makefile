include ../global.mk

SRC = ../src
OUT = ../build

EXEC := \
	   test_node \
	   test_matrix
EXEC := $(addprefix $(OUT)/,$(EXEC))

.PHONY: all clean check
all: $(OUT) $(EXEC)

$(OUT):
	mkdir -p $@

$(OUT)/test_%: test_%.c
	$(CC) -o $@ $< ../$(LIB_PATH)/$(LIB_NAME) -I$(SRC) -Wl,-rpath=../$(LIB_PATH) -lm

check: $(OUT) $(EXEC)
	@for case in $(EXEC); do \
		./$$case; \
	done;

clean:
	$(RM) $(EXEC)
	rmdir $(OUT)
