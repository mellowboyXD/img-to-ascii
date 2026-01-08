CC=gcc
CFLAGS=-I./include/ -Wall -Wextra -g

all: build

build: src/main.c include/stb_image.h
	$(CC) $(CFLAGS) -o img-to-ascii src/main.c

clean: img-to-ascii
	rm img-to-ascii
