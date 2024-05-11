#ifndef PADDLE_H_
#define PADDLE_H_

#include "Entity.h"

class Paddle: public Entity
/* các thành viên public của lớp cha Entity
vẫn có thể được truy cập trực tiếp từ Paddle.
*/
{
public:
    Paddle(SDL_Renderer* renderer);
    ~Paddle();
    void Update(float delta);
    void Render(float delta);

private:
    SDL_Texture* texture;
};

#endif
