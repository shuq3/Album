#include "common.h"
#include "fatfs/ff.h"

typedef struct {
  int x;
  int y;
} Vec2;

int image_num;
int image_id;

Image images[TOTAL_IMAGES];
Image *this_image;
FIL file;

Vec2 position;

unsigned int OffSet;
long width;
long height;

//***************************
// private functions
void load_data();
void image_set(Image *image, int* dat, int h, int w);
void image_sync();
// #ifndef MAKE_PUBLIC
// #define image_sync() do {this_image = images + image_id;} while (0)
// #endif
//***************************

int atoi(const char* str) {
	int sum = 0;
	int i;
	for (i = 0; str[i] != '\0'; i++) {
		sum *= 10;
		sum += str[i] - '0';
	}
	return sum;
}

unsigned int get_total_images() {
	int openfile = f_open(&file, "photos/photos.ini", FA_READ);
    if (openfile != FR_OK) {
     printf("cannot open the file: photos.ini\r\n");
     return;
    }
    char *buffer = 0x56000000;
    unsigned int has_read;
    f_read(&file, buffer, 16, &has_read);
    f_close(&file);
    return atoi(buffer);
}

void image_init() {
  image_num = get_total_images();
  image_id = 0;
  readimage();
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
  // led_cycle_once();
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

void image_sync() {
  this_image = images + image_id;
  position.x = (ROW - this_image->height) / 2;
  position.y = (COL - this_image->width) / 2;
}

void getImageName(char *buffer, int id) {
  strcpy(buffer, "photos/");
  unsigned int s;
  unsigned int div = 10;
  soft_udiv(&id, &div, &s);

  // 添加十位
  if (s) {
    char ten[] = {(char)(s + '0'), '\0'};
    strcat(buffer, ten);
  }
  // 添加个位
  char one[] = {(char)(id + '0'), '\0'};
  strcat(buffer, one);
  strcat(buffer, ".bmp");
}


void readimage() {
  lcd_draw_char_reset();
  int i;
  for (i = 0 ; i < image_num; i++) {
    if (i == 0) {
      printf("\n\tLoading %d images from SD card...\r\n", image_num);
      printf("\tPlease wait for a moment...\r\n");
      printf("\t%2d images has been loaded\r", 0);
    }
    printf("\t%2d\r", i);
    char str[80];
    getImageName(str, i);
    int openfile = f_open(&file, str, FA_READ);
    if (openfile != FR_OK) {
     printf("cannot open the file\r\n");
     return;
    }
    bmpHeaderPartLength();        //Get the length of Header Part
    BmpWidthHeight();             //Get the width and width of the Data Part

    bmpDataPart(i);                //Reserve the data to file
    image_set(images + i, ADDR(i), height, width);
    f_close(&file);
  }
}

unsigned int getBytes(char *buffer, unsigned int size) {
	unsigned int has_read;
	if (size < 512) {
		f_read(&file, buffer, size, &has_read);
		return has_read;
	}
	f_read(&file, buffer, 512, &has_read);
	return has_read + getBytes(buffer + 512, size - 512);
}

void bmpDataPart(int id)
{
  int i, j=0;
  int stride;
  char* pix=0x56000000;
  f_lseek(&file, OffSet);
  stride= (24 * width + 31) / 8;
  stride &= ~0x3;
  int color;
  char red, green, blue;
  for(j = 0; j < height; j++) {
  	getBytes(pix, stride);
    for(i = 0; i < width; i++)
    {
      red =pix[i*3+2];
      green =pix[i*3+1];
      blue =pix[i*3];
      color = 0xff000000;
      color |= (red   << 16);
      color |= (green << 8 );
      color |= (blue       );
      ADDR(id)[(height - 1 - j) * width + i] = color;
    }
  }
}

/* To get the OffSet of header to data part */
void bmpHeaderPartLength()
{
     int readByte = 4;
     f_lseek(&file, 10);
     f_read(&file, &OffSet, readByte, &readByte);
}

/* To get the width and height of the bmp FIL */
void BmpWidthHeight()
{
     int readByte = 4;
     f_lseek(&file, 18);
     f_read(&file, &width, readByte, &readByte);
     int readByte2 = 4;
     f_lseek(&file, 22);
     f_read(&file, &height, readByte2, &readByte2);
}
