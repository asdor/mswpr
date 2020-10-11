#include "game_engine.hpp"

int main()
{
    static constexpr int screen_width = 800;
    static constexpr int screen_height = 640;

    mswpr::game_engine engine("mswpr", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height);

    while (engine.running())
    {
        engine.handle_input();
        engine.update();
        engine.render();
    }

    return 0;
}
