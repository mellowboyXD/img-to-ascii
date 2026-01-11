CC=gcc
CFLAGS=-I./include/ -Wall -Wextra -g
CLIBS=-lm

all: build

build: src/main.c include/stb_image.h include/stb_image_resize.h
	$(CC) $(CFLAGS) $(CLIBS) -o bin/img2ascii src/main.c

clean: bin/
	rm bin/*
