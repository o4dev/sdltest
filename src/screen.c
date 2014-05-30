#include <SDL2/SDL.h>

#include <stdlib.h>
#include <math.h>

#include "defs.h"
#include "screen.h"

SDL_Window   *wnd;
SDL_Renderer *renderer;
SDL_Texture  *tex;

int          *pixels;

#define HTABSIZE    4
#define VTABSIZE    4

const uchar FONT[12 * 95] = {
#include "font"
};

void _getAndDetectDeviceSet(void) {
    int devcount = SDL_GetNumRenderDrivers();
    printf("Found %d Render Devices\n", devcount);
    for (int i = 0; i < devcount; i++) {
        SDL_RendererInfo devinfo;
        SDL_GetRenderDriverInfo(i, &devinfo);
        bool devsoftware    = devinfo.flags & SDL_RENDERER_SOFTWARE;
        bool devaccelerated = devinfo.flags & SDL_RENDERER_ACCELERATED;
        bool devvsync       = devinfo.flags & SDL_RENDERER_PRESENTVSYNC;
        bool devtargettex   = devinfo.flags & SDL_RENDERER_TARGETTEXTURE;
        bool devargb32      = false;
        for (int j = devinfo.num_texture_formats; j--; )
            devargb32 |= (devinfo.texture_formats[j]
                          & SDL_PIXELFORMAT_ARGB8888);
        #define b(q) (q ? "true" : "false")
        printf("--- Device %d Details ---\n", i);
        printf(" Name:                 %s\n", devinfo.name);
        printf(" Software Device:      %s\n", b(devsoftware));
        printf(" Hardware Accelerated: %s\n", b(devaccelerated));
        printf(" Vertical Sync:        %s\n", b(devvsync));
        printf(" Supports ARGB 32-bit: %s\n", b(devargb32));
        #undef b
    }
}

int initScreen(void) {
    _getAndDetectDeviceSet();
    wnd = SDL_CreateWindow(
        TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH * SCALE, HEIGHT * SCALE, 
        0
    );
    int devid = 1;
    printf("Attempting to use device %d\n", devid);
    renderer = SDL_CreateRenderer(
        wnd, devid, 0
    );
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
    if (pixels)   free(pixels);
    if (tex)      SDL_DestroyTexture(tex);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (wnd)      SDL_DestroyWindow(wnd);
}

void updateScreen(void) {
    SDL_UpdateTexture(tex, NULL, pixels, WIDTH * 4);
    SDL_RenderCopy(renderer, tex, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void clear(int c) {
    for (register uint i = WIDTH * HEIGHT; i--;)
        pixels[i] = c;
}

void setPixel(int x, int y, int c) {
    *(pixels + x + y * WIDTH) = c;
}

int getPixel(int x, int y) {
    return *(pixels + x + y * WIDTH);
}

void drawRect(int x, int y, uint w, uint h, uint t, int c) {
    for (register uint xx = w; xx--;)
        for (register uint yy = h; yy--;) {
            if (xx - t < w - t * 2 && yy - t < h - t * 2)
                continue;
            setPixel(x + xx, y + yy, c);
        }
}

void fillRect(int x, int y, uint w, uint h, int c) {
    for (register uint xx = w; xx--;)
        for (register uint yy = h; yy--;)
            setPixel(x + xx, y + yy, c);
}

void _drawFontGlyph(int x, int y, uchar i, int c) {
    for (register uint yy = 12; yy--; ) {
        uint glyphrow = FONT[i * 12 + yy];
        for (register uint xx = 8; xx--; )
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

void drawImgUnscaled(int x, int y, uint iw, uint ih, const int *ipix) {
    for (register uint xx = iw; xx--; )
        for (register uint yy = ih; yy--; ) {
            register int c = ipix[xx + yy * iw];
            if (c != COL_TRANS)
                setPixel(x + xx, y + yy, c);
        }
}

void drawImg(int x, int y, uint dw, uint dh, uint iw, uint ih,
             const int *ipix) {
    double sx = (double)dw / iw;
    double sy = (double)dh / ih;
    for (register uint xx = dw; xx-- != 1; )
        for (register uint yy = dh; yy-- != 1; ) {
            register int c = ipix[(int)floor(xx / sx) + 
                                  (int)floor(yy / sy) * iw];
            if (c != COL_TRANS)
                setPixel(x + xx, y + yy, c);
        }
}
