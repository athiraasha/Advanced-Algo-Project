#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int solve_sudoku(int sudoku[9][9]);
int find_empty_location(int sudoku[9][9],int *row,int* col);
int is_safe_location(int sudoku[9][9],int row,int col,int num);
int used_in_row(int sudoku[9][9],int row,int num);
int used_in_column(int sudoku[9][9],int col,int num);
int used_in_box(int sudoku[9][9],int row,int col,int num);
void print_sudoku(int sudoku[9][9]);
int c=0;
struct timespec timer_start(){
	struct timespec start_time;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
	return start_time;
}

// call this function to end a timer, returning nanoseconds elapsed as a long
long timer_end(struct timespec start_time){
	struct timespec end_time;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);
	long diffInNanos = (end_time.tv_sec - start_time.tv_sec) * (long)1e9 + (end_time.tv_nsec - start_time.tv_nsec);
	return diffInNanos;
}
int main()
{
	struct timespec vartime = timer_start();
	int n=9;
	int sudoku[9][9]= {{2,5,0,9,0,0,0,0,0},{6,0,0,4,3,2,0,0,5},{9,0,0,6,5,8,0,2,7},{0,2,0,0,0,0,3,4,0},{0,0,0,0,0,0,0,0,0},{8,0,1,0,4,0,0,5,0},{0,7,9,8,0,0,1,0,0},{0,0,0,0,7,0,5,0,0},{5,0,0,0,2,0,7,0,0}};
	/*for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			scanf("%d",&sudoku[i][j]);
		}
	}
	printf("\n");*/
	if(solve_sudoku(sudoku))
		print_sudoku(sudoku);
	else
		printf("no solution exists\n");
	long time_elapsed_nanos = timer_end(vartime);
    printf("Time taken - %ld\n", time_elapsed_nanos);
    //printf("c=%d\n",c);
    return 0;
}

int solve_sudoku(int sudoku[9][9])
{
	c+=1;
	int row=0;
	int col=0;
	if(!find_empty_location(sudoku,&row,&col))
		return 1;
	for(int num=1;num<10;num++)
	{
		if(is_safe_location(sudoku,row,col,num))
		{
			sudoku[row][col]=num;
			if(solve_sudoku(sudoku))
				return 1;
			sudoku[row][col]=0;
		}
	}
	return 0;		
}

int find_empty_location(int sudoku[9][9],int *row,int* col)
{
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			if(sudoku[i][j]==0)
			{
				*row=i;
				*col=j;
				return 1;
			}
		}
	}
	return 0;
}

int is_safe_location(int sudoku[9][9],int row,int col,int num)
{
	return (!used_in_row(sudoku,row,num)) && (!used_in_column(sudoku,col,num)) && (!used_in_box(sudoku,row-row%3,col-col%3,num));
}

int used_in_row(int sudoku[9][9],int row,int num)
{
	for(int i=0;i<9;i++)
	{
		if(sudoku[row][i]==num)
			return 1;
	}
	return 0;
}

int used_in_column(int sudoku[9][9],int col,int num)
{
	for(int i=0;i<9;i++)
	{
		if(sudoku[i][col]==num)
			return 1;
	}
	return 0;
}

int used_in_box(int sudoku[9][9],int row,int col,int num)
{
	for(int i=0;i<3;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(sudoku[row+i][col+j]==num)
				return 1;
		}
	}
	return 0;
}

void print_sudoku(int sudoku[9][9])
{
	for(int i=0;i<9;i++)
	{
		for(int j=0;j<9;j++)
		{
			printf("%d ",sudoku[i][j]);
		}
		printf("\n");
	}
}










