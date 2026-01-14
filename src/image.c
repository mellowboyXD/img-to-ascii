#include <stdio.h>
#include <stdlib.h>
#include "image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

struct img *load_img(const char *path, int desired_width, char *errmsg)
{
	if (desired_width < 0) {
		sprintf(errmsg, "negative width not valid");
		return NULL;
	}

	int w = 0;
	int h = 0;
	int channels = 0;

	unsigned char *img_data = stbi_load(path, &w, &h, &channels, 0);
	if (!img_data) {
		sprintf(errmsg, "%s", stbi_failure_reason());
		return NULL;
	}

	if (desired_width > w) {
		sprintf(errmsg, "width invalid: %d>%d", desired_width, w);
		return NULL;
	}

	float desired_height = ((float)desired_width / (w * 2.0f)) * h;
	stbir_resize_uint8(img_data, w, h, 0, img_data, desired_width,
			   desired_height, 0, channels);

	struct img *image = malloc(sizeof(struct img));
	if (!image) {
		sprintf(errmsg, "could not allocate struct img");
		return NULL;
	}

	image->data = img_data;
	image->width = desired_width;
	image->height = desired_height;
	image->channels = channels;

	return image;
}

struct img *grayscale_img(struct img *image, char *errmsg)
{
	struct img *grayscale = malloc(sizeof(struct img));
	if (grayscale == NULL) {
		sprintf(errmsg, "could not malloc");
		return NULL;
	}

	grayscale->data = malloc(image->width * image->height);
	if (!grayscale->data) {
		sprintf(errmsg, "could not malloc data");
		return NULL;
	}

	grayscale->width = image->width;
	grayscale->height = image->height;
	grayscale->channels = 1;

	for (int i = 0; i < image->height * image->width; ++i) {
		int j = i * image->channels;
		int r = image->data[j];
		int g = image->data[j + 1];
		int b = image->data[j + 2];

		grayscale->data[i] = 0.299 * r + 0.587 * g + 0.114 * b;
	}

	return grayscale;
}

void close_img(struct img *image)
{
	stbi_image_free(image->data);
	free(image);
}
