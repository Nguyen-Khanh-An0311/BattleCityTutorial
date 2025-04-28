#ifndef BASE_H_INCLUDED
#define BASE_H_INCLUDED

#include <bits/stdc++.h>
#include "SDL.h"
#include "SDL_image.h"
#include "Statistics.h"

class Base{
public:
    int x,y;
    SDL_Texture* baseTexture;
    SDL_Rect rect;
    bool active;
    Base();
    Base(int, int, SDL_Renderer*);

    void render(SDL_Renderer*);
};

#endif // BASE_H_INCLUDED
