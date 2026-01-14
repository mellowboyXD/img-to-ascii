#ifndef IMAGE_H /* image.h */
#define IMAGE_H

struct img {
	unsigned char *data;	// 8 bytes
	int width;		// 4 bytes
	int height;		// 4 bytes
	int channels;		// 4 bytes
}; // 20 bytes

struct img *load_img(const char *path, int desired_width, char *errmsg);
struct img *grayscale_img(struct img *image, char *errmg);
void close_img(struct img *image);

#endif /* image.h */
