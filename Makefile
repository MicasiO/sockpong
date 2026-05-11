TARGET = sockpong

CC = gcc

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	FLAGS = -lncurses -lm -lc
else
	FLAGS = -lncursesw -lm -lc
endif

TESTFLAGS = -g -fsanitize=address,leak,undefined,pointer-compare,pointer-subtract -lncursesw -lform -lm -Wall

SOURCES = $(wildcard src/*.c) $(wildcard src/network/*.c) $(wildcard src/ui/*.c)

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $(TARGET) $(FLAGS)

clean:
	rm -f $(TARGET)

install: $(TARGET)
	mkdir -p /usr/local/bin
	cp $(TARGET) /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/$(TARGET)

test:
	$(CC) $(SOURCES) -o $(TARGET) $(TESTFLAGS)
