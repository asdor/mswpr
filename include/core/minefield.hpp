#ifndef MSWPR_MINEFIELD_HPP
#define MSWPR_MINEFIELD_HPP

#include "core/cell.hpp"
#include "core/cell_grid.hpp"

#include <vector>

namespace mswpr
{
  enum class open_cell_result
  {
    OPENED,
    DETONATED
  };

  class minefield
  {
  public:
    minefield(size_t width, size_t height, size_t bombs_cnt);

    template<MinesGenerator Generator>
    void generate(const Generator& i_generator)
    {
      d_grid.generate(i_generator);
    }

    void reset();

    const cell_grid& get_grid() const;
    const cell& operator()(size_t i_x, size_t i_y) const;

    size_t get_width() const;
    size_t get_height() const;
    size_t get_bomb_cnt() const;

    bool is_deminied() const;

    int get_value(size_t x, size_t y) const;
    cell_state get_cell_state(size_t x, size_t y) const;
    cell_value get_cell_value(size_t x, size_t y) const;

    void detonate_bomb(size_t x, size_t y);
    open_cell_result reveal_closed(size_t x, size_t y);

    void toggle_flag(size_t x, size_t y);

    void flag_bombs();
    void reveal_bombs();

  private:
    void open_cell(size_t i_x, size_t i_y);

    size_t d_width;
    size_t d_height;
    size_t d_bombs_cnt;
    size_t d_unopened_cnt;
    cell_grid d_grid;
  };

}  // namespace mswpr

#endif  // MSWPR_MINEFIELD_HPP
