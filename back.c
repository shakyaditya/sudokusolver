int size ;
int subgrid_size ;
int ** grid;


int row_check(int row,int val ){
	int col;
	for(col=0;col<size;col++)
	    if(grid[row][col]==val)
	        return 0 ;
	return 1;
}
int column_check(int col,int val ){
	int row;
	for(row=0;row<size;row++)
	    if(grid[row][col]==val)
	        return 0;
	return 1;
}
int grid_check(int row,int col,int val ){

	row=(row/subgrid_size)*subgrid_size ;
	col=(col/subgrid_size)*subgrid_size;
	int r,c;
	for(r=0;r<subgrid_size;r++)
	    for(c=0;c<subgrid_size;c++)
	         if(grid[row+r][col+c]==val)
	            return 0;
	return 1;
}
void display()
{//The function to display the solved Sudoku
int row,column;
printf("THE SOLVED SUDOKU \n");
for(row=0;row<9;row++)
{
for(column=0;column<9;column++)
printf("%d ",grid[row][column]);
printf("\n");
}
// getch();
}
void fill_cell(int ** grid , int row , int col){
	printf("cell:%dx%d\n",row,col);
	if(row > size-1){
		display();
	}
	if(grid[row][col]!=0){
		printf("cell filled :%dx%d\n",row,col);
		fill_cell(grid,row+(col+1)/size,(col+1)%size);	
	}
	else{
		int fill_val ;
		printf("cell looking :%dx%d\n",row,col);
		for (fill_val = 0; fill_val < size; fill_val++)
		{
			int r =  row_check(row , fill_val );
			int c =  column_check(col , fill_val );
			int g = grid_check(row,col,fill_val);
			if((r+c+g)/3 == 1){
				printf("filling :%dx%d - %d\n",row,col);
				grid[row][col]=fill_val;
				fill_cell(grid,row+(col+1)/size,(col+1)%size);	
			}
		}
		grid[row][col]=0;
		printf("rollback\n");
	}
}
int ** solveSudoku(int ** mygrid){
	grid = mygrid;
	size = sizeof(grid) +1 ;
	subgrid_size =  1; 
	int k = 2 ;
	int sz = size; 
	while(1){
		if(sz==1){
			break;
		}
		if(sz%k == 0 ){
			sz = sz /(k*k);
			subgrid_size = subgrid_size*k ; 
		}
		else{
			k = k+1;
		}
	}
	printf("size:%dx%d\n", size , size );	
	fill_cell(grid,0,0);
	return grid;
}