#include "lcd.h"

// 初始化LCD
void lcd_init(void)
{
	// 配置GPIO用于LCD相关的功能
	GPECON = 0x00011111;
	GPEDAT = 0x00000001;
	GPFCON = 0x96AAAAAA;
	GPFDAT = 0x00002000;
	GPICON = 0xAAAAAAAA;
	GPJCON = 0x00AAAAAA;

	// normal mode
	MIFPCON &= ~(1<<3);

	// RGB I/F
	SPCON =  (SPCON & ~(0x3)) | 1;

	// 配置VIDCONx，设置接口类型、时钟、极性和使能LCD控制器等
	VIDCON0 = (0<<26)|(0<<17)|(0<<16)|(10<<6)|(0<<5)|(1<<4)|(0<<2)|(3<<0);
	VIDCON1 |= 1<<5 | 1<<6;

	// 配置VIDTCONx，设置时序和长宽等
	// 设置时序
	VIDTCON0 = VBPD<<16 | VFPD<<8 | VSPW<<0;
	VIDTCON1 = HBPD<<16 | HFPD<<8 | HSPW<<0;
	// 设置长宽
	VIDTCON2 = (LINEVAL << 11) | (HOZVAL << 0);


	// 配置WINCON0，设置window0的数据格式
	WINCON0 |= 1<<0;
	WINCON0 &= ~(0xf << 2);
	WINCON0 |= 0xB<<2;

	// 配置VIDOSD0A/B/C,设置window0的坐标系
	VIDOSD0A = (LeftTopX<<11) | (LeftTopY << 0);
	VIDOSD0B = (RightBotX<<11) | (RightBotY << 0);
	VIDOSD0C = (LINEVAL + 1) * (HOZVAL + 1);

	// 置VIDW00ADD0B0和VIDW00ADD1B0，设置framebuffer的地址
	VIDW00ADD0B0 = FRAME_BUFFER;
	VIDW00ADD1B0 = (((HOZVAL + 1)*4 + 0) * (LINEVAL + 1)) & (0xffffff);
}

// 描点
void lcd_draw_pixel(int row, int col, int color)
{
	unsigned long * pixel = (unsigned long  *)FRAME_BUFFER;

	*(pixel + row * COL + col) = color;

}

// 清屏
void lcd_clear_screen(int color)
{
	int i, j;

	for (i = 0; i < ROW; i++)
		for (j = 0; j < COL; j++)
			lcd_draw_pixel(i, j, color);

}

void lcd_clear_screen_lr(int color)
{
	int i, j;
	for (j = 0; j < COL; j++)
		for (i = 0; i < ROW; i++)
			lcd_draw_pixel(i, j, color);

}

void lcd_clear_screen_rl(int color)
{
	int i, j;
	for (j = COL - 1; j >= 0; j--)
		for (i = 0; i < ROW; i++)
			lcd_draw_pixel(i, j, color);

}

void lcd_clear_screen_bu(int color)
{
	int i, j;
	for (i = ROW - 1; i >= 0; i--)
		for (j = 0; j < COL; j++)
			lcd_draw_pixel(i, j, color);

}

// 划横线
void lcd_draw_hline(int row, int col1, int col2, int color)
{
	int j;

	// 描第row行，第j列
	for (j = col1; j <= col2; j++)
		lcd_draw_pixel(row, j, color);

}

// 划竖线
void lcd_draw_vline(int col, int row1, int row2, int color)
{
	int i;
	// 描第i行，第col列
	for (i = row1; i <= row2; i++)
		lcd_draw_pixel(i, col, color);

}

// 划十字
void lcd_draw_cross(int row, int col, int halflen, int color)
{
	lcd_draw_hline(row, col-halflen, col+halflen, color);
	lcd_draw_vline(col, row-halflen, row+halflen, color);
}

// 绘制同心圆
void lcd_draw_circle(void)
{
	int x,y;
	int color;
	unsigned char red,green,blue,alpha;
	int xsize = ROW;
	int ysize = COL;

	for (y = 0; y < ysize; y++)
		for (x = 0; x < xsize; x++)
		{
			color = ((x-xsize/2)*(x-xsize/2) + (y-ysize/2)*(y-ysize/2))/64;
			red   = (color/8) % 256;
			green = (color/4) % 256;
			blue  = (color/2) % 256;
			alpha = (color*2) % 256;

			color |= ((int)alpha << 24);
			color |= ((int)red   << 16);
			color |= ((int)green << 8 );
			color |= ((int)blue       );

			lcd_draw_pixel(x,y,color);
		}
}
