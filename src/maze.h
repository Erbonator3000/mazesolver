/* Copyright 2025 Eero Prittinen */

#ifndef MAZE_H
#define MAZE_H

#define CHAR_WALL '#'
#define CHAR_PASS ' '
#define CHAR_START '^'
#define CHAR_END 'E'

typedef enum {
  MAZE_WALL,
  MAZE_PASS,
  MAZE_START,
  MAZE_END
} maze_space_t;

typedef enum {
  MAZE_ERROR_NO_START = -1, /* Start doesa not exist */
  MAZE_ERROR_NO_END = -2, /* End does not exit */
  MAZE_ERROR_NO_ROUTE = -3, /* No route found between start and end */
} solver_error_t;
struct maze_node {
  // struct maze_node *up;
  // struct maze_node *down;
  // struct maze_node *left;
  // struct maze_node *right;
  int x;
  int y;
  maze_space_t type;
  int dist;
};

/**
 * @brief Finds the optimal path trough the maze
 * 
 * @param maze Array of maze nodes, left to right, top to bottom order
 * @param mazeWidth Width of the maze
 * @param mazeHeight Height of the maze
 * @param route Pointer where to store maze solution
 * @param route Pointer where to store maze solution
 * @param routeLength Length of the maze solution
 * @return int Length of the route, or error value, see solver_error_t
 */
int solve(const struct maze_node *const maze, const int mazeWidth, const int mazeHeight, struct maze_node **route);
// TODO: Use enum type as return type??

int solveSimple(char *maze, int mazeWidth, int mazeHeight, int **mazeRoute);

#endif /* MAZE_H */
