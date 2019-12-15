bool solveNQUtil(int board[N][N], int col)
{
    //if we have filled all the columns already.
    if(col >= N)
        return true;

    //iterate over each row in column "col"
    for(int i = 0; i < N; i++)
    {
        /*
         * Place the queen at the corresponding position
         * if it is safe, by updating the matrix with 1.
         */
        if(isSafe(board, i, col))
        {
            board[i][col] = 1;
            /*
             * Check if there is a possible arrangement
             * for the current position of 
             * the queen in the column "col".
             */
            if(solveNQUtil(board, col + 1))
            {
                return true;
            }
            /*
             * if no possible arrangement is found,
             * then backtrace and remove the queen.
             */
            board[i][col] = 0;
        }
    }
    return false;
}
