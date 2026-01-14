#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

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

	static struct option long_opt[] = {
		{ "width", required_argument, 0, 'w' }, { 0, 0, 0, 0 }
	};

	int width = 80;
	int c;
	while ((c = getopt_long(argc, argv, "w:h", long_opt, &optind)) != -1) {
		switch (c) {
		case 'w':
			width = atoi(optarg);
			break;

		default: /* '?' */
			fprintf(stderr, "Usage: %s <image_file>\n", argv[0]);
			return 1;
		}
	}

	if (img2ascii(infile, width, output, errmsg) == 1) {
		return 1;
	}
	return 0;
}
