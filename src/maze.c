/* Copyright 2025 Eero Prittinen */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include "maze.h"

// Default characters to parse the maze
#define MAZE_CHAR_WALL '#'
#define MAZE_CHAR_PASS ' '
#define MAZE_CHAR_START '^'
#define MAZE_CHAR_EXIT 'E'
#define MAZE_CHAR_PATH '*'

static maze_node_t *getLeft(maze_solver_handle_t *mazeHandle, maze_node_t *self) {
  long int index = (self - mazeHandle->maze);
  if (index % mazeHandle->width == 0 || mazeHandle->maze[index - 1].type == WALL) {
    return NULL;
  }
  maze_node_t *left = &mazeHandle->maze[index - 1];
  if (left->type == WALL) {
    return NULL;
  }
  return left;
}

static maze_node_t *getRight(maze_solver_handle_t *mazeHandle, maze_node_t *self) {
  long int index = (self - mazeHandle->maze);
  if (index % mazeHandle->width >= (mazeHandle->width - 1)) {
    return NULL;
  }
  maze_node_t *right = &mazeHandle->maze[index + 1];
  if (right->type == WALL) {
    return NULL;
  }
  return right;
}

static maze_node_t *getTop(maze_solver_handle_t *mazeHandle, maze_node_t *self) {
  long int index = (self - mazeHandle->maze);
  if (index / mazeHandle->width == 0) {
    return NULL;
  }
  maze_node_t *top = &mazeHandle->maze[index - mazeHandle->width];
  if (top->type == WALL) {
    return NULL;
  }
  return top;
}

static maze_node_t *getBottom(maze_solver_handle_t *mazeHandle, maze_node_t *self) {
  long int index = (self - mazeHandle->maze);
  if (index / mazeHandle->width >= mazeHandle->height - 1) {
    return NULL;
  }
  maze_node_t *bottom = &mazeHandle->maze[index + mazeHandle->width];
  if (bottom->type == WALL) {
    return NULL;
  }
  return bottom;
}

int mazeSolver_solve(maze_solver_handle_t *mazeHandle, int searchLimit) {
  maze_node_t *start = NULL;

  // Find the start of the maze
  for (int i = 0; i < mazeHandle->width * mazeHandle->height; i++) {
    if (mazeHandle->maze[i].type == START) {
      if (start != NULL) {
        return MAZE_ERROR_MULTIPLE_START;
      }
      start = &mazeHandle->maze[i];
    }
  }
  if (start == NULL) {
    return MAZE_ERROR_NO_START;
  }

  start->dist = 0;

  maze_node_t *current = start;
  maze_node_t *listTail = start;

  // Loop trough the list, adding to the end the next nodes to search
  // This allows for breadth-first search trough the graph
  while (current != NULL) {
    if (current->dist > searchLimit) {
      return MAZE_ERROR_MAX_STEPS_REACHED;
    }

    if (current->type == EXIT) {
      // Exit node found, search no further
      break;
    }

    maze_node_t *left = getLeft(mazeHandle, current);
    if (left != NULL && left->parent == NULL) {
      left->parent = current;
      left->dist = current->dist + 1;

      listTail->processNext = left;
      listTail = left;
    }
    maze_node_t *right = getRight(mazeHandle, current);
    if (right != NULL && right->parent == NULL) {
      right->parent = current;
      right->dist = current->dist + 1;
      listTail->processNext = right;
      listTail = right;
    }
    maze_node_t *top = getTop(mazeHandle, current);
    if (top != NULL && top->parent == NULL) {
      top->parent = current;
      top->dist = current->dist + 1;
      listTail->processNext = top;
      listTail = top;
    }
    maze_node_t *bottom = getBottom(mazeHandle, current);
    if (bottom != NULL && bottom->parent == NULL) {
      bottom->parent = current;
      bottom->dist = current->dist + 1;
      listTail->processNext = bottom;
      listTail = bottom;
    }
    current = current->processNext;
  }

  // Breaking out of the loop means that exit found or all paths from start exhausted
  if (current == NULL || current->type != EXIT) {
    return MAZE_ERROR_NO_ROUTE;
  }

  mazeHandle->exitPathLength = current->dist;
  mazeHandle->exitPath = malloc(current->dist * sizeof(*(mazeHandle->exitPath)));
  if (mazeHandle->exitPath == NULL) {
    return MAZE_ERROR_FAILED_MEMORY_ALLOCATION;
  }

  // Walk back from the exit and record the used path
  while (current != start) {
    mazeHandle->exitPath[current->dist - 1] = current - mazeHandle->maze;
    current = current->parent;
  }

  return mazeHandle->exitPathLength;
}

