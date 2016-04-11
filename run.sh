clear
clear
rm a.out
gcc -fopenmp -O3 -g -w main.c sudoku.c
# ./a.out 4 Sudoku_Testcases/16x16_board/sudoku16_input5
# ./a.out 4 Sudoku_Testcases/9x9_board/sudoku9_input5