#include "SDL.h"

#include <stdio.h>
#include <time.h>
#include <math.h>

#include "defs.h"
#include "screen.h"

uint    cticks;
uint    framelabel;
bool    running;

void eventProc(SDL_Event event) {
    switch (event.type) {
    case SDL_QUIT:
        running = false;
        break;
    }
}

void tick(void) {
    const uchar *kbstate = SDL_GetKeyboardState(NULL);
    if (kbstate[SDL_SCANCODE_ESCAPE])
        running = false;
}

#include "bitmaps/face"

void render(void) {
    clear(RGBTOINT(
          (int)(sin(cticks / 1000.0             ) * 127 + 128),
          (int)(sin(cticks / 1000.0 + PI / 3 * 2) * 127 + 128),
          (int)(sin(cticks / 1000.0 + PI / 3 * 4) * 127 + 128)
    ));
    int w = 4 * FACE_WIDTH  + (int)(cos(cticks / 100.0) * FACE_WIDTH  * 2);
    int h = 4 * FACE_HEIGHT + (int)(sin(cticks / 100.0) * FACE_HEIGHT * 2);
    int x = (WIDTH  - w) >> 1;
    int y = HEIGHT / 16 * 7 - h / 2;
    drawImg(x, y, w, h, FACE_WIDTH, FACE_HEIGHT, FACE);
    drawStr(WIDTH  / 2 - TITLEL * 4 - (int)(sin(cticks / 1000.0) * WIDTH / 4),
            HEIGHT / 4 * 3 - 4, TITLE, COL_BRWHITE);
}

void run(void) {
    cticks  = 0;
    framelabel = 0;
    running = true;
    uint nowtime = 0;
    uint oldtime = (uint)time(NULL);
    uint frames = 0;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event))
            eventProc(event);
        tick();
        render();
        updateScreen();
        frames++;
        cticks++;
        if ((nowtime = (uint)time(NULL)) != oldtime) {
            framelabel = frames;
            printf("%dfps\n", framelabel);
            frames = 0;
            oldtime = nowtime;
        }
    }
}

int start() {
    printf("Loading, please wait...\n");
    if (sizeof(int) != 4) {
        printf("Fatal error: 'int' MUST be 4 bytes wide only");
        return 1;
    }
    bool sdlsuccess = (SDL_Init(SDL_INIT_EVERYTHING) == 0);
    bool screensuccess = (sdlsuccess ? initScreen() == 0 : false);
    if (!sdlsuccess || !screensuccess) {
        printf("A critical error occured:\n%s\n", SDL_GetError());
        return 1;
    }
    printf("Continuing...\n");
    clear(COL_BLACK);    
    return 0;
}

void stop() {
    printf("Closing...\n");
    cleanupScreen();
    SDL_Quit();
}

#undef int
#undef char

int main(int argc, char* argv[]) {
    if (start() != 0) {
        stop();
        return 1;
    }
    run();
    stop();
    return 0;
}
