#include "./window.hpp"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "ds/optional.hpp"

namespace sdl_imm {

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

  return opt_error{ds::null};
}

Window::~Window() noexcept {
  if (this->renderer != nullptr) {
    SDL_DestroyRenderer(this->renderer);
    this->renderer = nullptr;
  }

  if (this->window != nullptr) {
    SDL_DestroyWindow(this->window);
    this->window = nullptr;
  }
}

bool Window::start() noexcept {
  SDL_Event event{};
  if (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      return false;
    }
  }

  // Clear screen
  SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderClear(this->renderer);

  return true;
}

void Window::end() noexcept {
  SDL_RenderPresent(this->renderer);
}

// Drawing Stuff
void Window::draw_rectangle(const rect<f32>& dimensions, rgba8 color) noexcept {
  SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);
  SDL_RenderRect(this->renderer, (SDL_FRect*)&dimensions);
}

void Window::fill_rectangle(const rect<f32>& dimensions, rgba8 color) noexcept {
  SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);
  SDL_RenderFillRect(this->renderer, (SDL_FRect*)&dimensions);
}

} // namespace sdl_imm
