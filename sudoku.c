/*2012CS10210_2012CS10221*/
#include <math.h>
#include "sudoku.h"
#include <omp.h>

int size , min_size ;
// int possible[SIZE][SIZE][SIZE];
int** final_output;
int solvesudoku(int,int ,int **);
int*** apply_huer(int ** sudoku);
int checkrow(int row,int num ,int ** sudoku){
	int column;
	// #pragma omp prallel for
	for(column=0;column<size;column++)
	    if(sudoku[row][column]==num)
	        return 0 ;
	return 1;
}
int checkcolumn(int column,int num , int ** sudoku){
	int row;
	// #pragma omp prallel for
	for(row=0;row<size;row++)
	    if(sudoku[row][column]==num)
	        return 0;
	return 1;
}
int checkgrid(int row,int column,int num , int ** sudoku){
	row=(row/min_size)*min_size ;
	column=(column/min_size)*min_size;
	int r,c;
	// #pragma omp prallel for
	for(r=0;r<min_size;r++)
	    for(c=0;c<min_size;c++)
	         if(sudoku[row+r][column+c]==num)
	            return 0;
	return 1;
}
int navigate(int row,int column , int ** sudoku)
{//Function to move to the next cell in case we have filled one cell
	int ans ;
	// if(size ==9 ){
	// 	if(column<size -1 )
	//        return solve9x9sudoku(row,column+1 , sudoku);
	// 	else
	//        return solve9x9sudoku(row+1,0,sudoku);	
	// }
	// else{
		if(column<size -1 )
	       return solvesudoku(row,column+1 , sudoku);
		else
	       return solvesudoku(row+1,0,sudoku);			
	// }

}
void display(int ** sudoku){
	int row,column;
	printf("THE SOLVED SUDOKU \n");
	for(row=0;row<size;row++)
	{
		for(column=0;column<size;column++)
		printf("%d ",sudoku[row][column]);
		printf("\n");
	}
// getch();
}
// int solve9x9sudoku(int row,int column ,int ** sudoku)
// {
// 	// printf("!");
// 	if(row> size -1 ){//If the row number is greater than 8 than we have filled all cells hence we have solved the sudoku
//     	// int i,j;
//     	// for(i=0; i<size; ++i)
//     	// 	for(j=0; j<size; ++j)
//     	// 		final_sudoku[i][j] = sudoku[i][j];
//     	// display();

//     	return 1;
// 	}
//     if(sudoku[row][column]!=0)
//     	return navigate(row,column,sudoku);//If the value filled at a cell is not zero than it is filled with some value from 0 to 9 hence we move further
//     else
//     {
//         int ctr;//This is a counter to check numbers from 1 to 9 whether the number can be filled in the cell or not
//         for(ctr=1;ctr<=size;ctr++)
//         {//We check row,column and the grid
//         if(possible[row][column][ctr-1])
//         {
//         	if((checkrow(row,ctr,sudoku)==1)&&(checkcolumn(column,ctr,sudoku)==1)&&(checkgrid(row,column,ctr,sudoku)==1)){
//         		sudoku[row][column]=ctr;
//              	if(navigate(row,column,sudoku))
//              		return 1;	
//         	}
            
//         }
//         }
//         sudoku[row][column]=0;//No valid number was found so we clean up and return to the caller.
//     }
//     return 0 ;
    
