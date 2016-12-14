#include <stdio.h>
#include <stdlib.h>

#define HEADERLENGTH 14   // The bmp FileHeader length is 14
#define BM 0x4d42         // The ASCII code for BM
unsigned int biWidth;
unsigned int biHeight;
unsigned int biBitCount;
unsigned int bfType;
// offsets of color information
unsigned int bfOffBits;
unsigned char* pix;

void lcd_show_picture(FILE *bmp);

void lcd_show_picture()
{
  FILE *bmp = fopen("1.bmp", "r+");
  fread(&bfType, sizeof(char), 2, bmp);
  if (bfType != BM) {
    printf("%s\n", "Wrong format!");
    return 0;
  }
  fseek(bmp,18L,0);
  fread(&biWidth, sizeof(char), 4, bmp);
  fread(&biHeight, sizeof(char), 4, bmp);
  fseek(bmp,2L,1);
  fread(&biBitCount, sizeof(char), 2, bmp);
  int i,j;
  unsigned char red,green,blue,alpha;
  int desrow = ROW;
  int descol = COL;
  int srcPicture[biHeight][biWidth];
  int dstPicture[ROW][COL];
  int stride;
  fseek(bmp,10L,0);
  fread(&bfOffBits, sizeof(char), 4, bmp);
  fseek(bmp, bfOffBits, 0);
  stride=(24*biWidth+31)/8;
  stride=stride/4*4;
  pix=(unsigned char *)malloc(stride);
  for(j=0; j<biHeight; j++)
  {
     fread(pix, 1, stride, bmp);
     for(i=0; i<biWidth; i++)
      {
          red=pix[i*3+2];
          green=pix[i*3+1];
          blue=pix[i*3];
          color |= (1 << 24);
          color |= ((int)red   << 16);
          color |= ((int)green << 8 );
          color |= ((int)blue       );
          lcd_draw_pixel(height-1-j,i,color);
      }
  }
}
