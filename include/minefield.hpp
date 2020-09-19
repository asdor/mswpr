#ifndef MSWPR_MINEFIELD_HPP
#define MSWPR_MINEFIELD_HPP

#include <vector>
#include <type_traits>

namespace minesweeper
{
class texture_manager;

enum class cell_state
{
    EMPTY = 0,

    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,

    BOMB
};

struct cell
{
    cell_state state;
    bool is_pressed;
    bool is_flagged;

};
static_assert(std::is_trivial_v<cell>);

class minefield
{
public:
    minefield(size_t width, size_t height);

    void handle_input();

    void update();

    void render(texture_manager& manager);

    void generate();

    void on_left_click(size_t x, size_t y);
    void on_right_click(size_t x, size_t y);
private:
    size_t width_;
    size_t height_;
    std::vector<cell> field_;
};

} // namespace minesweeper

#endif // MSWPR_MINEFIELD_HPP
