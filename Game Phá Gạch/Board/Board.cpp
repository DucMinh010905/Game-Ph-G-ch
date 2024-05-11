#include "Board.h"

Board::Board(SDL_Renderer* renderer): Entity(renderer)
{
    //6 dòng tiếp theo tương tự như đã giải thích trong ball.cpp
    SDL_Surface* surface = IMG_Load("bricks.png");
    bricktexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = IMG_Load("side2.png");
    sidetexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    srand(time(0));
    x = 16;
    /*
    bảng sẽ bắt đầu vẽ từ một vị trí nào đó bên phải
    của màn hình, cách điểm góc trên bên trái 16 pixel.
    */
    y = 0;
    width = 768;   //kích thước bảng
    height = 600;
    brickoffsetx = 0;
    brickoffsety = 16;
}

Board::~Board()
{
    // Dọn dẹp tài nguyên
    SDL_DestroyTexture(bricktexture);
    SDL_DestroyTexture(sidetexture);
}

void Board::Update(float delta)
{

}

void Board::Render(float delta)
{
    // Vẽ những viên gạch
    for (int i=0; i<BOARD_WIDTH; i++)
    {
        for (int j=0; j<BOARD_HEIGHT; j++)
        {
            Brick brick = bricks[i][j];
            // Kiểm tra xem viên gạch có tồn tại không
            if (!brick.state)
                continue;
            SDL_Rect srcrect;
            srcrect.x = (brick.type % 2) * BOARD_BRWIDTH;
            srcrect.y = (brick.type / 2) * BOARD_BRHEIGHT;
            srcrect.w = BOARD_BRWIDTH;
            srcrect.h = BOARD_BRHEIGHT;
            SDL_Rect dstrect;
            dstrect.x = brickoffsetx + x + i * BOARD_BRWIDTH;
            dstrect.y = brickoffsety + y + j * BOARD_BRHEIGHT;
            dstrect.w = BOARD_BRWIDTH;
            dstrect.h = BOARD_BRHEIGHT;
            SDL_RenderCopy(renderer, bricktexture, &srcrect, &dstrect);
        }
    }

    // Vẽ tường 2 bên
    SDL_Rect dstrect;
    dstrect.x = 0;
    dstrect.y = 0;
    dstrect.w = 18/*20*/;
    dstrect.h = 600/*700*/;
    SDL_RenderCopy(renderer, sidetexture, 0, &dstrect);
    dstrect.x = 800 - 18; //Khoảng cách tường trái phải
    dstrect.y = 0;
    dstrect.w = 18;
    dstrect.h = 600;
    SDL_RenderCopy(renderer, sidetexture, 0, &dstrect);
}

void Board::CreateLevel()
{
    for (int i=0; i<BOARD_WIDTH; i++)
    {
        for (int j=0; j<BOARD_HEIGHT; j++)
        {
            Brick brick;
            brick.type = rand() % 4;
            brick.state = true;
            bricks[i][j] = brick;
        }
    }
}
