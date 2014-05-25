#include <SDL2/SDL.h>

#include "defs.h"
#include "screen.h"

#include <stdlib.h>
#include <math.h>

SDL_Window   *wnd;
SDL_Renderer *renderer;
SDL_Texture  *tex;

void         *pixels;

const unsigned char FONT[12 * 95] __align(8) = {
#include "font"
};

#define HTABSIZE    4
#define VTABSIZE    4

int initScreen(void) {
    wnd = SDL_CreateWindow(
        "sdltest",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH, HEIGHT, NONE
    );
    if (!wnd) return 1;
    renderer = SDL_CreateRenderer(
        wnd, -1, SDL_RENDERER_SOFTWARE
    );
    if (!renderer) return 1;
    tex = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );
    if (!tex) return 1;
    pixels = malloc(WIDTH * HEIGHT * 4);
    return 0;
}

void cleanupScreen(void) {
    free(pixels);
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(wnd);
}

void updateScreen(void) {
    SDL_UpdateTexture(tex, NULL, pixels, WIDTH * 4);
    SDL_RenderCopy(renderer, tex, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void clear(int c) {
    for (register unsigned i = WIDTH * HEIGHT; i--;)
        ((int*)pixels)[i] = c;
}

void setPixel(int x, int y, int c) {
    *((int*)pixels + x + y * WIDTH) = c;
}

int getPixel(int x, int y) {
    return *((int*)pixels + x + y * WIDTH);
}

void drawRect(int x, int y, unsigned w, unsigned h, unsigned t, int c) {
    for (register unsigned xx = w; xx--;)
        for (register unsigned yy = h; yy--;) {
            if (xx - t < w - t * 2 && yy - t < h - t * 2)
                continue;
            setPixel(x + xx, y + yy, c);
        }
}

void fillRect(int x, int y, unsigned w, unsigned h, int c) {
    for (register unsigned xx = w; xx--;)
        for (register unsigned yy = h; yy--;)
            setPixel(x + xx, y + yy, c);
}

void _drawFontGlyph(int x, int y, unsigned char i, int c) {
    for (int yy = 0; yy != 12; yy++) {
        unsigned int glyphrow = FONT[i * 12 + yy];
        for (int xx = 0; xx != 8; xx++)
            if ((glyphrow >> xx) & 1)
                setPixel(x + xx, y + yy, c);
    }
}

void drawStr(int x, int y, const char* str, int c) {
    for (int i = 0, xx = 0, yy = 0; ; i++) {
        switch (str[i]) {
        case '\0': return;
        case '\n': yy++;
        case '\r': xx = 0;
                   break;
        case '\t': xx += HTABSIZE - (xx % HTABSIZE);
                   break;
        case '\v': yy += VTABSIZE - (yy % VTABSIZE);
                   break;
        case '\b': xx--;
                   break;
        default:   _drawFontGlyph(x + xx++ * 8,
                                  y + yy * 12,
                                  str[i] - 32, c);
                   break;
        }
    }
}

void drawImgUnscaled(int x, int y, unsigned iw, unsigned ih, int *ipix) {
    for (register unsigned xx = iw; xx--; )
        for (register unsigned yy = ih; yy--; ) {
            register int c = ipix[xx + yy * iw];
            if (c != COL_TRANS)
                setPixel(x + xx, y + yy, c);
        }
}

void drawImg(int x, int y, unsigned dw, unsigned dh,
             unsigned iw, unsigned ih, int *ipix) {
    double sx = (double)dw / iw;
    double sy = (double)dh / ih;
    for (register unsigned xx = dw; xx--; )
        for (register unsigned yy = dh; yy--; ) {
            register int c = ipix[(int)floor(xx / sx) + 
                                  (int)floor(yy / sy) * iw];
            if (c != COL_TRANS)
                setPixel(x + xx, y + yy, c);
        }
}
