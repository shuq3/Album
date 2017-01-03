#include "common.h"
#include "fatfs/ff.h"

#define delay_with_led(time) do {led_flash(); delay(time); led_flash();} while (0)

STATUS status; // STOP, START, AUTO, EDIT

// 延时
void delay(volatile int t) {
  while (t--);
}

void play_effect() {
  lcd_clear_screen(LCD_RED);
  delay_with_led(1000000);
  lcd_clear_screen_lr(LCD_GREEN);
  delay_with_led(1000000);
  lcd_clear_screen_rl(LCD_BLUE);
  delay_with_led(1000000);
  lcd_clear_screen_bu(LCD_CYAN);
  delay_with_led(1000000);
  lcd_clear_screen_lr(LCD_MAGENTA);
  delay_with_led(1000000);
  lcd_clear_screen_rl(LCD_YELLOW);
  delay_with_led(1000000);
  lcd_clear_screen(LCD_WHITE);
}

void enter_begining_menu() {
  lcd_draw_char_reset();
  puts("\n\twelcome!\r\n"
     "\tpress K1 to start\r\n"
     "\tAfter starting:\r\n"
     "\tpress K4 to show next photo\r\n"
     "\tpress K3 to show last photo\r\n"
     "\tpress K2 to enter/exit auto-play mode\r\n"
     "\tpress K7 to enter/exit edit mode\r\n"
     "\tin edit mode, you can move the photo by K3-K6\r\n"
     );
}

// 这两个struct比较大，要放在全局，放函数栈里可能会溢出
FATFS fatFs;		/* FatFs work area needed for each volume */
FIL file;			/* File object needed for each open file */
void sdcard_test() {
  lcd_draw_char_reset();
  printf("testing sdcard and the FAT file system\r\n");
  char *memoryBuffer = 0x56000000;
  int result;
  int readByte;
  result = f_open(&file, "1/test.txt", FA_READ);
  if (result != FR_OK) {
    printf("cannot open the file\r\n");
    return;
  }
  /* Read data from the file */
  f_read(&file, memoryBuffer, 51200, &readByte);
  printf("totally %d bytes has been read.\r\n", readByte);
  printf("%s\r\n", memoryBuffer);

  f_close(&file);		/* Close the file */
  while(1);
}

int main() {
  led_init();
  play_effect();
  f_mount(&fatFs, "", 0);
  // sdcard_test();
  status = STOP;
  image_init();
  enter_begining_menu();
  timer_init(0,65,4,62500*4,0); // 4s
  led_hex_count_forever();
  return 0;
}
