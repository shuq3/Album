#ifndef __IMAGE_H__
#define __IMAGE_H__

typedef struct {
	int *pixel;
	int height;
	int width;
} Image;

void showImage(Image image, int x, int y);
void showImageLR(Image image, int x, int y);
void showImageRL(Image image, int x, int y);
void setImage(Image *image, int* pixel, int h, int w);
void mixRGB(Image *img, char* r, char *g, char *b);

#endif
