#ifndef __IMAGE_H__
#define __IMAGE_H__

#define TOTAL_IMAGES	256


#define RAM(x) 			((unsigned long *)(x))
#define IMG_ADDR_BASE1	0x57000000
#define NORM_IMG_SIZE	0x80000
// 512KB
#define ADDR(n) 		RAM(IMG_ADDR_BASE1 + n * NORM_IMG_SIZE)
#define IMG_ADDR_BASE2	0x58000000
#define HC_IMG_SIZE		0x400000
// 4MB
#define ADDR2(n) 		RAM(IMG_ADDR_BASE2 + n * HC_IMG_SIZE)

typedef struct {
  int *dat;
  int height;
  int width;
} Image;

#ifdef MAKE_PUBLIC

extern int image_num;
extern int image_id;
extern Image images[TOTAL_IMAGES];

void load_data();
void image_set(Image *image, int* dat, int h, int w);
void image_sync();
// #define image_sync() do {this_image = images + image_id;} while (0)
// void mixRGB(Image *img, char* r, char *g, char *b);

#endif

void image_init();
void image_show();
void image_move(int x, int y);
void image_show_lr(); // default shown in the center
void image_show_rl(); // default shown in the center
void image_set(Image *image, int* dat, int h, int w);
void next_image();
void previous_image();
void bmpHeaderPartLength(); // To get the OffSet of header to data part
void BmpWidthHeight(); // To get the width and height of the bmp FIL
void bmpDataPart(); //get r,g,b data
// read bmg file
void readimage();
// get image file name
void getImageName(char *buffer, int id);


#endif
