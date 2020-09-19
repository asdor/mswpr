#ifndef MSWPR_GAME_HPP
#define MSWPR_GAME_HPP

#include <string_view>

#include "sdl_helper.hpp"
#include "minefield.hpp"
#include "texture_manager.hpp"

namespace minesweeper
{

class game
{
public:
    game(std::string_view title, size_t xpos, size_t ypos, size_t width, size_t height);

    void handle_input();

    void update();

    void render();

    constexpr bool running() const noexcept
    {
        return is_running_;
    }

private:
    minesweeper::sdl_init_t sdl_init_;
    minesweeper::sdl_window_t window_;
    minesweeper::sdl_renderer_t renderer_;

    bool is_running_;
    minesweeper::texture_manager texture_manager_;
    minesweeper::minefield minefield_;
};

} // namespace minesweeper

#endif // MSWPR_GAME_HPP
