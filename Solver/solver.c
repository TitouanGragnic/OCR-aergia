int valid(int matrix[][9], int row, int column, int guess) {
     /*
       Check if guess is valid on the matrix.
     */
     int corner_x = row / 3 * 3;
     int corner_y = column / 3 * 3;
     for (int x = 0; x < 9; ++x)
     {
	  if (matrix[row][x] == guess)
	       return 0;
	  if (matrix[x][column] == guess)
	       return 0;
	  if (matrix[corner_x + (x % 3)][corner_y + (x / 3)] == guess)
	       return 0;
     }
     return 1;
}

int find_empty_cell(int matrix[][9], int *row, int *column) {
     /*
       Find first empty cell.
     */
     for (int x = 0; x < 9; x++)
	  for (int y = 0; y < 9; y++)
	       if (!matrix[x][y])
	       {
		    *row = x;
		    *column = y;
		    return 1;
	       }
     return 0;
}

int solve(int matrix[][9]) {
    /*
      Solve the Sudoku with backtracking.
     */
    int row;
    int column;

    if(!find_empty_cell(matrix, &row, &column))
	 return 1;

    for (int guess = 1; guess < 10; guess++)
	if (valid(matrix, row, column, guess))
	{
	    matrix[row][column] = guess;
	    if(solve(matrix))
		 return 1;
	    matrix[row][column] = 0;
	}
    return 0;
}
