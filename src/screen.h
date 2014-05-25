#ifndef _SCREEN_H_
#define _SCREEN_H_

#define COL_BLACK   0x000000
#define COL_WHITE   0xffffff
#define COL_RED     0xff0000
#define COL_GREEN   0x00ff00
#define COL_BLUE    0x0000ff
#define COL_MAGENTA 0xff00ff
#define COL_YELLOW  0xffff00
#define COL_CYAN    0x00ffff
#define COL_TRANS   COL_MAGENTA

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

