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

  class minefield
  {
  public:
    minefield(size_t width, size_t height, size_t bombs_cnt);
    minefield(const std::vector<size_t>& mines_ind, size_t width, size_t height, size_t bombs_cnt);
    void generate(size_t x, size_t y);
    void reset();

    void render(texture_manager& manager);

    void reveal_closed(size_t x, size_t y);

    bool is_bomb(size_t x, size_t y) const;
    int get_value(size_t x, size_t y) const;

    bool is_opened(size_t x, size_t y) const;
    bool is_closed(size_t x, size_t y) const;
    bool is_flagged(size_t x, size_t y) const;
    bool is_detonated(size_t x, size_t y) const;

    bool open_cell(size_t x, size_t y);
    void set_flag(size_t x, size_t y);
    void detonate_bomb(size_t x, size_t y);

  private:
    void place_values_around_mines();

    size_t width_;
    size_t height_;
    size_t bombs_cnt_;
    std::vector<cell> field_;
  };

}  // namespace mswpr

#endif  // MSWPR_MINEFIELD_HPP
