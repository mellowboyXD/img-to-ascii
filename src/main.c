#include <stdio.h>

#include "ascii.h"

int main(int argc, char **argv)
{
	char errmsg[1024] = "";
	FILE *output = stdout;

	if (argc < 2) {
		fprintf(stderr, "No input file provided\n");
		return 1;
	}

	const char *infile = argv[1];

	int width = 80;
	if (img2ascii(infile, width, output, errmsg) == 1) {
		return 1;
	}
	return 0;
}
