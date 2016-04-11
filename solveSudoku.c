#include <math.h>
#include "sudoku.h"

int size , min_size ;
int possible[SIZE][SIZE][SIZE];
int solvesudoku(int,int ,int **);
int checkrow(int row,int num ,int ** sudoku)
{//This function checks whether we can put the number(num) in the row(row) of the Sudoku or not
int column;
for(column=0;column<size;column++)
    if(sudoku[row][column]==num)
        return 0 ;//If the number is found already present at certain location we return zero
return 1;//If the number is not found anywhere we return 1
}
int checkcolumn(int column,int num , int ** sudoku)
{//This function checks whether we can put the number(num) in the column(column) of the Sudoku or not
int row;
for(row=0;row<size;row++)
    if(sudoku[row][column]==num)
        return 0;//If the number is found already present at certain location we return zero
return 1;//If the number is not found anywhere we return 1
}
int checkgrid(int row,int column,int num , int ** sudoku)
{//This function checks whether we can put the number(num) in the 3*3 grid or not
//We get the starting row and column for the 3*3 grid
row=(row/min_size)*min_size ;
column=(column/min_size)*min_size;
int r,c;
for(r=0;r<min_size;r++)
    for(c=0;c<min_size;c++)
         if(sudoku[row+r][column+c]==num)
            return 0;//If the number is found already present at certain location we return zero
return 1;//If the number is not found anywhere we return 1
}
int navigate(int row,int column , int ** sudoku)
{//Function to move to the next cell in case we have filled one cell
	int ans ;
	if(column<size -1 )
       return solvesudoku(row,column+1 , sudoku);
	else
       return solvesudoku(row+1,0,sudoku);

}
// void display()
// {//The function to display the solved Sudoku
// int row,column;
// printf("THE SOLVED SUDOKU \n");
// 	for(row=0;row<size;row++)
// 	{
// 		for(column=0;column<size;column++)
// 		printf("%d ",final_sudoku[row][column]);
// 		printf("\n");
// 	}
// // getch();
// }
int solvesudoku(int row,int column ,int ** sudoku)
{
	// printf("!");
	if(row> size -1 ){//If the row number is greater than 8 than we have filled all cells hence we have solved the sudoku
    	// int i,j;
    	// for(i=0; i<size; ++i)
    	// 	for(j=0; j<size; ++j)
    	// 		final_sudoku[i][j] = sudoku[i][j];
    	// display();

    	return 1;
	}
    if(sudoku[row][column]!=0)
    	return navigate(row,column,sudoku);//If the value filled at a cell is not zero than it is filled with some value from 0 to 9 hence we move further
    else
    {
        int ctr;//This is a counter to check numbers from 1 to 9 whether the number can be filled in the cell or not
        for(ctr=1;ctr<=size;ctr++)
        {//We check row,column and the grid
        if(possible[row][column][ctr-1])
        {
        	if((checkrow(row,ctr,sudoku)==1)&&(checkcolumn(column,ctr,sudoku)==1)&&(checkgrid(row,column,ctr,sudoku)==1)){
        		sudoku[row][column]=ctr;
             	if(navigate(row,column,sudoku))
             		return 1;	
        	}
            
        }
        }
        sudoku[row][column]=0;//No valid number was found so we clean up and return to the caller.
    }
    return 0 ;
    
}
int hur_eliminate(int ** sudoku){
	int i , j , k , ret;
	ret = 0 ; 
	for(i = 0 ; i < size; ++i){
		for(j=0 ; j < size ; ++j){
			int count = 0 ;
			int ind = -1;
			for(k =1 ; k <=size ; ++k ){
				if(sudoku[i][j] == 0){
					if((checkrow(i,k,sudoku)==1)&&(checkcolumn(j,k,sudoku)==1)&&(checkgrid(i,j,k,sudoku)==1)){
						possible[i][j][k-1] = 1;
						// row_count[i][k-1]++;
						// column_count[j][k-1]++;
						// grid_count[(i/min_size)*min_size + (j/min_size)][k-1]++; 
						count ++;
						ind = k; 
					}
				}
			}
			if(count ==1 ){
				sudoku[i][j] = ind ;
				possible[i][j][ind-1] = 0 ;
				ret++;
			}
		}
	}
	return ret;
}
int lone_check(int r, int c , int v , int ** sudoku){
	int i,j,ret;
	ret = 0 ; 
	for(i = 0 ;i < size ; i++){
		if(possible[r][i][v] == 1) 
			ret++;
	}
	if(ret == 1){
		sudoku[r][c] = v+1 ;
		possible[r][c][v] = 0 ; 
		return 1;  
	}
	ret = 0; 
	for(i = 0 ;i < size ; i++){
		if(possible[i][c][v] == 1) 
			ret++;
	}
	if(ret == 1){
		sudoku[r][c] = v+1 ;
		possible[r][c][v] = 0 ; 
		return 1;  
	}
	ret = 0; 
	for(i = (r/min_size)*min_size ;i < (r/min_size)*min_size+ min_size ; i++){
		for(j = (c/min_size)*min_size ;j < (c/min_size)*min_size + min_size ; j++){
			if(possible[i][j][v] == 1) 
				ret++;
		}
	}
	if(ret == 1){
		sudoku[r][c] = v+1 ;
		possible[r][c][v] = 0 ; 
		return 1;  
	}
	return 0 ; 
		 
}
int hur_lone_ranger(int ** sudoku){
	int i , j , k , ret;
	ret = 0; 	
	for(i = 0 ; i < size; i++){
		for(j = 0 ; j<size ; j++){
			if(sudoku[i][j] == 0){
				for(k =0 ;k < size; k++){
					if(lone_check(i,j,k ,sudoku) == 1 ){
						sudoku[i][j] = k+1; 
						possible[i][j][k] =0 ;
						ret++; 
						break;
					}	
				}
			}
		}
	}
	return ret;
}

int ** solveSudoku(int ** mygrid){
	size = SIZE;
	min_size = MINIGRIDSIZE;
	
	printf("size: %d\n" , SIZE);
	
	int i , j , k;
	for (i = 0; i < size; ++i)
		for(j = 0 ; j < size; ++j)
			for(k=0 ;k < size; ++k)
				possible[i][j][k]= 0 ; 
	
	//lone 
	int val ;
	while((val = hur_eliminate(mygrid)) != 0 ){
		printf("elimnating..%d\n", val);
	}
	printf("done------------------------\n");

	// int val1 = hur_lone_ranger(mygrid);
	// while((val1 = hur_lone_ranger(mygrid)) != 0 ){
	// 	printf("lone ranger..%d\n", val1);
	// }
	// printf("done------------------------\n");

	// val = hur_eliminate(mygrid);
	// 	printf("elimnating..%d\n", val);
	// printf("done------------------------\n");
		
    // exit(0);    


	printf("%d\n",solvesudoku(0,0, mygrid));
	// display();
	return mygrid;
	
}