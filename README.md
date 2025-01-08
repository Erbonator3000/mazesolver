# Maze solver

Basic maze solving program using Dijkstra's algorithm.

## Building
```(bash)
cmake -S . -B build && cmake --build build
```

## Installing
```(bash)
cmake -S . -B build && cmake --build build && cmake --install
```

## Usage
```(bash)
solver [OPTION]... [FILE]
```
Following command line parameters are supported.
```(bash)
-h, --help      Print this help text
-l --limit      Set the limit how many steps to seach
```

### Limit
The search algorithm depth can be limited with the `l` otpion.
```(bash)
solver -l 10 mazefile.txt     # Only search exit 10 steps from start
```

### Maze file format
Mazes are presented as ascii .txt files. For example:

```
#######
^ #   #
# # # #
#   # E
#######
```

All the rows in the maze file must be of equal width.


Characters in the maze are represented as:
```
'#'               : Wall
' ' (empty space) : Passage, maze solving algorithm searches these for path
'^'               : Start, path finding algorithm starts from this space
'E'               : Exit, path finding algorithm triest to find path from start to closest exit
```

Program permits for multiple exit points to be present in the maze. The algorithm will find the shortest exit route. In case no
route is found within the given search steps, an error message is printed.

Only one starting point is permitted to be present in the maze.
