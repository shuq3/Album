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
FIL fpbmp;

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

void image_init() {
  image_num = TOTAL_IMAGES;
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
  for (i = 0 ; i < TOTAL_IMAGES; i++) {
    printf("Loading image %d from SD card...\r\n", i+1);
    char str[80];
    getImageName(str, i);
    int openfile = f_open(&fpbmp, str, FA_READ);
    if (openfile != FR_OK) {
     printf("cannot open the file\r\n");
     return;
    }
    bmpHeaderPartLength();        //Get the length of Header Part
    BmpWidthHeight();             //Get the width and width of the Data Part

    bmpDataPart(i);                //Reserve the data to file
    image_set(images + i, ADDR(i), height, width);
    f_close(&fpbmp);
  }
}

void bmpDataPart(int id)
{
  int i, j=0;
  int stride;
  unsigned char* pix=0x56000400;
  f_lseek(&fpbmp, OffSet);
  stride=(24 *(int)width+31)/8;
  stride=stride/4 * 4;
  unsigned long color;
  unsigned char red, green, blue;
  for(j=0;j<height;j++) {
    f_read(&fpbmp, pix, stride, &stride);
    for(i=0;i<width;i++)
    {
      red =pix[i*3+2];
      green =pix[i*3+1];
      blue =pix[i*3];
      color = 0xff000000;
      color |= ((int)red   << 16);
      color |= ((int)green << 8 );
      color |= ((int)blue       );
      ADDR(id)[(height-1-j)*height+i] = color;
    }
  }
}

/* To get the OffSet of header to data part */
void bmpHeaderPartLength()
{
     int readByte = 4;
     f_lseek(&fpbmp, 10);
     f_read(&fpbmp, &OffSet, readByte, &readByte);
    //  printf("The Header Part is of length %d.\r\n", OffSet);
}

/* To get the width and height of the bmp FIL */
void BmpWidthHeight()
{
     int readByte = 4;
     f_lseek(&fpbmp, 18);
     f_read(&fpbmp, &width, readByte, &readByte);
     readByte = 4;
     f_lseek(&fpbmp, 22);
     f_read(&fpbmp, &height, readByte, &readByte);
    //  printf("The Width of the bmp file is %ld.\r\n", width);
    //  printf("The Height of the bmp file is %ld.\r\n", height);
}
