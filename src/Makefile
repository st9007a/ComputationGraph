include ../global.mk
RM = rm -rf

SOVERSION ?= 1
REALVERSION ?= 1.0.0

OBJS = \
      graph.o

OUT = ../$(LIB_PATH)
TARGET = $(OUT)/$(LIB_NAME)

CFLAGS = -std=gnu99 -Wall -fPIC -lm
SOFLAGS = -shared -Wl,-soname,$(TARGET).$(SOVERSION)

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(TARGET): $(OBJS)
	mkdir -p $(OUT)
	$(CC) $(SOFLAGS) -o $(TARGET).$(REALVERSION) $^
	ln -s -f $(TARGET).$(REALVERSION) $(TARGET).$(SOVERSION)
	ln -s -f $(TARGET).$(REALVERSION) $(TARGET)

clean:
	$(RM) $(OBJS) $(OUT)
