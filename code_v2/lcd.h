#ifndef __LCD_H__
#define __LCD_H__

#include "s3c6410map.h"

#define FRAME_BUFFER	0x54000000
#define ROW				272
#define COL				480
#define HSPW			(2)
#define HBPD			(40- 1)
#define HFPD			(5 - 1)
#define VSPW			(2)
#define VBPD			(8 - 1)
#define VFPD			(9 - 1)
#define LINEVAL 		(271)
#define HOZVAL			(479)
#define LeftTopX		0
#define LeftTopY		0
#define RightBotX		479
#define RightBotY		271

#define LCD_WHITE		0xFFFFFF
#define LCD_BLACK		0x000000
#define LCD_RED			0xFF0000
#define LCD_GREEN		0x00FF00
#define LCD_BLUE		0x0000FF
#define LCD_CYAN		0x00FFFF
#define LCD_MAGENTA		0xFF00FF
#define LCD_YELLOW		0xFFFF00

#define TAB_LENTH		4


void lcd_init(void);
void lcd_draw_pixel(int row, int col, int color);
void lcd_clear_screen(int color);
void lcd_clear_screen_lr(int color);
void lcd_clear_screen_rl(int color);
void lcd_clear_screen_bu(int color);

// #define LCDFUNC
#ifdef LCDFUNC
void lcd_draw_hline(int row, int col1, int col2, int color);
void lcd_draw_vline(int col, int row1, int row2, int color);
void lcd_draw_cross(int row, int col, int halflen, int color);
void lcd_draw_circle(void);
#endif

void lcd_draw_char_reset();
void lcd_draw_char(unsigned char c);
void puts(const char* str);

#endif
