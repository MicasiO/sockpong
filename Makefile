TARGET = sockpong

CC = gcc

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    CC = clang
    FLAGS = $(shell pkg-config --cflags --libs raylib)
    TESTFLAGS = -g -fsanitize=address,undefined -Wall $(FLAGS)
else
    CC = gcc
    FLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt
    TESTFLAGS = -g -fsanitize=address,leak,undefined,pointer-compare,pointer-subtract -Wall $(FLAGS)
endif

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
