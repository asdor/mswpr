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

    bool is_empty() const;
    bool is_bomb() const;

    bool is_closed() const;
    bool is_opened() const;
    bool is_flagged() const;
};
static_assert(std::is_trivial_v<cell>);

class minefield
{
public:
    minefield(size_t width, size_t height, size_t bombs_cnt);

    cell& operator()(size_t x, size_t y);
    const cell& operator()(size_t x, size_t y) const;

    // void set_flag(size_t x, size_t y);

    void reveal_cell(size_t x, size_t y);

    // void handle_input();

    // void update();

    void render(texture_manager& manager);

    void reset();

    // void on_left_click(size_t x, size_t y);
    // void on_right_click(size_t x, size_t y);
    // void reveal_closed(size_t x, size_t y);
    const size_t width_;
    const size_t height_;
    const size_t bombs_cnt_;

private:
    std::vector<cell> field_;
};

} // namespace mswpr

#endif // MSWPR_MINEFIELD_HPP
