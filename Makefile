CC = gcc

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    TARGET = sockpong_mac
    CC = clang
    FLAGS = $(shell pkg-config --cflags raylib)
    TESTFLAGS = -g -fsanitize=address,undefined -Wall $(FLAGS)
    FRAMEWORKS = src/libraylib.a -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
else
    TARGET = sockpong_linux
    CC = gcc
    FLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt
    TESTFLAGS = -g -fsanitize=address,leak,undefined,pointer-compare,pointer-subtract -Wall $(FLAGS)
    FRAMEWORKS = 
endif

SOURCES = $(wildcard src/*.c) $(wildcard src/network/*.c) $(wildcard src/ui/*.c)

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $(TARGET) $(FLAGS) $(FRAMEWORKS)

clean:
	rm -f $(TARGET)

install: $(TARGET)
	mkdir -p /usr/local/bin
	cp $(TARGET) /usr/local/bin/

uninstall:
	rm -f /usr/local/bin/$(TARGET)

test:
	$(CC) $(SOURCES) -o $(TARGET) $(TESTFLAGS)
