cmake_minimum_required(VERSION 3.20)
project(maze_solver LANGUAGES C)

add_executable(mazer)

target_compile_options(mazer
  PRIVATE
  -Wall
  -pedantic
  -Wpointer-arith
  -Wconversion
  -Wimplicit-fallthrough
  -Werror=implicit-function-declaration
)

target_sources(mazer
  PRIVATE
  src/main.c
  src/maze.c
)

install(TARGETS mazer DESTINATION bin)
