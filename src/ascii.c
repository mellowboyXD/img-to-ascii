#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "ascii.h"

char map_brightness_to_char(int brightness)
{
	const char *palette =
		" .'`\",:;Il!i<>~+_-?[]{}1()|\\/tfjrxnuvczXYUJCLQ0OZmwpqdbkhao*#MWAG&8%B@$";

	int n = strlen(palette);

	float b = pow(brightness, 0.98);
	int j = (int)(b * (n - 1) / 255);
	return palette[j];
}

unsigned char *get_grayscale(unsigned char *data, int width, int height,
			     int channels)
{
	unsigned char *grayscale = (unsigned char *)(malloc(width * height));
	if (grayscale == NULL)
		return NULL;
	for (int i = 0; i < height * width; ++i) {
		int j = i * channels;
		int r = data[j];
		int g = data[j + 1];
		int b = data[j + 2];

		grayscale[i] = 0.299 * r + 0.587 * g + 0.114 * b;
	}
	return grayscale;
}
