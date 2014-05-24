#include <SDL2/SDL.h>

#include "defs.h"
#include "screen.h"

#include <stdio.h>
#include <time.h>
#include <math.h>

unsigned  cticks;
unsigned  framelabel;
BOOL      running;

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

#include "face"

void render(void) {
    clear(0x7f7f);
    int w = FACE_WIDTH  * 20 + (int)(sin(cticks / 100.0     ) * 100);
    int h = FACE_HEIGHT * 20 + (int)(cos(cticks / 100.0 + PI) * 100);
    int x = (WIDTH  - w) / 2;
    int y = (HEIGHT - h) / 2;
    drawImg(x, y, w, h, FACE_WIDTH, FACE_HEIGHT, FACE);
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
    BOOL error = (SDL_Init(SDL_INIT_EVERYTHING) != 0);
    if (!error)
        error |= (initScreen() != 0);
    if (error) {
        printf("Error: %s\n", SDL_GetError());
        return 1;
    }
    run();
    cleanupScreen();
    SDL_Quit();
    return 0;
}

