/* Copyright 2025 Eero Prittinen */

#include <stdlib.h>
#include <stdio.h> // TODO: Deelete laterz
#include <string.h> // TODO: Deelete laterz
#include "maze.h"

int getLeft(char* maze, int mazeWidth, int mazeHeight, int self) {
  // Check for outer wall
  if (self % mazeWidth == 0) return -1; // Cant go outside of maze
  else if (maze[self - 1] == '#') return -1; // Cant go to wall
  else return self - 1;
}

int getRight(char* maze, int mazeWidth, int mazeHeight, int self) {
  // Check for outer wall
  if (self % mazeWidth >= (mazeWidth - 1) ) return -1; // Cant go outside of maze
  else if (maze[self + 1] == '#') return -1; // Cant go to wall
  else return self + 1;
}

int getTop(char* maze, int mazeWidth, int mazeHeight, int self) {
  // Check for outer wall
  if (self / mazeWidth == 0 ) return -1; // Cant go outside of maze
  else if (maze[self - mazeWidth] == '#') return -1; // Cant go to wall
  else return self - mazeWidth;
}

int getBottom(char* maze, int mazeWidth, int mazeHeight, int self) {
  // Check for outer wall
  if (self / mazeWidth >= mazeHeight - 1 ) return -1; // Cant go outside of maze
  else if (maze[self + mazeWidth] == '#') return -1; // Cant go to wall
  else return self + mazeWidth;
}

void rateNeighbours(char* maze, int ** dist, int mazeWidth, int mazeHeight, int self, int parentDist) {
  if ((*dist)[self] != 0 && (*dist)[self] <= parentDist) return;
  (*dist)[self] = parentDist;
  int left = getLeft(maze, mazeWidth, mazeHeight, self);
  if (left != -1) {
    rateNeighbours(maze, dist, mazeWidth, mazeHeight, left, parentDist + 1);
  }
  int right = getRight(maze, mazeWidth, mazeHeight, self);
  if (right != -1) {
    rateNeighbours(maze, dist, mazeWidth, mazeHeight, right, parentDist + 1);
  }
  int top = getTop(maze, mazeWidth, mazeHeight, self);
  if (top != -1) {
    rateNeighbours(maze, dist, mazeWidth, mazeHeight, top, parentDist + 1);
  }
  int bottom = getBottom(maze, mazeWidth, mazeHeight, self);
  if (bottom != -1) {
    rateNeighbours(maze, dist, mazeWidth, mazeHeight, bottom, parentDist + 1);
  }
}

int solve(const struct maze_node *const maze, const int mazeWidth, const int mazeHeight, struct maze_node **route) {
  // Find start of the maze
  struct maze_node *nodeListEnd = maze + (mazeWidth * mazeHeight);
  struct maze_node *start = maze;
  while (start->type != MAZE_START) {
    if (start >= nodeListEnd) {
      return MAZE_ERROR_NO_START;
    }
  }
  // TODO: SOLVE
  return 1; // TODO
}






int solveSimple(char *maze, int mazeWidth, int mazeHeight, int **mazeRoute) {
  // Find start of the maze
  int mazeStart;
  for (mazeStart = 0; maze[mazeStart] != CHAR_START; mazeStart++) {
    if (mazeStart >= mazeWidth * mazeHeight) {
      return MAZE_ERROR_NO_START;
    }
  }

  int *dist = malloc(mazeWidth * mazeHeight * sizeof(int));
  memset(dist, 0, mazeWidth * mazeHeight * sizeof(int));
  rateNeighbours(maze, &dist, mazeWidth, mazeHeight, mazeStart, 1);


  // Todo: Walk back from ends


  // TODO: Remove when tests come
  // Check caölculated read correctly
  for (int i = 0; i < mazeHeight * mazeWidth; i++) {
    if (maze[i] == ' ') {
      printf("%0.3d ", dist[i]);
    } else {
       printf("%c", maze[i]);
       printf("%c", maze[i]);
       printf("%c ", maze[i]);
    }
    if ((i + 1) % mazeWidth == 0) {
      printf("\n");
    }
  }

  return 1;
}