int mazeSolver_loadMazeFromFile(maze_solver_handle_t *mazeHandle, FILE *mazeFile) {
  // Count the maze size
  mazeHandle->width = 0;
  char c = (char) fgetc(mazeFile);
  while (c != '\n' && c != EOF) {
    mazeHandle->width++;
    c = (char) fgetc(mazeFile);
  }

  mazeHandle->height = 1; // Got one row already
  int widthCheck = 0;
  while ((c = (char) fgetc(mazeFile)) != EOF) {
    if (c == '\n') {
      if (widthCheck != mazeHandle->width) {
        return MAZE_ERROR_INCORRECT_FORMAT;
      }
      widthCheck = 0;
      mazeHandle->height++;
    } else {
      widthCheck++;
    }
  }

  // Make sure we can process the maze
  if (mazeHandle->width > INT_MAX || mazeHandle->height > INT_MAX || mazeHandle->width * mazeHandle->height > INT_MAX) {
    fclose(mazeFile);
    return MAZE_ERROR_MAZE_TOO_LARGE;
  }

  rewind(mazeFile);

  mazeHandle->maze = malloc(mazeHandle->width * mazeHandle->height * sizeof(*mazeHandle->maze));
  if (mazeHandle->maze == NULL) {
    fclose(mazeFile);
    return MAZE_ERROR_FAILED_MEMORY_ALLOCATION;
  }

  int i = 0;
  while ((c = (char) fgetc(mazeFile)) != EOF) {
    switch (c) {
      case MAZE_CHAR_PASS:
        mazeHandle->maze[i].type = PASS;
        i++;
        break;
      case MAZE_CHAR_WALL:
        mazeHandle->maze[i].type = WALL;
        i++;
        break;
      case MAZE_CHAR_EXIT:
        mazeHandle->maze[i].type = EXIT;
        i++;
        break;
      case MAZE_CHAR_START:
        mazeHandle->maze[i].type = START;
        i++;
        break;
      case '\n':
        // Do nothing
        break;
      default:
        fclose(mazeFile);
        return MAZE_ERROR_INCORRECT_FORMAT;
        break;
    }
  }
  fclose(mazeFile);
  return MAZE_OK;
}

void mazeSolver_init(maze_solver_handle_t *mazeHandle) {
  // Set all pointers to NULL
  memset(mazeHandle, 0, sizeof(maze_solver_handle_t));
}

void mazeSolver_deinit(maze_solver_handle_t *mazeHandle) {
  // Free all used dynamic memory
  if (mazeHandle->exitPath != NULL) {
    free(mazeHandle->exitPath);
  }
  if (mazeHandle->maze != NULL) {
    free(mazeHandle->maze);
  }
}

int mazeSolver_print(maze_solver_handle_t *mazeHandle, FILE *stream) {
  char *printableMaze = malloc(mazeHandle->width * mazeHandle->height * sizeof(char));
  if (printableMaze == NULL) {
    return MAZE_ERROR_FAILED_MEMORY_ALLOCATION;
  }
  for (int i = 0; i < mazeHandle->height * mazeHandle->width; i++) {
    switch (mazeHandle->maze[i].type) {
      case PASS:
        printableMaze[i] = MAZE_CHAR_PASS;
        break;
      case WALL:
        printableMaze[i] = MAZE_CHAR_WALL;
        break;
      case EXIT:
        printableMaze[i] = MAZE_CHAR_EXIT;
        break;
      case START:
        printableMaze[i] = MAZE_CHAR_START;
        break;
    }
  }

  for (int i = 0; i < mazeHandle->exitPathLength; i++) {
    printableMaze[mazeHandle->exitPath[i]] = MAZE_CHAR_PATH; // Mark the path
  }

  // Nicer print with unicode
  for (int i = 0; i < mazeHandle->height * mazeHandle->width; i++) {
    fprintf(stream, "%c", printableMaze[i]);
    if ((i + 1) % mazeHandle->width == 0) {
      fprintf(stream, "\n");
    }
  }
  free(printableMaze);
  return MAZE_OK;
}
