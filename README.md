# Operating Systems – 2022/2023

## 1. Goal

To parallelize a simulation of the Game of Life using the Process model, together with System V Shared Memory for data sharing and System V Semaphores for synchronization. The work is to be developed and executed in a Linux system.

## 2. Introduction

"The Game of Life [...] is a cellular automaton devised by the British mathematician John Horton Conway in 1970. It is a zero-player game, meaning that its evolution is determined by its initial state, requiring no further input. One interacts with the Game of Life by creating an initial configuration and observing how it evolves. The universe of the Game of Life is an infinite, 2D orthogonal grid of square cells, each of which is in one of two possible states: live or dead [...]. Every cell interacts with its eight neighbors, which are the cells that are horizontally, vertically, or diagonally adjacent. At each step in time, the following transitions occur:

1. Any live cell with two or three live neighbors survives;
2. Any dead cell with three live neighbors becomes a live cell;
3. All other live cells die in the next generation. Similarly, all other dead cells stay dead.

The initial pattern constitutes the seed of the system. The first generation is created by applying the above rules simultaneously to every cell in the seed, live or dead; births and deaths occur simultaneously [...]. Each generation is a function of the preceding one. The rules are applied repeatedly to create further generations." [1]

## 3. Details

In this work, you are required to implement the Game of Life in the C language. But, before that, start by understanding the problem (read the Wikipedia page [1]).

Use the supplied file `single-process-game.c` as a starting point. This file already includes some `.h` header files (you may need more), defines characters to represent dead or alive cells, defines constants to identify a specific or a random pattern, defines constants that are useful when using a random pattern, defines global variables for the number of lines and columns of the 2D grid, defines global pointers for the current generation and the next generation grids, and defines the maximum number (NITERS) of iterations for the game (in case the game does not first converge).

The same file also includes several empty functions to be filled (see comments) and one `clearScreen()` function already fully implemented. The latter is to be used before showing the next generation of cells on the screen. In order to be able to use `clearScreen()`, you must compile the program with `-lncurses`, e.g., `gcc single-process-game.c -o single-process-game.exe -lncurses`. Note: you may need to install the ncurses library (`sudo apt-get install libncurses-dev`).

### 3.1 Single Process Version

Develop a sequential (single process) solution, where the main concern is to correctly implement the Game of Life algorithm, using the simple Oscillator patterns [1]. Basically, you must provide the missing code that implements the behavior described in the various comments scattered in the provided `single-process-game.c` file.

This first version should already be able to receive from the command line the kind of initial pattern to be used, with 0 to 4 meaning a certain Oscillator pattern, and 5 meaning a random pattern. Example of usage: `single-process-game.exe 0`.

Regarding the simple Oscillator patterns: they may be easily fitted in 2D arrays (grids); but, in the C language, 2D arrays have a

 1D arrangement in memory (one line after the other) and so you may simply use 1D arrays (`char *`) to store the grids, provided you do the correct math to access a cell at a specific line and column.

Besides supporting the simple Oscillator patterns, your solution must also support an initial RANDOM pattern. In this case, the number of lines and columns are defined by the constants `NLINES_RANDOM` and `NCOLUMNS_RANDOM`, whose values (50 and 200) are appropriate to execute the game in a maximized terminal (but you may change them). Also, when randomly generating the initial pattern, you must ensure that the initial number of alive cells is (approximately) 10% of the total number of cells.

### 3.2 Multi Process Version

Once you have developed the previous Single Process Version, and that version is ensured to be working correctly, you should derive from it a new version where several processes divide among them the work performed in the `gridEvolve` function. This new version will be placed in a file named `multi-process-game.c`.

Basically, you will now have a parent process and a number of `NWORKERS` children (`NWORKERS` is a constant whose value you may change in the program to define its degree of parallelism). It will be up to the parent process to show on the screen the initial grid and each of the newly generated grids. It is up to the children to generate the next grid based on the previous one. So, you have to carefully synchronize the parent and the children: the parent must wait for the children to end producing a generation before it can show it on the screen; the children should only start producing the next generation after the parent has shown the previous one. This synchronization must be implemented with System V Semaphores.

Also, the work involved in generating the next grid must be divided among the children in the most possible balanced way. Basically, the grids must be split into `NWORKERS` slices (i.e., one slice per child process) of equal dimensions (in case the grids are not evenly divisible by the children, just choose a specific child to handle the remainder of the division). To share the grids (and other global data structures) among all the processes involved, you must use System V Shared Memory.

Compare the results of the Multi Process Version with those produced by the Single Process Version. They should be the same for the same initial conditions (note that to compare the results of the RANDOM pattern, you must ensure the same seed parameter is passed to the function that initializes the random number generator).

## 4. References

[1] [https://en.wikipedia.org/wiki/Conway%27s\_Game\_of\_Life](https://en.wikipedia.org/wiki/Conway%27

s_Game_of_Life)
