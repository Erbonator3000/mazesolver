/* Copyright 2025 Eero Prittinen */

#ifndef MAZE_H
#define MAZE_H

#include <stdio.h>

typedef enum {
  WALL,
  PASS,
  START,
  EXIT,
} node_type_t;

typedef struct maze_node maze_node_t;

struct maze_node {
  node_type_t type;
  int dist; // Step distance from the start
  maze_node_t *parent; // Used for backtracing the found path
  maze_node_t *processNext; // Used for queueing nodes for processing
} maze_node;

typedef struct {
  int width;
  int height;
  maze_node_t *maze;
  long int *exitPath; // List of node indexes in maze, leading from start to exit
  int exitPathLength;
} maze_solver_handle_t;

typedef enum {
  MAZE_ERROR_NO_START = -2, /* Start does not exist */
  MAZE_ERROR_NO_ROUTE = -3, /* No route found between start and end */
  MAZE_ERROR_MULTIPLE_START = -4, /* Multiple starting points given in the maze */
  MAZE_ERROR_MAZE_TOO_LARGE = -5, /* Maze dimensions were too large to store in variables */
  MAZE_ERROR_FAILED_MEMORY_ALLOCATION = -6, /* Failed to dynamically allocate memory for the maze */
  MAZE_ERROR_INCORRECT_FORMAT = -7, /* Maze file format was somehow incorrect */
  MAZE_ERROR_MAX_STEPS_REACHED = -8, /* Maze could not be solved with the step limit */
} solver_error_t;

#define MAZE_OK 0

#define MAZE_ERROR(e) \
  ((e) == MAZE_ERROR_NO_START)                     ? "No route start found on maze\n" \
    : ((e) == MAZE_ERROR_NO_ROUTE)                 ? "No exit route found on maze\n" \
    : ((e) == MAZE_ERROR_MULTIPLE_START)           ? "Multiple starting points declared in the maze\n" \
    : ((e) == MAZE_ERROR_MAZE_TOO_LARGE)           ? "Maze dimensions were too large, can not process maze\n" \
    : ((e) == MAZE_ERROR_FAILED_MEMORY_ALLOCATION) ? "Failed dynamic memory allocation\n" \
    : ((e) == MAZE_ERROR_INCORRECT_FORMAT)         ? "Incorrectly formatted maze file\n" \
    : ((e) == MAZE_ERROR_MAX_STEPS_REACHED)        ? "Maximum number of steps reached\n" \
                                                   : "Unknown error\n"

/**
 * @brief Finds the optimal path trough the maze.
 *
 * @param mazeHandle Maze solver state handle
 * @param searchLimit Limit on the step distance which to search
 * @return int Maze exit path length, or error code (see @solver_error_t)
 */
int mazeSolver_solve(maze_solver_handle_t *mazeHandle, int searchLimit);

/**
 * @brief Load a maze from a file.
 *  Default file format:
 *    '#'               : Wall
 *    ' ' (empty space) : Passage, maze solving algorithm searches these for path
 *    '^'               : Start, path finding algorithm starts from this space
 *    'E'               : Exit, path finding algorithm tries to find path from start to closest exit
 *
 * @param maze Maze solver state handle
 * @param mazeFile File from which to parse the maze
 * @return int MAZE_OK on success, or error (see @solver_error_t)
 */
int mazeSolver_loadMazeFromFile(maze_solver_handle_t *mazeHandle, FILE *mazeFile);

/**
 * @brief Initialize maze solver with default values.
 *
 * @param mazeHandle Maze solver state handle
 */
void mazeSolver_init(maze_solver_handle_t *mazeHandle);

/**
 * @brief Deinits maze solver state and frees all dynamically allocated memory.
 *
 * @param mazeHandle Maze solver state handle
 */
void mazeSolver_deinit(maze_solver_handle_t *mazeHandle);

/**
 * @brief Prints the maze in to an I/O stream. Prints the path if the maze has been solved.
 *
 * @param mazeHandle Maze solver state handle
 * @param stream Stream to print the maze to
 * @return int MAZE_OK on success, or error (see @solver_error_t)
 */
int mazeSolver_print(maze_solver_handle_t *mazeHandle, FILE *stream);

#endif /* MAZE_H */
