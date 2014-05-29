#include <SDL2/SDL.h>

#include <stdlib.h>
#include <math.h>

#include "defs.h"
#include "screen.h"

SDL_Window   *wnd;
SDL_Renderer *renderer;
SDL_Texture  *tex;

int          *pixels;

const unsigned char FONT[12 * 95] __align(8) = {
#include "font"
};

#define HTABSIZE    4
#define VTABSIZE    4

void _detectDrivers(void) {
    int devcount = SDL_GetNumRenderDrivers();
    printf("Found %d Render Devices\n", devcount);
    for (int i = 0; i < devcount; i++) {
        SDL_RendererInfo devinfo;
        SDL_GetRenderDriverInfo(i, &devinfo);
        BOOL devsoftware    = devinfo.flags & SDL_RENDERER_SOFTWARE;
        BOOL devaccelerated = devinfo.flags & SDL_RENDERER_ACCELERATED;
        BOOL devvsync       = devinfo.flags & SDL_RENDERER_PRESENTVSYNC;
        BOOL devtargettex   = devinfo.flags & SDL_RENDERER_TARGETTEXTURE;
        #define b(q) (q ? "true" : "false")
        printf("--- Device %d Details ---\n", i);
        printf(" Name:                 %s\n", devinfo.name);
        printf(" Software Device:      %s\n", b(devsoftware));
        printf(" Hardware Accelerated: %s\n", b(devaccelerated));
        printf(" Vertical Sync:        %s\n", b(devvsync));
        printf(" Can Target Texture:   %s\n", b(devsoftware));
        #undef b
    }
}

int initScreen(void) {
    _detectDrivers();
    wnd = SDL_CreateWindow(
        TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH * SCALE, HEIGHT * SCALE, 
        0
    );
    SDL_ClearError();
    renderer = SDL_CreateRenderer(
        wnd, -1, SDL_RENDERER_ACCELERATED
    );
    if (strlen(SDL_GetError())) { // fuck
        SDL_DestroyRenderer(renderer);
        renderer = SDL_CreateRenderer(
            wnd, -1, SDL_RENDERER_SOFTWARE
        );
        SDL_ClearError();
        printf("Couldn't create an accelerated renderer, "
               "falling back to software rendering.\n");
    }
    tex = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH, HEIGHT
    );
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
        pixels[i] = c;
}

void setPixel(int x, int y, int c) {
    *(pixels + x + y * WIDTH) = c;
}

int getPixel(int x, int y) {
    return *(pixels + x + y * WIDTH);
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
    for (register unsigned yy = 12; yy--; ) {
        unsigned int glyphrow = FONT[i * 12 + yy];
        for (register unsigned xx = 8; xx--; )
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
        default:   _drawFontGlyph(x + (xx++ << 3),
                                  y + yy * 12,
                                  str[i] - 32, c);
                   break;
        }
    }
}

void drawImgUnscaled(int x, int y, unsigned iw, unsigned ih,
                     const int *ipix) {
    for (register unsigned xx = iw; xx--; )
        for (register unsigned yy = ih; yy--; ) {
            register int c = ipix[xx + yy * iw];
            if (c != COL_TRANS)
                setPixel(x + xx, y + yy, c);
        }
}

void drawImg(int x, int y, unsigned dw, unsigned dh,
             unsigned iw, unsigned ih, const int *ipix) {
    double sx = (double)dw / iw;
    double sy = (double)dh / ih;
    for (register unsigned xx = dw; xx-- != 1; )
        for (register unsigned yy = dh; yy-- != 1; ) {
            register int c = ipix[(int)floor(xx / sx) + 
                                  (int)floor(yy / sy) * iw];
            if (c != COL_TRANS)
                setPixel(x + xx, y + yy, c);
        }
}
