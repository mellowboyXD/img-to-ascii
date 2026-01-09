#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

int main(int argc, char **argv)
{
	if (argc < 3) {
		fprintf(stderr, "No image file provided\n");
		exit(EXIT_FAILURE);
	} else if (argc < 2) {
		fprintf(stderr, "No output file name provided");
		exit(EXIT_FAILURE);
	}

	const char *infile = argv[1];
	const char *outfile = argv[2];

	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char *data = stbi_load(infile, &width, &height, &channels, 0);
	if (data == NULL) {
		const char *errmsg = stbi_failure_reason();
		fprintf(stderr, "%s\n", errmsg);
		exit(EXIT_FAILURE);
	}

	// returns a row-major 1d array
	unsigned char *buf = (unsigned char *)malloc(width * height * 1);
	if (buf == NULL) {
		fprintf(stderr, "Enable to allocate memory.\n");
		stbi_image_free(data);
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < height * width; ++i) {
		int j = i * channels;
		int r = data[j];
		int g = data[j + 1];
		int b = data[j + 2];

		buf[i] = 0.299 * r + 0.587 * g + 0.114 * b;
	}

	if (stbi_write_png(outfile, width, height, 1, buf, 0) == 0) {
		fprintf(stderr, "Error creating: %s\n", outfile);
		stbi_image_free(data);
		free(buf);
		exit(EXIT_FAILURE);
	}

	stbi_image_free(data);
	free(buf);
}
