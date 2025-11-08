cmake_minimum_required(VERSION 3.5)

project(immpp VERSION 1.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_STANDARD 17)

if (WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
endif (WIN32)

# Main Stuff
add_subdirectory(external/ds)
include_directories(external/ds/src)

include_directories(src)

# === SDL3 === #
set(SDL3_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl3)
set(SDL3_TTF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl3-ttf)
set(SDL3_IMG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl3-img)
set(SDL_TEST OFF)
add_subdirectory(${SDL3_DIR})
add_subdirectory(${SDL3_TTF_DIR})
add_subdirectory(${SDL3_IMG_DIR})

if (UNIX)
  set(SDL_IMM_LIBRARIES
    ds
    SDL3::SDL3
    SDL3_ttf-shared
    SDL3_image-shared
  )
elseif (WIN32)
  set(SDL_IMM_LIBRARIES
    ds
    SDL3::SDL3
    SDL3_ttf-shared
    SDL3_image-shared
  )
endif (UNIX)

add_executable(sdl3
  samples/animation.cpp

  src/immpp/dev_logger.cpp
  src/immpp/math.cpp
  src/immpp/size.cpp
  src/backend/sdl3/initializer.cpp
  src/backend/sdl3/window.cpp
)
target_link_libraries(sdl3 PRIVATE ${SDL_IMM_LIBRARIES})

# === SDL3 === #

# TODO: Unit testing
# add_subdirectory(external/catch2)
# add_executable(pixel_draw test/draw.cpp src/math.cpp ${draw_srcs})
# target_link_libraries(pixel_draw PRIVATE Catch2::Catch2WithMain)

# add_executable(pixel_vector test/vector.cpp)
# target_link_libraries(pixel_vector PRIVATE Catch2::Catch2WithMain)
