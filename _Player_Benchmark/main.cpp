#include "BenchmarkPlayer.h"


int main(int argc, char* argv[]) {
    const Vector2i resolution(800*2, 600*1.5);

    BenchmarkPlayer player(resolution);
    player.Run();

    return 0;
}
