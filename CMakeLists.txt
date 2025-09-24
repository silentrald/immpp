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

add_executable(${PROJECT_NAME} src/main src/window.cpp)
include_directories(src)

# SDL3 stuff
set(SDL3_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl3)
set(SDL3_TTF_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl3-ttf)
set(SDL3_IMG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/sdl3-img)
set(SDL_TEST OFF)
add_subdirectory(${SDL3_DIR})
add_subdirectory(${SDL3_TTF_DIR})
add_subdirectory(${SDL3_IMG_DIR})


# TODO: Unit testing
# add_subdirectory(external/catch2)
# add_executable(pixel_draw test/draw.cpp src/math.cpp ${draw_srcs})
# target_link_libraries(pixel_draw PRIVATE Catch2::Catch2WithMain)

# add_executable(pixel_vector test/vector.cpp)
# target_link_libraries(pixel_vector PRIVATE Catch2::Catch2WithMain)

if (UNIX)
  set(SDL_IMM_LIBRARIES
    ds
    SDL3::SDL3
    SDL3_ttf-shared
    SDL3_image-shared
    # png
  )
elseif (WIN32)
  set(SDL_IMM_LIBRARIES
    ds
    SDL3::SDL3
    SDL3_ttf-shared
    SDL3_image-shared
    # png z
  )
endif (UNIX)

target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL_IMM_LIBRARIES})
