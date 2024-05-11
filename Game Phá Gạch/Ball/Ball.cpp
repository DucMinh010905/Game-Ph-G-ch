#include "Ball.h"

Ball::Ball(SDL_Renderer* renderer): Entity(renderer)
/*Đây là constructor của lớp Ball, được
gọi khi một đối tượng Ball được tạo ra. Nó kế thừa
từ lớp Entity và nhận một tham số là con trỏ tới renderer
của SDL để vẽ quả bóng lên màn hình.
*/
{
    SDL_Surface* surface = IMG_Load("Ball.png");//IMG_Load AKA Image load là hàm thuộc SDL_Image để lấy hình ảnh từ file cái SDL_Surface dùng để lưu trũ thôi, dùng texture cho nó tối ưu
    texture = SDL_CreateTextureFromSurface(renderer, surface);//chuyển đổi surface thành một texture để có thể hiển thị bằng renderer cho nó tối ưu.
    SDL_FreeSurface(surface);//giải phóng bộ nhớ của surface sau khi đã chuyển đổi thành texture.
    x = 0; //Tọa độ x của quả bóng
    y = 0; //Tọa độ y của quả bóng
    width = 31*0.85; //Chiều rộng quả bóng = 24pixels
    height = 30*0.85; //Chiều cao quả bóng = 24pixels
    SetDirection(1, 1);
}

Ball::~Ball() //Hàm hủy khi bóng bị hủy, rơi trượt paddle, ...
{
    SDL_DestroyTexture(texture);//Hàm này của SDL
}

void Ball::Update(float delta)
{
    x += dirx * delta;
    /*x là tọa độ x hiện tại của quả bóng, dirx
    là hướng di chuyển của quả bóng theo trục x, và
    delta là thời gian đã trôi qua
    */
    y += diry * delta;//tương tự dòng trên
}

void Ball::Render(float delta)
{
    SDL_Rect rect; //Đây là cấu trúc trong SDL để định nghĩa 1 hình chữ nhật
    rect.x = (int)(x + 0.5f); //Làm tròn tọa độ vì SDL yêu cầu tọa độ nguyên thì mới vẽ được
    rect.y = (int)(y + 0.5f);
    rect.w = width;
    rect.h = height;
    SDL_RenderCopy(renderer, texture, 0/*tức là truyển toàn bộ texture lên renderer*/, &rect);
    /*quả bóng sẽ được vẽ tại tọa độ (rect.x, rect.y)
    với kích thước (rect.w, rect.h)., dựa vào SDL_RenderCopy
    của SDL */
}

void Ball::SetDirection(float dirx, float diry)
{
    float length = sqrt(dirx * dirx + diry * diry);
    //Chiều dài đường đi giữa 2 tọa độ
    this->dirx = BallSpeed * (dirx / length);
    /*yêu cầu compiler truy cập vào thành viên dữ liệu
    dirx của đối tượng hiện tại mà phương thức đang thực
    thi, this ở đây thay cho đối tượng ball hiện tại.
    */
    this->diry = BallSpeed * (diry/ length);
    /*tương tự*/
}
