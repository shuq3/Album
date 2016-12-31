#include "s3c6410map.h"
#include "led.h"
#include "lcd.h"
#include "image.h"

#define delay_with_led(time) do {led_flash(); delay(time); led_flash();} while (0)
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
	puts("\n\twelcome!\r\n");
	puts("\tpress K1 to start\r\n");
}

int main() {
	led_init();
	play_effect();
	enter_begining_menu();
	

	delay(10000000);
	image_init();
	lcd_clear_screen_lr(LCD_WHITE);
	image_show_lr();

	timer_init(0,65,4,62500*4,0); // 4s
	timer_enable();//timer_disable();
	led_hex_count_forever();
	// led_cycle_forever();
	
	return 0;
}
