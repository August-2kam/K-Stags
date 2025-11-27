CC      = gcc
CFLAGS  = -Wall -g
SRC     = main.c \
          machine.c \
          debugScreen.c
TARGET  = stags
LIBS	= -lncurses

DEBUG ?= 0

ifeq ($(DEBUG),1)
    CFLAGS += -DDEBUG 
endif

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)



