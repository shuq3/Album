#include "Image.h"
#include "lcd.h"

void showImage(Image image, int x, int y) {
	int i, j;
	for (i = 0; i < image.height; i++) {
		for (j = 0; j < image.width; j++) {
			lcd_draw_pixel(x + i, y + j, image.pixel[i * image.width + j]);
		}
	}
}

void setImage(Image *image, int* pixel, int h, int w) {
	image->pixel = pixel;
	image->height = h;
	image->width = w;
}

void mixRGB(Image *img, char* r, char *g, char *b) {
	int i;
	for (i = 0; i < img->height * img->width; i++) {
		img->pixel[i] = 0xff000000 |
					   (0xff & (int)r[i]) << 16 |
					   (0xff & (int)g[i]) << 8 |
					   (0xff & (int)b[i]);
	}
}
