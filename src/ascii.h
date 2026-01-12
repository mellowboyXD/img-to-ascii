#ifndef ASCII /* ascii */
#define ASCII

char map_brightness_to_char(int brightness);

unsigned char *get_grayscale(unsigned char *data, int width, int height,
			     int channels);

#endif /* ascii */
