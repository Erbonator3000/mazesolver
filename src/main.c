/* Copyright 2025 Eero Prittinen */

#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

#define DIST_WALL 0xffffffff

int main(int argc, char **argv) {
  // TODO: Check for parameters and evaluate
  if (argc < 2) {
    printf("Provide a maze filename\n"); // TODO: Better error message
    return 1;
  }

  char *mazeFileName = argv[argc - 1];
  FILE *mazeFile = fopen(mazeFileName, "r");
  if (mazeFile == NULL) { // TODO: Should use ferror()?
    printf("Could not open maze %s\n", mazeFileName);
    return 1;
  }

  // TODO: Is there a newline in the end??
  // First count the maze size
  int mazeWidth = 0;
  char c = fgetc(mazeFile);
  while (c != '\n' && c != EOF) {
    mazeWidth++;
    c = fgetc(mazeFile);
  }

  int mazeHeight = 1; // Got one row already
  while ((c = fgetc(mazeFile)) != EOF) {
    if (c == '\n') {
      mazeHeight++;
    }
  }

  printf("Maze width: %d, height: %d\n", mazeWidth, mazeHeight);
  rewind(mazeFile);
  char *mazeNodes = malloc(mazeWidth * mazeHeight * sizeof(char)); // TODO: Check for malloc success
  // TODO: Make prettier
  int i = 0;
  while ((c = fgetc(mazeFile)) != EOF) {
    if (c != '\n') {
      mazeNodes[i] = c;
      i++;
    }
  }
  
  // TODO: Remove when tests come
  // Check read correctly
  for (int i = 0; i < mazeHeight * mazeWidth; i++) {
    printf("%c", mazeNodes[i]);
    if ((i + 1) % mazeWidth == 0) {
      printf("\n");
    }
  }
  
  int *mazeRoute;
  int routeLength = solveSimple(mazeNodes, mazeWidth, mazeHeight, &mazeRoute);
  if (routeLength == MAZE_ERROR_NO_START) {
    printf("No route start found on maze\n");
    return 1;
  } else if (routeLength == MAZE_ERROR_NO_END) {
    printf("No route end(s) found on maze\n");
    return 1;
  } else if (routeLength == MAZE_ERROR_NO_ROUTE) {
    printf("No route found on maze\n");
    return 1;
  }


  // struct maze_node *mazeNodes = malloc(mazeWidth * mazeHeight * sizeof(struct maze_node));
  // struct maze_node *mazeStart = NULL;

  // int i = 0;
  // while ((c = fgetc(mazeFile)) != EOF) {
  //   if (c == CHAR_WALL) {
  //     mazeNodes[i].type = MAZE_WALL;
  //   } else if (c == CHAR_PASS) {
  //     mazeNodes[i].type = MAZE_PASS;
  //   } else if (c == CHAR_START) {
  //     mazeNodes[i].type = MAZE_START;
  //     mazeStart = &mazeNodes[i];
  //   } else if (c == CHAR_END) {
  //     mazeNodes[i].type = MAZE_END;
  //   }
  //   if (c != '\n') {
  //     i++;
  //   }
  // }

  // TODO: Remove when tests come
  // Check read correctly
  // for (int i = 0; i < mazeHeight * mazeWidth; i++) {
  //   printf(
  //     "%c",
  //     mazeNodes[i].type == MAZE_WALL      ? '#'
  //       : mazeNodes[i].type == MAZE_START ? '^'
  //       : mazeNodes[i].type == MAZE_END   ? 'E'
  //                                         : ' '
  //   );
  //   if ((i + 1) % mazeWidth == 0) {
  //     printf("\n");
  //   }
  // }


  // Process the maze to more usable format
  // struct maze_node *route;
  // int routeLength = solve(mazeNodes, mazeWidth, mazeHeight, &route);
  // if (routeLength == MAZE_ERROR_NO_START) {
  //   printf("No route start found on maze\n");
  //   return 1;
  // } else if (routeLength == MAZE_ERROR_NO_END) {
  //   printf("No route end(s) found on maze\n");
  //   return 1;
  // } else if (routeLength == MAZE_ERROR_NO_ROUTE) {
  //   printf("No route found on maze\n");
  //   return 1;
  // }

  // TODO: Display route

  fclose(mazeFile);
  return 0;
}
