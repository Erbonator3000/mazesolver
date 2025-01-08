/* Copyright 2025 Eero Prittinen */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <getopt.h>
#include <errno.h>
#include "maze.h"

static struct option long_options[] = {
  {"help", no_argument, 0, 'h'},
  {"limit", required_argument, 0, 'l'},
  {0, 0, 0, 0} // Sentinel to mark the end of the array
};

static long int searchLimit = INT_MAX;

__attribute__((noreturn)) static void usage(int status) {
  if (status != EXIT_SUCCESS) {
    fprintf(stderr, "Usage: %s [OPTION]... [FILE]\n", getprogname());
    fprintf(stderr, "Try '%s --help' for more information.\n", getprogname());
  } else {
    printf("Usage: %s [OPTION]... [FILE]\n", getprogname());
    printf("Try '%s --help' for more information.\n", getprogname());
    printf("\
  -h, --help      Print this help text\n\
  -l --limit      Set the limit how many steps to seach\n");
  }
  fprintf(stderr, "\n");
  exit(status);
}

int main(int argc, char **argv) {
  int opt;
  int option_index = 0;
  while ((opt = getopt_long(argc, argv, "hl:", long_options, &option_index)) != -1) {
    switch (opt) {
      case 'h':
        usage(EXIT_SUCCESS);
        return EXIT_SUCCESS;
      case 'l': {
        char *endptr;
        errno = 0;
        searchLimit = strtol(optarg, &endptr, 10);
        if (errno != 0 || endptr == optarg || *endptr != '\0') {
          usage(EXIT_FAILURE);
        }
        break;
      }
      default:
        usage(EXIT_FAILURE);
        return EXIT_FAILURE;
    }
  }

  if (argc - optind != 1) {
    usage(EXIT_FAILURE);
  }
  char *mazeFileName = argv[argc - 1];
  FILE *mazeFile = fopen(mazeFileName, "r");
  if (mazeFile == NULL) {
    fprintf(stderr, "Could not open maze %s\n", mazeFileName);
    return EXIT_FAILURE;
  }

  maze_solver_handle_t mazeHandle;
  mazeSolver_init(&mazeHandle);

  int ret = mazeSolver_loadMazeFromFile(&mazeHandle, mazeFile);
  if (ret < 0) {
    fprintf(stderr, MAZE_ERROR(ret));
    mazeSolver_deinit(&mazeHandle);
    fclose(mazeFile);
    return EXIT_FAILURE;
  }

  ret = mazeSolver_solve(&mazeHandle, (int) searchLimit);

  if (ret < 0) {
    printf(MAZE_ERROR(ret));
    mazeSolver_deinit(&mazeHandle);
    fclose(mazeFile);
    return EXIT_FAILURE;
  }

  ret = mazeSolver_print(&mazeHandle, stdout);
  mazeSolver_deinit(&mazeHandle);
  fclose(mazeFile);

  if (ret < 0) {
    printf(MAZE_ERROR(ret));
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
