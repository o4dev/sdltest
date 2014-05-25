#ifndef _SCREEN_H_
#define _SCREEN_H_

#define COL_BLACK       0x000000
#define COL_RED         0x7f0000
#define COL_GREEN       0x007f00
#define COL_YELLOW      0x7f7f00
#define COL_BLUE        0x00007f
#define COL_MAGENTA     0x7f007f
#define COL_CYAN        0x007f7f
#define COL_WHITE       0xcfcfcf
#define COL_BRBLACK     0x7f7f7f
#define COL_BRRED       0xff0000
#define COL_BRGREEN     0x00ff00
#define COL_BRYELLOW    0xffff00
#define COL_BRBLUE      0x0000ff
#define COL_BRMAGENTA   0xff00ff
#define COL_BRCYAN      0x00ffff
#define COL_BRWHITE     0xffffff
#define COL_TRANS       COL_BRMAGENTA
#define COL_GREY        COL_BRBLACK

int  initScreen(void);
void cleanupScreen(void);
void updateScreen(void);

void clear(int c);
void setPixel(int x, int y, int c) __inline;
int  getPixel(int x, int y);

void drawRect(int x, int y, unsigned w, unsigned h, unsigned t, int c);
void fillRect(int x, int y, unsigned w, unsigned h, int c);

void drawStr(int x, int y, const char* str, int c);

void drawImgUnscaled(int x, int y, unsigned iw, unsigned ih, int *ipix);
void drawImg(int x, int y, unsigned dw, unsigned dh,
             unsigned iw, unsigned ih, int *ipix);

#endif /* _SCREEN_H_ */

