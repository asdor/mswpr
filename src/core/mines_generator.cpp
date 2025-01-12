#include "core/mines_generator.hpp"
#include "core/adjacent_cells_iterator.hpp"
#include "core/cell.hpp"
#include "core/types.hpp"

#include <algorithm>

void mswpr::place_values_around_mines(mswpr::cell_grid& io_grid, std::size_t i_width, std::size_t i_height)
{
  for (size_t y = 0; y < i_height; ++y)
  {
    for (size_t x = 0; x < i_width; ++x)
    {
      auto& cell = io_grid(x, y);
      if (cell.is_bomb())
        continue;

      const auto neighbours = fetch_adjacent_cells({ x, y }, i_width, i_height);
      const size_t cnt = std::count_if(neighbours.begin(), neighbours.end(), [&io_grid](cell_coord i_cell) {
        return io_grid(i_cell.x, i_cell.y).is_bomb();
      });
      cell.set_value(to_enum<cell_value>(cnt));
    }
  }
}
