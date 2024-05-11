#include "Game.h"

Game::Game() //Định nghĩa constructor của lớp Game, Game đầu tiên là tên của lớp, game thứ hai lag tên hàm constructor
{
    window = 0;
    renderer = 0;
}

Game::~Game() //Định nghĩa Destructor của lớp Game
{

}

bool Game::Init() //Định nghĩa hàm khởi tạo của lớp Game
{
    SDL_Init(SDL_INIT_VIDEO);
    /*Hàm SDL_Init() là của thư viện SDL, SDL_INIT_VIDEO
    không phải là một biến, mà là một hằng số được
    định nghĩa trong thư viện SDL, SDL_INIT_VIDEO được
    dùng trong lời gọi hàm SDL_Init() để chỉ ra rằng
    bạn muốn khởi tạo hệ thống video của SDL.
    */
    // 3 dòng tiếp theo là tạo của sổ để hiện lên thôi
    window = SDL_CreateWindow("Game Phá Gạch"/*Đây là tên của sổ*/,
    SDL_WINDOWPOS_CENTERED/*Căn giữa ngang*/, SDL_WINDOWPOS_CENTERED/*Căn giữa dọc*/,
    800/*Rộng 800pixels*/, 600/*Cao pixels*/, SDL_WINDOW_SHOWN /*Hằng số làm cửa sổ hiển thị*/|
    SDL_WINDOW_OPENGL/*Hằng số làm của sổ có đồ họa 2D*/);
    if (!window) //Nếu ko mở
    {
        std::cout << "Error creating window:" << SDL_GetError()/*Hàm của thư viện SDL cho ta hiểu lỗi ở đâu*/ << std::endl;
        return false;
    }

    //Tạo renderer bằng cách gọi hàm SDL_CreateRenderer của thư viện SDL
    renderer = SDL_CreateRenderer(window/*Tên của sổ mà renderer sẽ liên kết*/, -1/* cho phép SDL tự chọn index phù hợp cho renderer.*/, SDL_RENDERER_ACCELERATED/*Yêu cấu sử dụng phần cứng gia tốc (nếu có)*/);
    if (!renderer) //Nếu không tạo được Renderer thì lại in ra báo lỗi
    {
        std::cout << "Error creating renderer:" << SDL_GetError() << std::endl;
        return false;
    }


    // Tạo những thứ sẽ hiện trong cửa sổ
    SDL_Surface* surface = IMG_Load("Game Phá Gạch.png"); //IMG_Load AKA Image load là hàm thuộc SDL_Image để lấy hình ảnh từ file cái SDL_Surface dùng để lưu trũ thôi, dùng texture cho nó tối ưu
    texture = SDL_CreateTextureFromSurface(renderer, surface); //chuyển đổi surface thành một texture để có thể hiển thị bằng renderer cho nó tối ưu.
    SDL_FreeSurface(surface); //giải phóng bộ nhớ của surface sau khi đã chuyển đổi thành texture.

    // Khởi tạo thời gian
    lasttick = SDL_GetTicks(); /*Hàm SDL_GetTicks() trả về số
    milliseconds kể từ khi SDL được khởi tạo.
    */
    fpstick = lasttick; //Dùng để tính toán thời gian giữa các khung hình
    fps = 0;       //fps và framecount dùng để tính tần suất khung hình
    framecount = 0;

    testx = 0; //Khởi tạo các biến vị trí
    testy = 0;

    return true;
}

