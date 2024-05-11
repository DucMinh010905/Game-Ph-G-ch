#include "main.h"

int main(int argc, char* argv[])
{
    /*argc là số lượng tham số dòng lệnh
    được truyền vào chương trình,
     argv là một mảng chứa các tham số dòng lệnh đó.
    */
    Game* game = new Game();
    /*tạo ra một đối tượng Game mới trên heap và
     gán địa chỉ của đối tượng này vào con trỏ game.*/

    //Khởi tạo và chạy game
    if(game->Init()) //Thực hiện phương thức Init trên đối tượng game
    /*game là cái vừa khai báo lúc nãy */
    {
        game->Run();
    }
    // Giải phóng bộ nhớ đã được cấp trc đó bằng từ khóa new
    delete game;
    /*Giải phóng bộ nhớ đã được cấp phát cho đối tượng
    game, sử dụng toán tử delete.
    Điều này là cần thiết để tránh rò rỉ
    bộ nhớ khi chương trình kết thúc.
    */
    return 0;
}
