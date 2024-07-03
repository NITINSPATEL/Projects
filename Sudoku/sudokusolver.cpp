#include<iostream>
#include<cmath>
using namespace std;


bool isValid(int board[][9],int i , int j , int num, int n)
{
    // row and column check
    for(int x = 0 ; x< n ; x++)
    {
        if(board[x][j]==num || board[i][x]==num)
            return false;
    }

    // subgrid check
    int rn = sqrt(n);
    int si = i - (i%rn);
    int sj = j - (j%rn);
    
    for(int x = si ; x<si+rn ; x++)
    {
        for(int y = sj ; y < sj + rn ; y++)
        {
            if(board[x][y]==num)
                return false;
        }
    }

    return true;

}

bool sudoku_solver(int  board[][9],int i , int j,int &n)
{   // base case
    if(i==n)
        return true;
    

    // if we r not inside board 
    if(j==n)
    {
        return sudoku_solver(board,i+1,0,n);
    }

    // if cell is already filled -> 
    if(board[i][j]!=0)
    {
        return sudoku_solver(board,i,j+1,n);
    }

    // trying to fill cells in column with appropriate numbers , backtracking if necessary
    for(int num = 1 ; num<=n ; num++)
    {
        // checking if number can be filled 
        if(isValid(board,i,j,num,n))
        {
            board[i][j] = num ;
            bool subans = sudoku_solver(board,i,j+1,n);
            if(subans)
                return true;
        }
            board[i][j] = 0 ;

    }
    return false;
}


int main()
{
    int n = 9 ;
    int board[9][9] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    for(int i = 0 ; i<9 ;i++)
    {
        for(int j = 0 ; j<9 ; j++)
        {
            cout<<board[i][j]<<" ";
        }
        cout<<endl;
    }
       cout<<endl<<endl;
   cout<<sudoku_solver(board,0,0,n);
   cout<<endl<<endl;
       for(int i = 0 ; i<9 ;i++)
    {
        for(int j = 0 ; j<9 ; j++)
        {
            cout<<board[i][j]<<" ";
        }
        cout<<endl;
    }
}