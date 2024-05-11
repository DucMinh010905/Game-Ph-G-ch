#ifndef GAME_H_  /*Include guard cho game.h*/
#define GAME_H_
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <stdio.h> //Nhập xuất chuẩn trong c++
#include "Board.h"
#include "Paddle.h"
#include "Ball.h"

#define FPS_DELAY 500 //500ms

class Game
{
public:
    Game(); //Contructor của class Game
    ~Game(); //Destructor của class Game

    bool Init();
    /* Hàm Init() dùng để khởi tạo các tài nguyên
     và thiết lập cần thiết cho trò chơi, như tạo
      cửa sổ, khởi tạo đồ họa, và chuẩn bị dữ liệu trò chơi.
    */
    void Run(); //Hàm Init() trả về true thì hàm này chạy

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    // Thời gian
    unsigned int lasttick/*thời điểm cuối cùng*/, fpstick/*lượng thời gian giữa các frames*/, fps/*frames per second*/, framecount;
    // Kiểm tra
    float testx, testy;
    Board* board;
    Paddle* paddle;
    Ball* ball;
    bool paddlestick;
    void Clean(); //Hàm dọn bộ nhớ
    void Update(float delta); //Hàm cập nhật
    void Render(float delta); //Hàm vẽ
    //Các Hàm
    void NewGame();
    void ResetPaddle();
    void StickBall();
    void SetPaddleX(float x);
    void CheckBoardCollisions();
    float GetReflection(float hitx);
    void CheckPaddleCollisions();
    void CheckBrickCollisions();
    void CheckBrickCollisions2();
    void BallBrickResponse(int dirindex);
    int GetBrickCount();
};

#endif
