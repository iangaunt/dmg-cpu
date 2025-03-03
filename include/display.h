#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL2/SDL.h>

class display {
    public:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        SDL_Event event;

        unsigned int width;
        unsigned int height;
        unsigned int size_modifier;

        display(unsigned int width, unsigned int height, unsigned int size_modifier, const char* title);

        bool fetch_input();
        void render();
        void end();
        
};

#endif