include ../global.mk
RM = rm -rf

RELEASE ?= 0
SOVERSION ?= 1
REALVERSION ?= 1.0.0

OBJS = \
	   node.o \
	   eval.o \
	   optimize.o \
	   matrix.o \
	   api.o

deps = $(OBJS:%.o=.%.o.d)

OUT = ../$(LIB_PATH)
TARGET = $(OUT)/$(LIB_NAME)

CFLAGS := -std=gnu99 -Wall -fPIC -lm
ifeq ($(RELEASE),1)
	CFLAGS += -DNDEBUG
endif

SOFLAGS = -shared -Wl,-soname,$(TARGET).$(SOVERSION)

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c -MMD -MF .$@.d $<

$(TARGET): $(OBJS)
	mkdir -p $(OUT)
	$(CC) $(SOFLAGS) -o $(LIB_NAME).$(REALVERSION) $^
	ln -s -f $(LIB_NAME).$(REALVERSION) $(LIB_NAME).$(SOVERSION)
	ln -s -f $(LIB_NAME).$(REALVERSION) $(LIB_NAME)
	mv $(LIB_NAME)* $(OUT)

clean:
	$(RM) $(OBJS) $(deps) $(OUT)
