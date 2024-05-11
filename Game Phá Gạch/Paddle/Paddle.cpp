#include "Paddle.h"

Paddle::Paddle(SDL_Renderer* renderer): Entity(renderer)
{
    SDL_Surface* surface = IMG_Load("Paddle.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    width = 120*1.25;
    height = 16*1.25;

    y = 560;
}

Paddle::~Paddle()
{
    //Dọn tài nguyên
    SDL_DestroyTexture(texture);
}


void Paddle::Update(float delta)
{

}

void Paddle::Render(float delta)
{
    SDL_Rect rect;
    rect.x = (int)(x + 0.5f);
    rect.y = (int)(y + 0.5f);
    rect.w = width;
    rect.h = height;
    SDL_RenderCopy(renderer, texture, 0, &rect);
}
