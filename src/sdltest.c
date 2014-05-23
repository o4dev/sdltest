#include <SDL2/SDL.h>

#include "defs.h"
#include "screen.h"

#include <stdio.h>
#include <time.h>

unsigned cticks;
unsigned framelabel;
BOOL     running;

void run(void);
void render(void);
void eventProc(SDL_Event*) __inline;

void eventProc(SDL_Event* event) {
    switch (event->type) {
    case SDL_QUIT:
        running = FALSE;
        break;
    case SDL_KEYDOWN:
        switch (event->key.keysym.sym) {
        case SDLK_ESCAPE:
            running = FALSE;
            break;
        }
        break;
    }
}

void render(void) {
    clear(0);
    fillRect(10, 10, WIDTH - 20, HEIGHT - 20, 0xffff00);
    drawRect(10, 10, WIDTH - 20, HEIGHT - 20, 1, 0xffff);
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
            eventProc(&event);
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

