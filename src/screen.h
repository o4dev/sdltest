#ifndef _SCREEN_H_
#define _SCREEN_H_

int  initScreen(void);
void cleanupScreen(void);
void updateScreen(void);

void clear(int c);
void setPixel(int x, int y, int c) __inline;
int  getPixel(int x, int y);

void drawRect(int x, int y, unsigned w, unsigned h, unsigned t, int c);
void fillRect(int x, int y, unsigned w, unsigned h, int c);

void drawStr(int x, int y, const char* str, int c);

#endif /* _SCREEN_H_ */

