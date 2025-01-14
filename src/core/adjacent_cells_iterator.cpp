#include "core/adjacent_cells_iterator.hpp"

std::vector<mswpr::cell_coord> mswpr::fetch_adjacent_cells(mswpr::cell_coord i_coord,
                                                           std::size_t i_width,
                                                           std::size_t i_height)
{
  std::vector<cell_coord> neighbours;
  const int x_i = static_cast<int>(i_coord.x);
  const int y_i = static_cast<int>(i_coord.y);

  for (size_t i = 0; i < neighbours_x_ind.size(); ++i)
  {
    const int x_ind = mswpr::neighbours_x_ind[i];
    const int y_ind = mswpr::neighbours_y_ind[i];
    if (x_i < -x_ind || y_i < -y_ind)
      continue;

    const auto x = static_cast<size_t>(x_i + x_ind);
    const auto y = static_cast<size_t>(y_i + y_ind);
    if (x < i_width && y < i_height)
      neighbours.emplace_back(x, y);
  }

  return neighbours;
}
