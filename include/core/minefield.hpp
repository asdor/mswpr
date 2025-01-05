#ifndef MSWPR_MINEFIELD_HPP
#define MSWPR_MINEFIELD_HPP

#include "core/cell.hpp"
#include "core/cell_grid.hpp"

#include <array>
#include <vector>

namespace mswpr
{
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

    int get_value(size_t x, size_t y) const;
    cell_state get_cell_state(size_t x, size_t y) const;
    cell_value get_cell_value(size_t x, size_t y) const;

    bool is_bomb(size_t x, size_t y) const;
    bool is_opened(size_t x, size_t y) const;
    bool is_closed(size_t x, size_t y) const;
    bool is_flagged(size_t x, size_t y) const;
    bool is_detonated(size_t x, size_t y) const;
    bool is_not_flagged_bomb(size_t x, size_t y) const;

    void detonate_bomb(size_t x, size_t y);
    open_cell_result reveal_closed(size_t x, size_t y);

    void set_flag(size_t x, size_t y);

    std::vector<cell_coord> get_neighbours(cell_coord coord) const;

    void flag_bombs();
    void reveal_bombs();

    static constexpr std::array<int, 8> neighbours_x_ind = { -1, 0, 1, -1, 1, -1, 0, 1 };
    static constexpr std::array<int, 8> neighbours_y_ind = { -1, -1, -1, 0, 0, 1, 1, 1 };

  private:
    void place_values_around_mines();

    void open_cell(size_t x, size_t y);
    void open_cell(cell& i_cell);

    std::vector<size_t> get_mines_candidates(size_t x, size_t y) const;

    size_t d_width;
    size_t d_height;
    size_t d_bombs_cnt;
    size_t d_unopened_cnt;
    cell_grid d_grid;
  };

}  // namespace mswpr

#endif  // MSWPR_MINEFIELD_HPP
