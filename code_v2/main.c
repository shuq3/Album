#include "common.h"
#include "sdcard/ff.h"

#define delay_with_led(time) do {led_flash(); delay(time); led_flash();} while (0)

STATUS status; // STOP, START, AUTOSTART

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

int is_blank(unsigned char *s, int l) {
	int i;
	for (i = 0; i < l; i++) {
		if (s[i] != 0xff)
			return 0;
		s[i] = i;
	}
	return 1;
}

int is_consist(unsigned char *s,unsigned char *t, int l) {
	int i;
	for (i = 0; i < l; i++) {
		if (s[i] != t[i])
			return 0;
		s[i] = 0;
	}
	return 1;
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
	result = f_mount(&fatFs, "", 0);
	result = f_open(&file, "test.txt", FA_READ);
	if (result != FR_OK) {
		printf("cannot open the file\r\n");
		return;
	}
	/* Read data from the file */
	f_read(&file, memoryBuffer, 512, &readByte);
	printf("totally %d bytes has been read.\r\n", readByte);
	printf("%s\r\n", memoryBuffer);

	f_close(&file);		/* Close the file */
	while(1);
}

int main() {
	led_init();
	play_effect();
	// nand_test();
	sdcard_test();
	status = STOP;
	enter_begining_menu();
	image_init();
	timer_init(0,65,4,62500*4,0); // 4s
	led_hex_count_forever();
	// led_cycle_forever();
	
	return 0;
}
