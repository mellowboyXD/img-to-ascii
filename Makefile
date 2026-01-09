CC=gcc
CFLAGS=-I./include/ -Wall -Wextra -g
CLIBS=-lm

all: build

build: src/main.c include/stb_image.h
	$(CC) $(CFLAGS) $(CLIBS) -o bin/grayscale src/main.c

clean: bin/
	rm bin/*
