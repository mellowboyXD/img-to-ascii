CC=gcc
CFLAGS=-I./include/ $(shell pkg-config --cflags libavcodec libavformat libavutil) -Wall -Wextra -g
CLIBS=-lm $(shell pkg-config --libs libavcodec libavformat libavutil)

all: build

build: src/*.c include/stb_image.h include/stb_image_resize.h
	$(CC) $(CFLAGS) $(CLIBS) -o bin/img2ascii src/main.c src/ascii.c src/image.c

video: src/video.c
	$(CC) $(CFLAGS) $(CLIBS) -o bin/video src/video.c src/ascii.c src/image.c

clean: bin/
	rm bin/*
