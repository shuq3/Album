#include "image.h"
#include "lcd.h"
#include "led.h"
#include "IMGDAT.h"

typedef struct {
	int x;
	int y;
} Vec2;

int image_num;
int image_id;

Image images[TOTAL_IMAGES];
Image *this_image;

Vec2 position;

int data1[] = {DAT1}; // 120*180
int data2[] = {DAT2}; // 120*180

//***************************
// private functions
void load_data();
void image_set(Image *image, int* dat, int h, int w);
void image_sync();
// #ifndef MAKE_PUBLIC
// #define image_sync() do {this_image = images + image_id;} while (0)
// #endif
// void mixRGB(Image *img, char* r, char *g, char *b);
//***************************

void image_init() {
	image_num = TOTAL_IMAGES;
	image_id = 0;
	load_data();
	image_sync();
}

void image_show() {
	int i, j;
	for (i = 0; i < this_image->height; i++) {
		for (j = 0; j < this_image->width; j++) {
			lcd_draw_pixel(
				position.x + i, position.y + j, 
				this_image->dat[i * this_image->width + j]);
		}
	}
}

void image_show_lr() {
	int i, j;
	for (j = 0; j < this_image->width; j++) {
		for (i = 0; i < this_image->height; i++) {
			lcd_draw_pixel(
				position.x + i, position.y + j, 
				this_image->dat[i * this_image->width + j]);
		}
	}
}

void image_show_rl() {
	int i, j;
	for (j = this_image->width - 1; j >= 0; j--) {
		for (i = 0; i < this_image->height; i++) {
			lcd_draw_pixel(
				position.x + i, position.y + j, 
				this_image->dat[i * this_image->width + j]);
		}
	}
}

void image_set(Image *image, int* dat, int h, int w) {
	image->dat = dat;
	image->height = h;
	image->width = w;
}

void next_image() {
	led_cycle_once();
	image_id++;
	if (image_id == image_num)
		image_id = 0;
	led_set_ex(image_id+1);
	image_sync();
}

void previous_image() {
	led_cycle_once();
	image_id--;
	if (image_id < 0)
		image_id = image_num - 1;
	led_set_ex(image_id+1);
	image_sync();
}

void image_move(int x, int y) {
	position.x += x;
	position.y += y;
}

void load_data() {
	image_set(images, data1, 120, 180);
	image_set(images + 1, data2, 120, 180);
}

void image_sync() {
	this_image = images + image_id;
	position.x = (ROW - this_image->height) / 2;
	position.y = (COL - this_image->width) / 2;
}

// void mixRGB(Image *img, char* r, char *g, char *b) {
// 	int i;
// 	for (i = 0; i < img->height * img->width; i++) {
// 		img->dat[i] = 0xff000000 |
// 					 (0xff & (int)r[i]) << 16 |
// 					 (0xff & (int)g[i]) << 8 |
// 					 (0xff & (int)b[i]);
// 	}
// }
