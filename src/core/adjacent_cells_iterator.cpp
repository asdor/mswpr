#include "core/adjacent_cells_iterator.hpp"

std::vector<mswpr::cell_coord> mswpr::fetch_adjacent_cells(mswpr::cell_coord i_coord,
                                                           std::size_t i_width,
                                                           std::size_t i_height)
{
  std::vector<cell_coord> neighbours;
  const int x_i = static_cast<int>(i_coord.x);
  const int y_i = static_cast<int>(i_coord.y);

  for (size_t i = 0; i < NEIGHBOURS_X_IND.size(); ++i)
  {
    const int x_ind = mswpr::NEIGHBOURS_X_IND[i];
    const int y_ind = mswpr::NEIGHBOURS_Y_IND[i];
    if (x_i < -x_ind || y_i < -y_ind)
      continue;

    const int x = x_i + x_ind;
    const int y = y_i + y_ind;
    if (x < static_cast<int>(i_width) && y < static_cast<int>(i_height))
      neighbours.emplace_back(x, y);
  }

  return neighbours;
}
