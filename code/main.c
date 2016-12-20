#include "lcd.h"
#include "Image.h"
#include "ImgData.h"

void init_uart();


// 201*201
int pixel[201*201];

//266*361
int data1[] = {
DAT1
};

//260*328
int data2[] = {
DAT2
};

volatile unsigned long *gpkcon0 = (volatile unsigned long *)0x7F008800;
volatile unsigned long *gpkdat = (volatile unsigned long *)0x7F008808;


void delay(volatile int t) {
  *gpkdat = ~(*gpkdat);
  while (t--);
  *gpkdat = ~(*gpkdat);
}

void test2() {
  do {
    delay(1000000);
    lcd_clear_screen(0xFF0000);
    delay(1000000);
    lcd_clear_screen_lr(0x00FF00);
    delay(1000000);
    lcd_clear_screen_rl(0x0000FF);
    delay(1000000);
    lcd_clear_screen_bu(~0xFF0000);
    delay(1000000);
    lcd_clear_screen_lr(~0x00FF00);
    delay(1000000);
    lcd_clear_screen_rl(~0x0000FF);
  } while(0);
}

void preTest() {
  delay(100000);
  lcd_clear_screen(0xFFFFFF);
  delay(100000);
  lcd_clear_screen_lr(0);
  delay(100000);
  lcd_clear_screen_rl(0xFFFFFF);
  delay(100000);
  lcd_clear_screen_bu(0);
  test2();
  delay(100000);
  lcd_clear_screen(0xFFFFFF);
}

int main(void)
{
  //LED
  *gpkcon0 = 0x11110000;
  *gpkdat = 0;
  // init_uart();
  // getc();
  // 初始化LCD控制器
  lcd_init();

  // 清屏
  preTest();

  Image image[3];
  setImage(image, pixel, 201, 201);
  mixRGB(image, img_red, img_green, img_blue);

  setImage(image + 1, data1, 266, 361);
  setImage(image + 2, data2, 260, 328);

  int image_num = 3;
  int i = 0;

  while (1) {
    showImageLR(image[i], (ROW - image[i].height)/2, (COL - image[i].width) / 2);
    // i = (i + 1) % image_num; 报错 undefined reference to `__aeabi_idivmod'
    i++;
    if (i == image_num)
      i = 0;
    delay(10000000);
    lcd_clear_screen_lr(0xFFFFFF);
  }

  return 0;
}
