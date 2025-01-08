cmake_minimum_required(VERSION 3.20)
project(maze_solver LANGUAGES C)

add_executable(solver)

target_compile_options(solver
  PRIVATE
  -Wall
  -pedantic
  -Wpointer-arith
  -Wconversion
  -Wimplicit-fallthrough
  -Werror=implicit-function-declaration
  -Wno-sign-conversion
)

target_sources(solver
  PRIVATE
  src/main.c
  src/maze.c
)

target_include_directories(solver
  PRIVATE
  src
)

install(TARGETS solver DESTINATION bin)
