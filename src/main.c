#include <math.h>
#include <stdio.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

char map_brightness_to_char(int brightness)
{
	//$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'.
	const char *palette =
		" .'`\",:;Il!i<>~+_-?[]{}1()|\\/tfjrxnuvczXYUJCLQ0OZmwpqdbkhao*#MWAG&8%B@$";
	int n = strlen(palette);

	float b = pow(brightness, 0.95);
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

int main(int argc, char **argv)
{
	unsigned char *grayscale = NULL;
	unsigned char *original = NULL;
	int exit_status = 0;

	if (argc < 2) {
		fprintf(stderr, "No image file provided\n");
		exit_status = 1;
		goto cleanup;
	}

	const char *infile = argv[1];

	int iwidth = 0;
	int iheight = 0;
	int channels = 0;
	original = stbi_load(infile, &iwidth, &iheight, &channels, 0);
	if (!original) {
		fprintf(stderr, "%s\n", stbi_failure_reason());
		exit_status = 1;
		goto cleanup;
	}

	int desired_width = 80;
	int desired_height = ((float)desired_width / iwidth) * iheight;
	stbir_resize_uint8(original, iwidth, iheight, 0, original,
			   desired_width, desired_height, 0, channels);

	// returns a row-major 1d array
	grayscale = get_grayscale(original, desired_width, desired_height,
				  channels);
	if (!grayscale) {
		fprintf(stderr, "Enable to allocate memory.\n");
		exit_status = 1;
		goto cleanup;
	}

	for (int i = 0; i < desired_height * desired_width; ++i) {
		char c = map_brightness_to_char(grayscale[i]);
		printf("%c", c);
		if (i % desired_width == 0)
			printf("\n");
	}
	printf("\n");

cleanup:
	stbi_image_free(original);
	free(grayscale);
	return exit_status;
}
