#ifndef MSWPR_MINEFIELD_HPP
#define MSWPR_MINEFIELD_HPP

#include <array>
#include <vector>
#include <type_traits>

namespace mswpr
{
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
    FLAGGED,
    DETONATED
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
    bool is_detonated() const;
  };
  static_assert(std::is_trivial_v<cell>);

  struct cell_coord
  {
    size_t x;
    size_t y;

    bool operator==(const cell_coord& other) const noexcept = default;
  };

  enum class open_cell_result
  {
    OPENED,
    DETONATED
  };

  class minefield
  {
  public:
    minefield(size_t width, size_t height, size_t bombs_cnt);
    minefield(const std::vector<size_t>& mines_ind, size_t width, size_t height, size_t bombs_cnt);
    void generate(size_t x, size_t y);
    void reset();

    size_t get_bomb_cnt() const;

    bool is_deminied() const;

    bool is_bomb(size_t x, size_t y) const;
    int get_value(size_t x, size_t y) const;
    cell_state get_cell_state(size_t x, size_t y) const;
    cell_value get_cell_value(size_t x, size_t y) const;

    bool is_opened(size_t x, size_t y) const;
    bool is_closed(size_t x, size_t y) const;
    bool is_flagged(size_t x, size_t y) const;
    bool is_detonated(size_t x, size_t y) const;

    void detonate_bomb(size_t x, size_t y);
    open_cell_result reveal_closed(size_t x, size_t y);

    void set_flag(size_t x, size_t y);

    std::vector<cell_coord> get_neighbours(cell_coord coord) const;

    static constexpr std::array<int, 8> neighbours_x_ind = { -1, 0, 1, -1, 1, -1, 0, 1 };
    static constexpr std::array<int, 8> neighbours_y_ind = { -1, -1, -1, 0, 0, 1, 1, 1 };

  private:
    void place_values_around_mines();

    void open_cell(size_t x, size_t y);
    void open_cell(cell& i_cell);

    std::vector<size_t> get_mines_candidates(size_t x, size_t y) const;

    size_t width_;
    size_t height_;
    size_t bombs_cnt_;
    size_t unopened_cnt_;
    std::vector<cell> field_;
  };

  int some_unused_function_for_test(int x);

}  // namespace mswpr

#endif  // MSWPR_MINEFIELD_HPP