// }
int flag =  1 ;
int solvesudoku(int row,int column ,int ** sudoku){
	// printf("!");
	if(row> size -1 ){
		//If the row number is greater than 8 than we have filled all cells hence we have solved the sudoku
    	// display(sudoku);
    	flag = 0;
    	int i,j;
    	for (i = 0; i < size; ++i)
    	{
    		for ( j = 0; j < size; ++j)
    		{
    			final_output[i][j] = sudoku[i][j];
    		}
    	}

    	return 1;
	}
	if(flag ==0){
		return 1;
	}
    if(sudoku[row][column]!=0)
    	return navigate(row,column,sudoku);//If the value filled at a cell is not zero than it is filled with some value from 0 to 9 hence we move further
    else
    {
    	// printf("reached here\n");
        int ctr,i,j;//This is a counter to check numbers from 1 to 9 whether the number can be filled in the cell or not
        int *** possible = apply_huer(sudoku);
         
        #pragma omp parallel for
        	for(ctr=1;ctr<=size*flag ;ctr++)
	        { 	
	        	if(flag == 0){
	        		// break;
	        	}
	        	else{
		        	if(sudoku[row][column] == 0 ){
				        if( possible[row][column][ctr-1])
				        {
				        	if((checkrow(row,ctr,sudoku)==1)&&(checkcolumn(column,ctr,sudoku)==1)&&(checkgrid(row,column,ctr,sudoku)==1)){
	        		        	int ** sudoku_new = malloc(sizeof(int*) * size); 
					        	int x ,y ; 
					        	for (x = 0 ; x < size ; x++ )
					        		sudoku_new[x] = malloc(size*sizeof(int));
								for (x = 0 ; x < size ; x++ )
					        		for(y= 0 ; y<size;y++)
					        			sudoku_new[x][y] = sudoku[x][y];


				        		sudoku_new[row][column]=ctr;
				             	if(navigate(row,column,sudoku_new)){
				             		
				             	}
				             	free(sudoku_new);	
				        	}		            
				        }
				    }
				    else{
				    	// #pragma omp critical
				    		flag =  !navigate(row,column ,sudoku);				    	
				    	
				    }			        	
	        	}        
	        }

    	// }
	    free(possible);
	    return !flag; 
    } 
}
int hur_eliminate(int ** sudoku , int*** possible){
	int i , j , k , ret;
	ret = 0 ;
	// #pragma omp prallel for  
	for(i = 0 ; i < size; ++i){
		for(j=0 ; j < size ; ++j){
			int count = 0 ;
			int ind = -1;
			if(sudoku[i][j] == 0){		
				for(k =1 ; k <=size ; ++k ){
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
				// printf("%d %d %d\n",i,j,ind );
				possible[i][j][ind-1] = 0 ;
				// //#pragma omp critical
					ret++;
				
			}
		}
	}
	return ret;
}
int lone_check(int r, int c , int v , int ** sudoku , int*** possible){
	int i,j,ret,flag;
	ret = 0 ;
	flag  = 0;  

	//#pragma omp prallel for  
	for(i = 0 ;i < size ; i++){
		if(sudoku[r][i] == v +1) 
			flag = 1;
	}
	//#pragma omp prallel for  
	for(i = 0 ;i < size ; i++){
		if(possible[r][i][v] == 1) 
			ret++;
	}
	if(ret == 1 && !flag){
	// if(r == 1 && c == 1){
	// 	printf("type :%d\n", 1);
	// }
		return 1;  
	}
	ret = 0; 
	flag = 0;
	//#pragma omp prallel for  	
	for(i = 0 ;i < size ; i++){
		if(sudoku[i][c] == v +1) 
			flag = 1;
	}

	//#pragma omp prallel for  
	for(i = 0 ;i < size ; i++){
		if(possible[i][c][v] == 1) 
			ret++;
	}
	if(ret == 1 && !flag){
		// if(r == 1 && c == 1){
		// printf("type :%d\n", 2);
		// }	

		return 1;  
	}
	ret = 0; 
	flag = 0;
	
	//#pragma omp prallel for  
	for(i = (r/min_size)*min_size ;i < (r/min_size)*min_size+ min_size ; i++){
		for(j = (c/min_size)*min_size ;j < (c/min_size)*min_size + min_size ; j++){
			if(sudoku[i][j] == v+1) 
				flag = 1;
		}
	}

	//#pragma omp prallel for  
	for(i = (r/min_size)*min_size ;i < (r/min_size)*min_size+ min_size ; i++){
		for(j = (c/min_size)*min_size ;j < (c/min_size)*min_size + min_size ; j++){
			if(possible[i][j][v] == 1) 
				ret++;
		}
	}
	if(ret == 1 && !flag){
		// if(r == 1 && c == 1){
		// printf("type :%d\n", 3);
		// }
		return 1;  
	}
	return 0 ; 
		 
}
int hur_lone_ranger(int ** sudoku, int*** possible){
	int i , j , k , ret;
	ret = 0; 	
	// #pragma omp prallel for  
	for(i = 0 ; i < size; i++){
		for(j = 0 ; j<size ; j++){
			if(sudoku[i][j] == 0){
				for(k =0 ;k < size; k++){
					if(possible[i][j][k]==1){
						// if(i==1 && j == 1){
						// 		printf("asdasd : %d\n", k);
						// 		printf("lone check %d\n", lone_check(i,j,k ,sudoku));
						// }
						if(lone_check(i,j,k ,sudoku, possible) == 1 ){
							sudoku[i][j] = k+1; 
							possible[i][j][k] =0 ;
							ret++; 
							break;
						}	
					}
				}
			}
		}
	}
	return ret;
}
int hur_twin(int ** sudoku , int *** possible){
	int i , j , k , change;
	change =0; 
	//#pragma omp prallel for  
	for(i =0 ; i < size; i ++){
		int l , m, n ;				 
		for(l = 0 ; l< size; l++){
			n = 0 ;
			for(m = 0 ; m < size ; m++){
				if( sudoku[i][m]==0 && possible[i][m][l] == 1)
					n++;
			}
			if(n==2){
				int x, y , z ;
				for (x =l+1 ; x < size ; x++){
					z = 0 ; 
					for(y = 0 ; y < size ; y++){
						if( sudoku[i][y]==0 && possible[i][y][x] == 1)
							z++;						
					}
					if(z == 2){
						int itr,cnt,y1,y2;
						cnt =0;
						for(itr =0;itr<size;itr++){
							if(sudoku[i][itr]==0 && possible[i][itr][l]==1){
								if(possible[i][itr][x]==1){
									cnt++;
								}
								else{
									break;
								}

								if(cnt==1){
									y1 = itr;
								}
								else if(cnt==2){
									y2 = itr;
								}
								
							}
						}
						if(cnt==2){
							int tmp;
							for(tmp = 0;tmp<size;tmp++){
								if(tmp !=l && tmp !=x){
									possible[i][y1][tmp]=0;
									possible[i][y2][tmp]=0;
									change++;
									change++;
								}
							}
						
						}
					}	
				}
			}	
		}
	}
	// twin for columns ----------
	// #pragma omp prallel for  
	for(i =0 ; i < size; i ++){
		int l , m, n ;				 
		for(l = 0 ; l< size; l++){
			n = 0 ;
			for(m = 0 ; m < size ; m++){
				if( sudoku[m][i]==0 && possible[m][i][l] == 1)
					n++;
			}
			if(n==2){
				int x, y , z ;
				for (x =l+1 ; x < size ; x++){
					z = 0 ; 
					for(y = 0 ; y < size ; y++){
						if( sudoku[y][i]==0 && possible[y][i][x] == 1)
							z++;						
					}
					if(z == 2){
						int itr,cnt,y1,y2;
						cnt =0;
						for(itr =0;itr<size;itr++){
							if(sudoku[itr][i]==0 && possible[itr][i][l]==1){
								if(possible[itr][i][x]==1){
									cnt++;
								}
								else{
									break;
								}

								if(cnt==1){
									y1 = itr;
								}
								else if(cnt==2){
									y2 = itr;
								}
								
							}
						}
						if(cnt==2){
							int tmp;
							for(tmp = 0;tmp<size;tmp++){
								if(tmp !=l && tmp !=x){
									possible[y1][i][tmp]=0;
									possible[y1][i][tmp]=0;
									change++;
									change++;
								}
							}
						
						}
					}	
				}
			}	
		}
	}

	// twin for grid ----------
	for(i =0 ; i < size; i ++){
		int l , m, n , p ;				 
		for(l = 0 ; l< size; l++){
			n = 0 ;
			for(m = (i/min_size)*min_size ; m < (i/min_size)*min_size + min_size; m++){
				for(p = (i%min_size)*min_size ;p < (i%min_size)*min_size + min_size; p++){
					if( sudoku[m][p]==0 && possible[m][p][l] == 1)
						n++;
				}
			}
			if(n==2){
				int x, y , z ,w;
				for (x =l+1 ; x < size ; x++){
					z = 0 ; 
					for(y = (i/min_size)*min_size ; y < (i/min_size)*min_size + min_size; y++){
						for(w = (i%min_size)*min_size ;w < (i%min_size)*min_size + min_size; w++){
							if( sudoku[y][w]==0 && possible[y][w][x] == 1)
								z++;
						}
					}
					if(z == 2){
						int itr,itr1,cnt,y1,y2,x1,x2;
						cnt =0;
						for(itr = (i/min_size)*min_size ; itr < (i/min_size)*min_size + min_size; itr++){
							for(itr1 = (i%min_size)*min_size ;itr1 < (i%min_size)*min_size + min_size; itr1++){
								if(sudoku[itr][itr1]==0 && possible[itr][itr1][l]==1){
									if(possible[itr][itr1][x]==1){
										cnt++;
									}
									else{
										break;
									}

									if(cnt==1){
										x1 = itr;
										y1 = itr;
									}
									else if(cnt==2){
										x1 = itr;
										y1 = itr1;
									}
									
								}
							}
						}
						if(cnt==2){
							int tmp;
							for(tmp = 0;tmp<size;tmp++){
								if(tmp !=l && tmp !=x){
									possible[x1][y1][tmp]=0;
									possible[x2][y2][tmp]=0;
									change++;
									change++;
								}
							}
						
						}
					}	
				}
			}	
		}
	}

	return change;	
}
int *** apply_huer(int ** sudoku){
	int i , j , k;
	//#pragma omp prallel for  
	int *** possible = (malloc(sizeof(int **)*size));
	for (i = 0; i < size; ++i)
		possible[i]= malloc(sizeof(int*)*size) ;

	for (i = 0; i < size; ++i)
		for(j=0;j<size;j++)
			possible[i][j]= malloc(sizeof(int)*size) ;

	for (i = 0; i < size; ++i)
		for(j=0;j<size;j++)
			for(k =0 ; k < size ; k++)
				possible[i][j][k]= 0 ;

	int val ;

	while((val = hur_eliminate(sudoku, possible)) != 0 ){
		// printf("elimnating..%d\n", val);
	}
	// printf("done------------------------\n");
	int val1;
	while((val1 = hur_lone_ranger(sudoku,possible)) != 0 ){
		// printf("lone ranger..%d\n", val1);
	}
	val1 = hur_twin(sudoku,possible);
	val = hur_eliminate(sudoku,possible);
	// while( (val1 = hur_twin(sudoku)) != 0 ){
	// 	// printf("twjn found...%d\n", val1);
	// }
	// printf("done------------------------\n");
	return possible;

}

int ** solveSudoku(int ** mygrid){
	size = SIZE;
	int i;
	min_size = MINIGRIDSIZE;
	final_output = malloc(sizeof(int*)*size);
	for ( i = 0; i < size; ++i)
	{
		final_output[i] = malloc(sizeof(int)*size);
	}
	// printf("size: %d\n" , SIZE);
	
		
	// for(i =0  ; i< size; i++){
	// 	printf("%d ",possible[1][1][i] );
	// }
	// printf("\n");
	// display(mygrid);
	// int val1 = hur_lone_ranger(mygrid);
	// printf("done------------------------%d\n",val1);
	
	// display(mygrid);
	// 	int val2 = hur_twin(mygrid);
	// 	printf("twin..%d\n", val2);
	// val = hur_eliminate(mygrid);
	// 	printf("elimnating..%d\n", val);
	// printf("done------------------------\n");
	// display(mygrid);
		
    // exit(0);    


	// if(size == 9 ){
	// 	apply_huer(mygrid);
	// int f = solve9x9sudoku(0,0, mygrid);		
	// }
	// else{
	int f = solvesudoku(0,0, mygrid);		
	// }
	// display();
	return final_output;
	
}