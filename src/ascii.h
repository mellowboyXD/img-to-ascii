#ifndef ASCII /* ascii.h */
#define ASCII

#include <stdio.h>

char map_brightness_to_char(int brightness);
int img2ascii(const char *infile, int width, FILE *restrict stream,
	      char *errmsg);

#endif /* ascii.h */
