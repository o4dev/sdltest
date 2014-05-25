#include <SDL2/SDL.h>

#include "defs.h"
#include "screen.h"

#include <stdio.h>
#include <time.h>
#include <math.h>

unsigned cticks;
unsigned framelabel;
BOOL     running;

void eventProc(SDL_Event event) {
    switch (event.type) {
    case SDL_QUIT:
        running = FALSE;
        break;
    }
}

void tick(void) {
    const unsigned char *kbstate = SDL_GetKeyboardState(NULL);
    if (kbstate[SDL_SCANCODE_ESCAPE])
        running = FALSE;
}

#include "bitmaps/boxes"
#include "bitmaps/face"

void render(void) {
    clear(RGBTOINT(
          (int)(sin(cticks / 1000.0             ) * 127 + 128),
          (int)(sin(cticks / 1000.0 + PI / 3 * 2) * 127 + 128),
          (int)(sin(cticks / 1000.0 + PI / 3 * 4) * 127 + 128)
    ));
    drawStr(WIDTH  / 2 - TITLEL * 4 + (int)(cos(cticks / 1000.0) * WIDTH  / 4), 
            HEIGHT / 2 - 6          + (int)(sin(cticks / 1000.0) * HEIGHT / 4),
            TITLE, COL_BRWHITE);
}

void run(void) {
    cticks  = 0;
    framelabel = 0;
    running = TRUE;
    unsigned nowtime = 0;
    unsigned oldtime = 0;
    unsigned frames = 0;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event))
            eventProc(event);
        tick();
        render();
        updateScreen();
        frames++;
        cticks++;
        if ((nowtime = (int)time(NULL)) != oldtime) {
            framelabel = frames;
            printf("%d\n", framelabel);
            frames = 0;
            oldtime = nowtime;
        }
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) | initScreen()) {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }
    clear(COL_BLACK);
    run();
    cleanupScreen();
    SDL_Quit();
    return 0;
}
