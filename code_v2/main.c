#include "common.h"

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
		 "\tpress K2 can open/close"
		 " the function of playing photos automatically\r\n"
		 );
}

int main() {
	led_init();
	play_effect();
	status = STOP;
	enter_begining_menu();
	image_init();
	timer_init(0,65,4,62500*4,0); // 4s
	led_hex_count_forever();
	// led_cycle_forever();
	
	return 0;
}
