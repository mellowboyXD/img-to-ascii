#include "ascii.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "image.h"

char map_brightness_to_char(int brightness)
{
	const char *palette =
		" .'`\",:;Il!i<>~+_-?[]{}1()|\\/tfjrxnuvczXYUJCLQ0OZmwpqdbkhao*#MWAG&8%B@$";

	int n = strlen(palette);

	float b = pow(brightness, 0.98);
	int j = (int)(b * (n - 1) / 255);
	return palette[j];
}

int img2ascii(const char *infile, int width, FILE *restrict stream,
	      char *errmsg)
{
	struct img *original = NULL;
	struct img *grayscale = NULL;
	int exit_status = 0;

	original = load_img(infile, width, errmsg);
	if (!original) {
		fprintf(stderr, "Error loading image: %s\n", errmsg);
		exit_status = 1;
		goto cleanup;
	}

	grayscale = grayscale_img(original, errmsg);
	if (!grayscale) {
		fprintf(stderr, "Enable to allocate memory: %s\n", errmsg);
		exit_status = 1;
		goto cleanup;
	}

	for (int i = 0; i < grayscale->width * grayscale->height; ++i) {
		char c = map_brightness_to_char(grayscale->data[i]);
		fprintf(stream, "%c", c);
		if (i % width == 0)
			fprintf(stream, "\n");
	}
	fprintf(stream, "\n");

cleanup:
	close_img(original);
	close_img(grayscale);
	return exit_status;
}
