// #include <array>
// #include <iostream>
// #include <iterator>
// #include <string_view>

// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>


// auto load_texture(SDL_Renderer* renderer, std::string_view texture)
// {
//     auto tex = IMG_LoadTexture(renderer, texture.data());
//     if (!tex)
//     {
//         const std::string err = "Failed to open " + std::string(texture);
//         throw std::runtime_error(err.c_str());
//     }

//     return tex;
// }

// void draw_texture(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect src_rect, SDL_Rect dst_rect, SDL_RendererFlip flip)
// {
//     SDL_RenderCopyEx(renderer, texture, &src_rect, &dst_rect, 0.0, nullptr, flip);
// }

// bool inside_rect(int x, int y, const SDL_Rect& rect)
// {
//     return x >= rect.x && x < rect.x + rect.w
//            && y >= rect.y && y < rect.y + rect.h;
// }

// template<size_t N>
// void process_mouse(const std::array<SDL_Rect, N>& dst_rects)
// {
//     int mouse_x = 0;
//     int mouse_y = 0;
//     SDL_GetMouseState(&mouse_x, &mouse_y);
//     SDL_Log("(%d, %d)", mouse_x, mouse_y);

//     // for (size_t i = 0; const auto& dst_rect : dst_rects)
//     for (size_t i = 0; i < N; ++i)
//     {
//         if (inside_rect(mouse_x, mouse_y, dst_rects[i]))
//             SDL_Log("Mouse inside: %d", int(i));
//     }


// }

#include "game.hpp"

int main()
{
    static constexpr int screen_width = 800;
    static constexpr int screen_height = 640;

    mswpr::game game("mswpr", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height);

    while (game.running())
    {
        game.handle_input();
        game.update();
        game.render();
    }

    return 0;
}

// int main()
// {
//     static constexpr int screen_width = 800;
//     static constexpr int screen_height = 640;
//     // static constexpr size_t FPS = 60;
//     // static constexpr size_t frame_delay = 1000 / FPS;
//     auto window = SDL_CreateWindow("mswpr", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, 0);
//     if (!window)
//     {
//         SDL_Log("Unable to create SDL_window: %s", SDL_GetError());
//         return -1;
//     }

//     auto renderer = SDL_CreateRenderer(window, -1, 0);
//     if (!renderer)
//     {
//         SDL_Log("Unable to create renderer: %s", SDL_GetError());
//         return -1;
//     }

//     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//     SDL_Log("Renderer created!\n");

//     SDL_Event event;
//     bool is_running = true;

//     auto tex_example = load_texture(renderer, "/home/asdor45/education/sdl2_test/assets/col_tex.png");
//     static constexpr int button_width = screen_width / 2;
//     static constexpr int button_height = screen_height / 2;
//     SDL_Rect src_rect = {0, 0, 32, 32};
//     SDL_Rect dst_rect_0 = {0, 0, button_width, button_height};
//     SDL_Rect dst_rect_1 = dst_rect_0;
//     dst_rect_1.x += button_width;
//     const std::array dst_rects = {dst_rect_0, dst_rect_1};

//     while(is_running)
//     {
//         int key = -1;
//         bool is_released = false;
//         while(SDL_PollEvent(&event))
//         {
//             switch (event.type)
//             {
//             case SDL_KEYUP:
//                 is_released = true;
//                 [[fallthrough]];
//             case SDL_KEYDOWN:
//                 switch (event.key.keysym.sym)
//                 {
//                 case SDLK_ESCAPE:
//                     is_running = false;
//                     break;
//                 default:
//                     break;
//                 }
//                 break;
//             case SDL_MOUSEBUTTONUP:
//                 is_released = true;
//                 SDL_Log("UP");
//                 [[fallthrough]];
//             case SDL_MOUSEBUTTONDOWN:
//                 switch (event.button.button)
//                 {
//                 case SDL_BUTTON_LEFT:
//                     SDL_Log("DOWN");
//                     key = SDL_BUTTON_LEFT;
//                     break;
//                 default:
//                     break;
//                 }
//             default:
//                 break;
//             }
//         }

//             // SDL_Log("Key: %d", key);

//             SDL_RenderClear(renderer);

//             for (const auto& dst_rect : dst_rects)
//                 draw_texture(renderer, tex_example, src_rect, dst_rect, SDL_FLIP_NONE);
//             SDL_RenderPresent(renderer);

//             if (key != -1)
//             {
//                 if (!is_released)
//                     continue;
//                 switch (key)
//                 {
//                 case SDL_BUTTON_LEFT:
//                     process_mouse(dst_rects);
//                     SDL_Log("After process");
//                     break;
//                 default:
//                     break;
//                 }
//             }

//         // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//         // SDL_RenderDrawRect(renderer, &rect);
//         // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);


//     }

//     return 0;
// }
