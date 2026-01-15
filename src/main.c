#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "ascii.h"

int main(int argc, char **argv)
{
	char errmsg[1024] = "";
	FILE *output = stdout;
	int exit_status = 0;
	bool file_output_flag = false;

	if (argc < 2) {
		fprintf(stderr, "No input file provided\n");
		exit_status = 1;
		goto main_close_output_file_and_exit;
	}

	const char *infile = argv[1];
	const char *outfile = NULL;

	static struct option long_opt[] = {
		{ "width", required_argument, 0, 'w' },
		{ "output", required_argument, 0, 'o' },
		{ 0, 0, 0, 0 }
	};

	int width = 80;
	int c;
	while ((c = getopt_long(argc, argv, "w:o:h", long_opt, &optind)) !=
	       -1) {
		switch (c) {
		case 'w':
			width = atoi(optarg);
			break;

		case 'o':
			outfile = optarg;
			output = fopen(outfile, "w");
			if (!output) {
				fprintf(stderr, "Error: could not open %s\n",
					outfile);
				exit_status = 1;
				goto main_close_output_file_and_exit;
			}
			file_output_flag = true;
			break;

		default: /* '?' */
			fprintf(stderr, "Usage: %s <image_file>\n", argv[0]);
			goto main_close_output_file_and_exit;
		}
	}

	if (img2ascii(infile, width, output, errmsg) == 1) {
		exit_status = 1;
	}

	if (file_output_flag) {
		fprintf(stdout, "Output file '%s' created\n", outfile);
	}

main_close_output_file_and_exit:
	if (file_output_flag)
		fclose(output);
	return exit_status;
}
