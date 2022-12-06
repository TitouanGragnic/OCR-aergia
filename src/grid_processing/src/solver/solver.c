#include "../../include/solver/solver.h"

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

int is_valid(int** matrix, int width) {
    /*
      Check if the matrix is valid.
    */
    for (int i = 0; i < width; i++)
    {
        int test1[width+1];
        int test2[width+1];

        for (int k = 0; k<=width; k++)
        {
            test1[k] = 0;
            test2[k] = 0;
        }

        for(int j = 0; j < width; j++)
        {
            if(test1[matrix[i][j]] || test2[matrix[j][i]])
                return 0;
            test1[matrix[i][j]] = matrix[i][j]!=0;
            test2[matrix[j][i]] = matrix[j][i]!=0;
        }
    }
    int s = sqrt(width);
    for (int i = 0; i<width; i+=s)
        for (int j = 0; j<width; j+=s)
        {
            int test[width+1];
            for (int k = 0; k<=width; k++)
                test[k] = 0;
            for (int x = 0; x<s; x++)
                for (int y = 0; y<s; y++)
                {
                    if(test[matrix[i+x][j+y]])
                        return 0;
                    test[matrix[i+x][j+y]] = matrix[i+x][j+y]!=0;
                }

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
    if (!is_valid(matrix, width))
        return 0;
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
