#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

#include "ascii.h"

unsigned char *load_img(const char *path, int desired_width, int *height,
			int *chan, char *errmsg)
{
	unsigned char *img = NULL;

	int w = 0;
	int h = 0;
	int channels = 0;

	img = stbi_load(path, &w, &h, &channels, 0);
	if (!img) {
		errmsg = (char *)stbi_failure_reason();
		return NULL;
	}
	printf("image loaded\n");

	if (desired_width < 0) {
		errmsg = "negative width not valid";
		return NULL;
	} else if (desired_width > w) {
		sprintf(errmsg, "width invalid: %d>%d", desired_width, w);
		return NULL;
	}

	float desired_height = ((float)desired_width / (w * 2.0f)) * h;
	stbir_resize_uint8(img, w, h, 0, img, desired_width, desired_height, 0,
			   channels);

	*height = desired_height;
	*chan = channels;

	printf("image resized\n");

	return img;
}

int main(int argc, char **argv)
{
	char errmsg[1024] = "";
	unsigned char *original = NULL;
	unsigned char *grayscale = NULL;
	int exit_status = 0;

	if (argc < 2) {
		fprintf(stderr, "No image file provided\n");
		exit_status = 1;
		goto cleanup;
	}

	const char *infile = argv[1];

	int width = 80;
	int height = 0;
	int channels = 0;
	original = load_img(infile, width, &height, &channels, errmsg);
	if (!original) {
		fprintf(stderr, "Error loading image: %s\n", errmsg);
		exit_status = 1;
		goto cleanup;
	}

	// returns a row-major 1d array
	grayscale = get_grayscale(original, width, height, channels);
	if (!grayscale) {
		fprintf(stderr, "Enable to allocate memory.\n");
		exit_status = 1;
		goto cleanup;
	}

	printf("image grayscaled\n");
	printf("w: %d h: %d channels: %d\n", width, height, channels);

	for (int i = 0; i < width * height; ++i) {
		char c = map_brightness_to_char(grayscale[i]);
		printf("%c", c);
		if (i % width == 0)
			printf("\n");
	}
	printf("\n");

cleanup:
	stbi_image_free(original);
	free(grayscale);
	return exit_status;
}
