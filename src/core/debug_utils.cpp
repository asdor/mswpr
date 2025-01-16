#include "core/debug_utils.hpp"
#include "core/cell_grid.hpp"
#include "core/types.hpp"

void mswpr::debug::display_grid_to_stream(std::ostream& io_os,
                                          const mswpr::cell_grid& i_grid,
                                          std::size_t i_width,
                                          std::size_t i_height)
{
  for (size_t y = 0; y < i_height; ++y)
  {
    for (size_t x = 0; x < i_width; ++x)
    {
      auto cell = i_grid(x, y).get_value();
      char cell_name = ' ';
      if (cell == mswpr::cell_value::EMPTY)
      {
        cell_name = ' ';
      }
      else if (cell == mswpr::cell_value::BOMB)
      {
        cell_name = '*';
      }
      else
      {
        cell_name = static_cast<char>('0' + (enum_to<std::size_t>(cell)));
      }

      io_os << cell_name;
    }
    io_os << '\n';
  }
}
