#ifndef MSWPR_MINEFIELD_HPP
#define MSWPR_MINEFIELD_HPP

#include <vector>
#include <type_traits>

namespace mswpr
{
class texture_manager;

enum class cell_value : size_t
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

enum class cell_state
{
    CLOSED,
    OPENED,
    FLAGGED
};

struct cell
{
    cell_value value;
    cell_state state;
};
static_assert(std::is_trivial_v<cell>);

class minefield
{
public:
    minefield(size_t width, size_t height, size_t bombs_cnt);

    void handle_input();

    void update();

    void render(texture_manager& manager);

    void generate();

    void on_left_click(size_t x, size_t y);
    void on_right_click(size_t x, size_t y);
private:
    size_t width_;
    size_t height_;
    size_t bombs_cnt_;
    std::vector<cell> field_;
};

} // namespace mswpr

#endif // MSWPR_MINEFIELD_HPP
