#include "./window.hpp"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_surface.h"
#include "SDL3/SDL_video.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "ds/vector.hpp"
#include <cstring>

namespace {

inline void set_color(SDL_Renderer* renderer, immpp::rgba8 color) noexcept {
  SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

// NOTE: Can create a static vector implementation with ds
inline immpp::i32
key_index(const ds::vector<immpp::u32>& keys, immpp::u32 key) noexcept {
  for (immpp::i32 i = 0; i < keys.get_size(); ++i) {
    if (keys[i] == key) {
      return i;
    }
  }

  return -1;
}

inline SDL_Texture* create_text_texture(
    SDL_Renderer* renderer, TTF_Font* font, const immpp::c8* text,
    immpp::i32 text_length, immpp::rgba8 color
) noexcept {
  // Draw the text
  SDL_Surface* surface =
      TTF_RenderText_Solid(font, text, text_length, *(SDL_Color*)&color);
  if (surface == nullptr) {
    return nullptr;
  }

  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_DestroySurface(surface);

  return texture;
}

} // namespace

namespace immpp {

Window::Window(Window&& other) noexcept
    : window(other.window), renderer(other.renderer) {
  other.window = nullptr;
  other.renderer = nullptr;
}

Window& Window::operator=(Window&& rhs) noexcept {
  if (this == &rhs) {
    return *this;
  }

  this->window = rhs.window;
  this->renderer = rhs.renderer;
  rhs.window = nullptr;
  rhs.renderer = nullptr;

  return *this;
}

opt_error Window::init(const c8* title) noexcept {
  this->window = SDL_CreateWindow(title, 640, 480, SDL_WINDOW_RESIZABLE);
  if (this->window == nullptr) {
    return opt_error{error_codes::SDL_INIT_ERROR};
  }

  this->renderer = SDL_CreateRenderer(this->window, nullptr);
  if (this->renderer == nullptr) {
    return opt_error{error_codes::SDL_INIT_ERROR};
  }

  this->font = TTF_OpenFont("../assets/fonts/PixeloidSans.ttf", 16);
  if (this->font == nullptr) {
    return opt_error{error_codes::SDL_INIT_ERROR};
  }

  return opt_error{null};
}

Window::~Window() noexcept {
  if (this->font != nullptr) {
    TTF_CloseFont(this->font);
    this->font = nullptr;
  }

  if (this->renderer != nullptr) {
    SDL_DestroyRenderer(this->renderer);
    this->renderer = nullptr;
  }

  if (this->window != nullptr) {
    SDL_DestroyWindow(this->window);
    this->window = nullptr;
  }
}

// Drawing Stuff

bool Window::start_render() noexcept {
  SDL_Event event{};
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_EVENT_QUIT:
      return false;

    case SDL_EVENT_MOUSE_MOTION:
      this->mouse_position.x = event.motion.x;
      this->mouse_position.y = event.motion.y;
      break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
      if (event.button.button == SDL_BUTTON_LEFT) {
        this->mouse_left = true;
      } else if (event.button.button == SDL_BUTTON_RIGHT) {
        this->mouse_right = true;
      } else if (event.button.button == SDL_BUTTON_MIDDLE) {
        this->mouse_middle = true;
      }
      break;

    case SDL_EVENT_MOUSE_BUTTON_UP:
      if (event.button.button == SDL_BUTTON_LEFT) {
        this->mouse_left = false;
      } else if (event.button.button == SDL_BUTTON_RIGHT) {
        this->mouse_right = false;
      } else if (event.button.button == SDL_BUTTON_MIDDLE) {
        this->mouse_middle = false;
      }
      break;

    case SDL_EVENT_KEY_DOWN:
      if (this->keys.get_size() >= 10) {
        break;
      }

      if (key_index(this->keys, event.key.key) == -1) {
        (void)this->keys.push(event.key.key);
      }
      break;

    case SDL_EVENT_KEY_UP:
      if (this->keys.is_empty()) {
        break;
      }

      {
        i32 index = key_index(this->keys, event.key.key);
        if (index > -1) {
          this->keys.remove(index);
        }
      }
      break;

    default:
      break;
    }
  }

  // Clear screen
  SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(this->renderer);

  return true;
}

void Window::end_render() noexcept {
  SDL_RenderPresent(this->renderer);
}

bool Window::text_button(rect<f32> dimensions, const c8* text) noexcept {
  i32 text_length = std::strlen(text);

  // Compute the rect
  vec2<i32> size{};
  TTF_GetStringSize(this->font, text, text_length, &size.x, &size.y);
  SDL_FRect text_rect{
      .x = dimensions.x + ((dimensions.w - size.x) / 2.0F),
      .y = dimensions.y + ((dimensions.h - size.y) / 2.0F),
      .w = (float)size.x,
      .h = (float)size.y
  };

  // Draw button background
  set_color(this->renderer, this->background_color);
  SDL_RenderFillRect(this->renderer, (SDL_FRect*)&dimensions);
  set_color(this->renderer, this->foreground_color);
  SDL_RenderRect(this->renderer, (SDL_FRect*)&dimensions);

  SDL_Texture* texture = create_text_texture(
      this->renderer, this->font, text, text_length, this->foreground_color
  );
  SDL_RenderTexture(this->renderer, texture, nullptr, &text_rect);
  SDL_DestroyTexture(texture);

  return false;
}

bool Window::text_button(vec2<f32> position, const c8* text) noexcept {
  i32 text_length = std::strlen(text);

  // Compute the rect
  vec2<i32> size{};
  TTF_GetStringSize(this->font, text, text_length, &size.x, &size.y);
  SDL_FRect text_rect{
      .x = position.x + this->padding.x,
      .y = position.y + this->padding.y,
      .w = (f32)size.x,
      .h = (f32)size.y
  };
  rect<f32> button_rect{
      .x = position.x,
      .y = position.y,
      .w = size.x + (2.0F * this->padding.x),
      .h = size.y + (2.0F * this->padding.y)
  };

  // Draw button background
  set_color(this->renderer, this->background_color);
  SDL_RenderFillRect(this->renderer, (SDL_FRect*)&button_rect);
  set_color(this->renderer, this->foreground_color);
  SDL_RenderRect(this->renderer, (SDL_FRect*)&button_rect);

  SDL_Texture* texture = create_text_texture(
      this->renderer, this->font, text, text_length, this->foreground_color
  );
  SDL_RenderTexture(this->renderer, texture, nullptr, &text_rect);
  SDL_DestroyTexture(texture);

  return false;
}

void Window::rectangle(rect<f32> dimensions, rgba8 color) noexcept {
  set_color(this->renderer, color);
  SDL_RenderRect(this->renderer, (SDL_FRect*)&dimensions);
}

void Window::fill_rectangle(rect<f32> dimensions, rgba8 color) noexcept {
  set_color(this->renderer, color);
  SDL_RenderFillRect(this->renderer, (SDL_FRect*)&dimensions);
}

} // namespace immpp
