#include "solver.h"

int valid(int** matrix, int row, int column, int guess, int width) {
    /*
      Check if guess is valid on the matrix.
    */
    int square = sqrt(width);
    int corner_x = row / square * square;
    int corner_y = column / square * square ;
    for (int x = 0; x < width; ++x)
    {
        if (matrix[row][x] == guess)
            return 0;
        if (matrix[x][column] == guess)
            return 0;
        if (matrix[corner_x + (x % square)][corner_y + (x / square)] == guess)
            return 0;
    }
    return 1;
}

int find_empty_cell(int** matrix, int *row, int *column, int width)
{
    /*
      Find first empty cell.
    */
    for (int x = 0; x < width; x++)
        for (int y = 0; y < width; y++)
            if (!matrix[x][y])
            {
                *row = x;
                *column = y;
                return 1;
            }
    return 0;
}

int solve(int** matrix, int width) {
    /*
      Solve the Sudoku with backtracking.
    */
    int row;
    int column;

    if(!find_empty_cell(matrix, &row, &column, width))
        return 1;

    for (int guess = 1; guess <= width; guess++)
	if (valid(matrix, row, column, guess, width))
	{
	    matrix[row][column] = guess;
	    if(solve(matrix, width))
                return 1;
	    matrix[row][column] = 0;
	}
    return 0;
}
