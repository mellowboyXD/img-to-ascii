CC=gcc
CFLAGS=-I./include/ -Wall -Wextra -g
CLIBS=-lm

all: build

build: src/*.c include/stb_image.h include/stb_image_resize.h
	$(CC) $(CFLAGS) $(CLIBS) -o bin/img2ascii src/main.c src/ascii.c

clean: bin/
	rm bin/*
