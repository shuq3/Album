#ifndef __LCD_H__
#define __LCD_H__

#define GPECON	(*((volatile unsigned long *)0x7F008080))
#define GPEDAT	(*((volatile unsigned long *)0x7F008084))
#define GPEPUD	(*((volatile unsigned long *)0x7F008088))
#define GPFCON	(*((volatile unsigned long *)0x7F0080A0))
#define GPFDAT	(*((volatile unsigned long *)0x7F0080A4))
#define GPFPUD	(*((volatile unsigned long *)0x7F0080A8))
#define GPICON	(*((volatile unsigned long *)0x7F008100))
#define GPIPUD	(*((volatile unsigned long *)0x7F008108))
#define GPJCON	(*((volatile unsigned long *)0x7F008120))
#define GPJPUD	(*((volatile unsigned long *)0x7F008128))

/* display controller */
#define MIFPCON			(*((volatile unsigned long *)0x7410800C))
#define SPCON			(*((volatile unsigned long *)0x7F0081A0))
#define VIDCON0			(*((volatile unsigned long *)0x77100000))
#define VIDCON1			(*((volatile unsigned long *)0x77100004))
#define VIDTCON0		(*((volatile unsigned long *)0x77100010))
#define VIDTCON1		(*((volatile unsigned long *)0x77100014))
#define VIDTCON2		(*((volatile unsigned long *)0x77100018))
#define WINCON0			(*((volatile unsigned long *)0x77100020))
#define VIDOSD0A		(*((volatile unsigned long *)0x77100040))
#define VIDOSD0B		(*((volatile unsigned long *)0x77100044))
#define VIDOSD0C		(*((volatile unsigned long *)0x77100048))
#define VIDW00ADD0B0	(*((volatile unsigned long *)0x771000A0))
#define VIDW00ADD1B0	(*((volatile unsigned long *)0x771000D0))
#define VIDW00ADD2		(*((volatile unsigned long *)0x77100100))
#define DITHMODE		(*((volatile unsigned long *)0x77100170))

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

void lcd_init(void);
void lcd_draw_pixel(int row, int col, int color);
void lcd_clear_screen(int color);
void lcd_draw_hline(int row, int col1, int col2, int color);
void lcd_draw_vline(int col, int row1, int row2, int color);
void lcd_draw_cross(int row, int col, int halflen, int color);
void lcd_draw_circle(void);

#endif