void Game::Clean()
{
    // Dọn tài nguyên sau mỗi lần chơi
    SDL_DestroyTexture(texture);
    // Dọn Renderer và cửa sổ
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void Game::Run() //Định nghĩa hàm chạy của lớp Game
{
    board = new Board(renderer);
    paddle = new Paddle(renderer);
    ball = new Ball(renderer);
    NewGame();
    /*hàm này có thể thiết lập lại các điểm số,
    vị trí của các đối tượng, hoặc các cài đặt
    khác để chuẩn bị cho trò chơi mới.
    */
    while (1) //Bắt đầu vòng lặp vô tận cho đến khi có sự kiện thoát (SDL_QUIT).
    {
        //Xứ lý các sự kiện
        SDL_Event suKien;
        if (SDL_PollEvent(&suKien))
        {
            if (suKien.type == SDL_QUIT)
            {
                break;
            }
        }
        /*
        Kiểm tra và xử lý sự kiện. SDL_PollEvent(&e)
        lấy một sự kiện từ hàng đợi sự kiện. Nếu sự kiện
        là kiểu SDL_QUIT (ví dụ khi nhấn nút thoát cửa sổ),
        vòng lặp sẽ kết thúc
        */

        //Tính delta và fps
        unsigned int curtick = SDL_GetTicks();
        float delta = (curtick - lasttick) / 1000.0f;
        /* Khoảng thời gian trôi qua giữa lần lặp hiện tại
        và lần lặp trước, đơn vị là giây. Được tính bằng cách
        lấy thời gian hiện tại trừ đi thời gian của lần lặp
        trước, sau đó chia cho 1000 để đổi từ millisecond sang
        second.
        */
        if (curtick - fpstick >= FPS_DELAY)
        {
            fps = framecount * (1000.0f / (curtick - fpstick));
            fpstick = curtick;
            framecount = 0;
            char buf[100];
            snprintf(buf, 100, "Game Phá Gạch (fps: %u)", fps);
            SDL_SetWindowTitle(window, buf);
        }
        else
        {
            framecount++;
        }
        lasttick = curtick;

        //Cập nhật và vẽ game(render)
        Update(delta);
        Render(delta);
    }
    delete board;
    delete paddle;
    delete ball;
    Clean();
    SDL_Quit();
}

void Game::NewGame()
{
    board->CreateLevel();
    /* board là một con trỏ trỏ đến đối tượng của
    lớp Board. Có thể định nghĩa trước đó hoặc được
    khởi tạo trong lớp Game.
-> là toán tử truy cập thành viên thông qua con
trỏ trong C++. Toán tử này được sử dụng để truy
cập thành viên (biến hoặc hàm) của một đối tượng
thông qua con trỏ trỏ đến đối tượng đó
*/
    ResetPaddle();
}

void Game::ResetPaddle()
{
    paddlestick = true;
    /* paddlestick được đặt là true,
    biểu thị rằng bóng sẽ dính vào thanh đẩy khi bắt đầu
    trò chơi.
    */
    StickBall();
    /*  hàm StickBall để đặt bóng ở vị trí thích hợp
    trên thanh đẩy.
    */
}

void Game::StickBall()
{
    ball->x = paddle->x + paddle->width/2 - ball->width/2;
    /*ball->x: Đây là thành viên lưu trữ tọa độ x của bóng (ball).
paddle->x: Tọa độ x của thanh đẩy (paddle), là vị trí bắt đầu của thanh đẩy trên trục x.
paddle->width/2: Một nửa chiều rộng của thanh đẩy. Dùng để tìm điểm trung tâm của thanh đẩy.
ball->width/2: Một nửa chiều rộng của bóng. Dùng để căn chỉnh trung tâm của bóng với trung tâm của thanh đẩy.
Phép tính: paddle->x + paddle->width/2 sẽ xác định vị trí x của trung tâm thanh đẩy. Khi trừ ball->width/2, bóng được đặt sao cho trung tâm của nó trùng với trung tâm thanh đẩy.
    */
    ball->y = paddle->y - ball->height;
    /*ball->y: Đây là thành viên lưu trữ tọa độ y của bóng.
paddle->y: Tọa độ y của thanh đẩy, vị trí trên cùng của thanh đẩy.
ball->height: Chiều cao của bóng.
Phép tính: paddle->y - ball->height sẽ đặt bóng ngay phía trên thanh đẩy bằng cách trừ chiều cao của bóng từ tọa độ y của thanh đẩy.
    */
}

void Game::Update(float delta)
{
    int mx, my;
    /* Khai báo các biến để lưu trữ tọa độ chuột. */
    Uint8 mstate = SDL_GetMouseState(&mx, &my);
    /*Khai báo biến mstate để lưu trạng thái của chuột
    vì hàm SDL_GetMouseState() trả về giá trị Unit8 nên ta
    khai báo kiểu dữ liệu Uint8(unsigned int 8bit) */
    /*Lấy tọa độ của con trỏ chuột (mx, my) và trạng thái của các nút chuột (mstate).*/
    SetPaddleX(mx - paddle->width/2.0f);
    /*Đặt vị trí của thanh đẩy bằng cách sử dụng tọa độ x
     của chuột trừ đi một nửa chiều rộng của thanh đẩy để
     căn giữa thanh đẩy.
     */

    if (mstate&SDL_BUTTON(1))
        /*Kiểm tra xem nút chuột trái có được nhấn hay không.
        */
    {
        if (paddlestick)/*Nếu paddlestick = true thì nghĩa là bóng đanh dính vào thanh đảy*/
        {
            paddlestick = false; /* bóng không còn dính vào thanh đẩy nữa*/
            ball->SetDirection(1, -1); /*Thiết lập hướng di chuyển của bóng lên trên và sang phải*/
        }
    }

    if (paddlestick) /*Không nhấn phím nào, bóng dính vào thanh đẩy*/
    {
        StickBall();/*Gọi hàm bóng dính*/
    }

    CheckBoardCollisions(); //Kiểm tra va chạm của bóng với các cạnh của bảng.
    CheckPaddleCollisions(); //Kiểm tra va chạm của bóng với thanh đẩy.
    CheckBrickCollisions2(); // Kiểm tra va chạm của bóng với các viên gạch.

    if (GetBrickCount() == 0) //Nếu hết gạch
    {
        NewGame();//Gọi hàm NewGame() để chơi ván mới
    }

    board->Update(delta);//Cập nhật trạng thái của bảng
    paddle->Update(delta);//Cập nhật trạng thái của thanh đẩy

    if (!paddlestick)//Nếu bóng không dính vào thanh đẩy nữa
    {
        ball->Update(delta);//Cập nhật trạng thái bóng
    }
}

void Game::SetPaddleX(float x) //Hàm này đặt vị trí x của thanh đẩy dựa trên giá trị x được cung cấp.
{
    float newx;
    if (x < board->x) //Kiểm tra xem giá trị x truyền vào có nhỏ hơn tọa độ x của bảng (board) hay không.
    {
        newx = board->x;//Điều này giúp đảm bảo rằng thanh đẩy không vượt quá giới hạn trái của bảng.
    }
    else if (x + paddle->width > board->x + board->width)
    {
        // Khoảng dưới
        newx = board->x + board->width - paddle->width;
    }
    else
    {
        newx = x;
    }
    paddle->x = newx;
}

void Game::CheckBoardCollisions()
{
    //Trên và dưới va chạm
    if (ball->y < board->y)
    {
        //Trên
        //Giữ quả bóng nằm trong cái bảng và phản xạ trục y
        ball->y = board->y;
        ball->diry *= -1;
    }
    else if (ball->y + ball->height > board->y + board->height)
    {
        //Dưới cái bảng

        //Mất bóng, nên reset
        ResetPaddle();
    }

    //Những va chạm trái phải
    if (ball->x <= board->x)
    {
        //Trái
        //Giữ quả bóng trong bảng và phản xạ theo trục x
        ball->x = board->x;
        ball->dirx *= -1;
    }
    else if (ball->x + ball->width >= board->x + board->width)
    {
        //Phải
        //Giữ quả bóng trong bảng và phản xạ theo trục x
        ball->x = board->x + board->width - ball->width;
        ball->dirx *= -1;
    }
}

float Game::GetReflection(float hitx)
{
    //Đảm bảo rằng biến hitx nằm trong chiều rộng của paddle(cái ván)
    if (hitx < 0)
    {
        hitx = 0;
    }
    else if (hitx > paddle->width)
    {
        hitx = paddle->width;
    }
    //Tất cả va chạm nằm ở nửa bên trái paddle sẽ phản xạ qua trái và ngược lại
    hitx -= paddle->width / 2.0f;
    //Tính phản xạ, làm nó trong khoảng -2.0f to 2.0f
    return 2.0f * (hitx / (paddle->width / 2.0f));
}


void Game::CheckPaddleCollisions()
{
    //Tìm tọa độ x tâm của quả bóng
    float ballcenterx = ball->x + ball->width / 2.0f;
    //Kiểm tra va chạm với ván
    if (ball->Collides(paddle))
    {
        ball->y = paddle->y - ball->height;
        ball->SetDirection(GetReflection(ballcenterx - paddle->x), -1);
    }
}

void Game::CheckBrickCollisions()
{
    for (int i=0; i<BOARD_WIDTH; i++)
    {
        for (int j=0; j<BOARD_HEIGHT; j++)
        {
            Brick brick = board->bricks[i][j];
            //Kiểm tra xem viên gạch mất chưa?
            if (brick.state)
            {
                //Tọa độ x và y của viên gạch
                float brickx = board->brickoffsetx + board->x + i*BOARD_BRWIDTH;
                float bricky = board->brickoffsety + board->y + j*BOARD_BRHEIGHT;
                //Kiểm tra va chạm bóng gạch
                //xác định sự va chạm (collision) giữa hai hình chữ nhật bằng cách sử dụng nửa chiều rộng (half-widths) của chúng
                float w = 0.5f * (ball->width + BOARD_BRWIDTH);
                float h = 0.5f * (ball->height + BOARD_BRHEIGHT);
                float dx = (ball->x + 0.5f*ball->width) - (brickx + 0.5f*BOARD_BRWIDTH);
                float dy = (ball->y + 0.5f*ball->height) - (bricky + 0.5f*BOARD_BRHEIGHT);
                if (fabs(dx) <= w && fabs(dy) <= h)
                {
                    //Xác định có va chạm
                    board->bricks[i][j].state = false;
                    float wy = w * dy;
                    float hx = h * dx;
                    if (wy > hx)
                    {
                        if (wy > -hx)
                        {
                            //Dưới cùng (y bị lật ngược)
                            BallBrickResponse(3);
                        }
                        else
                        {
                            //Bên trái
                            BallBrickResponse(0);
                        }
                    }
                    else
                    {
                        if (wy > -hx)
                        {
                            //Bên phải
                            BallBrickResponse(2);
                        }
                        else
                        {
                            //Trên cùng (y bị lật ngược)
                            BallBrickResponse(1);
                        }
                    }
                    return;
                }
            }
        }
    }
}

void Game::CheckBrickCollisions2()
{
    for (int i=0; i<BOARD_WIDTH; i++)
    {
        for (int j=0; j<BOARD_HEIGHT; j++)
        {
            Brick brick = board->bricks[i][j];
            // Kiểm tra nếu viên gạch vẫn còn tồn tại
            if (brick.state)
            {
                // Tọa độ x và y của viên gạch
                float brickx = board->brickoffsetx + board->x + i*BOARD_BRWIDTH;
                float bricky = board->brickoffsety + board->y + j*BOARD_BRHEIGHT;
                // Tọa độ x và y của tâm quả bóng
                float ballcenterx = ball->x + 0.5f*ball->width;
                float ballcentery = ball->y + 0.5f*ball->height;

                // Tọa độ x và y của tâm viên gạch
                float brickcenterx = brickx + 0.5f*BOARD_BRWIDTH;
                float brickcentery = bricky + 0.5f*BOARD_BRHEIGHT;

                if (ball->x <= brickx + BOARD_BRWIDTH && ball->x+ball->width >= brickx && ball->y <= bricky + BOARD_BRHEIGHT && ball->y + ball->height >= bricky)
                {
                    // có va chạm thì xóa viên gạch
                    board->bricks[i][j].state = false;

                    //Giả sử quả bóng di chuyển chậm đủ để không bị vượt qua các viên gạch.

                    // Tính ysize
                    float ymin = 0;
                    if (bricky > ball->y)
                    {
                        ymin = bricky;
                    }
                    else
                    {
                        ymin = ball->y;
                    }

                    float ymax = 0;
                    if (bricky+BOARD_BRHEIGHT < ball->y+ball->height)
                    {
                        ymax = bricky+BOARD_BRHEIGHT;
                    }
                    else
                    {
                        ymax = ball->y+ball->height;
                    }

                    float ysize = ymax - ymin;

                    // Tính xsize
                    float xmin = 0;
                    if (brickx > ball->x)
                    {
                        xmin = brickx;
                    }
                    else
                    {
                        xmin = ball->x;
                    }

                    float xmax = 0;
                    if (brickx+BOARD_BRWIDTH < ball->x+ball->width)
                    {
                        xmax = brickx+BOARD_BRWIDTH;
                    }
                    else
                    {
                        xmax = ball->x+ball->width;
                    }

                    float xsize = xmax - xmin;

                    // The origin is at the top-left corner of the screen!
                    // Gốc tọa độ ở phía trên cùng bên trái của màn hình
                    // Set collision response
                    if (xsize > ysize)
                    {
                        if (ballcentery > brickcentery)
                        {
                            // Dưới cùng
                            ball->y += ysize + 0.01f; // Di chuyển ra khỏi va chạm
                            BallBrickResponse(3);
                        }
                        else
                        {
                            // Trên cùng
                            ball->y -= ysize + 0.01f; // Di chuyển ra khỏi va chạm
                            BallBrickResponse(1);
                        }
                    }
                    else
                    {
                        if (ballcenterx < brickcenterx)
                        {
                            // Bên trái
                            ball->x -= xsize + 0.01f; // Di chuyển ra khỏi va chạm
                            BallBrickResponse(0);
                        }
                        else
                        {
                            // Bên phải
                            ball->x += xsize + 0.01f; // Di chuyển ra khỏi va chạm
                            BallBrickResponse(2);
                        }
                    }

                    return;
                }
            }
        }
    }
}

void Game::BallBrickResponse(int dirindex)
{
    // dirindex 0: Trái, 1: Trên cùng, 2: Phải, 3: Dưới cùng

    // Yếu tố định hướng
    int mulx = 1;
    int muly = 1;

    if (ball->dirx > 0)
    {
        // Bóng đang di chuyển theo chiều dương của trục x
        if (ball->diry > 0)
        {
            // Bóng đang di chuyển theo chiều dương của trục y
            // +1 +1
            if (dirindex == 0 || dirindex == 3)
            {
                mulx = -1;
            }
            else
            {
                muly = -1;
            }
        }
        else if (ball->diry < 0)
        {
            // Bóng đang di chuyển theo chiều âm của trục y
            // +1 -1
            if (dirindex == 0 || dirindex == 1)
            {
                mulx = -1;
            }
            else
            {
                muly = -1;
            }
        }
    }
    else if (ball->dirx < 0)
    {
        // Bóng đang di chuyển theo chiều âm của trục x
        if (ball->diry > 0)
        {
            // Bóng đang di chuyển theo chiều dương của trục y
            // -1 +1
            if (dirindex == 2 || dirindex == 3)
            {
                mulx = -1;
            }
            else
            {
                muly = -1;
            }
        }
        else if (ball->diry < 0)
        {
            // Bóng đang di chuyển theo chiều âm của trục y
            // -1 -1
            if (dirindex == 1 || dirindex == 2)
            {
                mulx = -1;
            }
            else
            {
                muly = -1;
            }
        }
    }

    // Tạo hướng mới cho quả bóng bằng cách nhân nó với hướng cũ
    ball->SetDirection(mulx*ball->dirx, muly*ball->diry);
}

int Game::GetBrickCount()
{
    int brickcount = 0;
    for (int i=0; i<BOARD_WIDTH; i++)
    {
        for (int j=0; j<BOARD_HEIGHT; j++)
        {
            Brick brick = board->bricks[i][j];
            if (brick.state)
            {
                brickcount++;
            }
        }
    }
    return brickcount;
}

void Game::Render(float delta)
{
    SDL_RenderClear(renderer);
    board->Render(delta);
    paddle->Render(delta);
    ball->Render(delta);
    SDL_RenderPresent(renderer);
}
