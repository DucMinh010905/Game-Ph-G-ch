#ifndef ENTITY_H_
#define ENTITY_H_

#include "SDL.h"
#include "SDL_image.h"

class Entity
{
public:

    Entity(SDL_Renderer* renderer);//Constructor của lớp Entity
    virtual ~Entity();
    float x, y, width, height;//tọa đô (x,y) và chiều rộng, chiều cao
    virtual void Update(float delta);
    virtual void Render(float delta);
    bool Collides(Entity* other);

protected:
    SDL_Renderer* renderer;
};

#endif
