cmake_minimum_required(VERSION 3.5)

project(immpp VERSION 1.0.1)

option(IMMPP_SAMPLES "IMMPP Example" OFF)

# Main Stuff
set(IMMPP_SOURCES
  src/immpp/dev_logger.cpp
  src/immpp/math.cpp
  src/immpp/size.cpp
)

set(SDL_LIBRARIES
  ds
  SDL3::SDL3
  SDL3_ttf-shared
  SDL3_image-shared
)
set(SDL_SOURCES
  src/backend/sdl3/initializer.cpp
  src/backend/sdl3/window.cpp
)

add_library(${PROJECT_NAME}
  ${IMMPP_SOURCES}
  ${SDL_SOURCES}
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC src)

if (IMMPP_SAMPLES)
  set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
  set(CMAKE_CXX_STANDARD 17)

  include_directories(src)
  add_subdirectory(external/ds)

  # === SDL3 === #
  set(SDL3_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl3)
  set(SDL3_TTF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl3-ttf)
  set(SDL3_IMG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl3-img)
  set(SDL_TEST OFF)
  add_subdirectory(${SDL3_DIR})
  add_subdirectory(${SDL3_TTF_DIR})
  add_subdirectory(${SDL3_IMG_DIR})

  add_executable(sdl3_animation
    samples/animation.cpp
    ${IMMPP_SOURCES}
    ${SDL_SOURCES}
  )
  target_link_libraries(sdl3_animation PRIVATE ${SDL_LIBRARIES})

  add_executable(sdl3_anchor
    samples/anchor.cpp
    ${IMMPP_SOURCES}
    ${SDL_SOURCES}
  )
  target_link_libraries(sdl3_anchor PRIVATE ${SDL_LIBRARIES})
endif (IMMPP_SAMPLES)

