#include "gui/game_engine.hpp"

#include <iostream>

int main()
{
  try
  {
    mswpr::game_engine engine("mswpr", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

    while (engine.running())
    {
      engine.handle_input();
      engine.update();
      engine.render();

      engine.limit_fps();
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "The game was terminated with exception: " << e.what() << '\n';
    return -1;
  }

  return 0;
}
