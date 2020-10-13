 /* Ezer.c
 *
 *   */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
check if array have numbers  and puts it in nums.
if not return the index of the invalid parameter*/
int str_to_int(char * array[],int len, int nums[]){
	int i;
	for (i = 0 ; i<len ; i++){
		if (strlen(array[i]) == 2 ){
			if (array[i][0] - 48 == 0){/* invalid number (start with 0)*/
				return i+1;
			}
			nums[i] = (array[i][0] -48)*10 + (array[i][1] - 48);
		}
		if ( strlen(array[i]) == 3){
			if (array[i][0] - 48 == 0 ){
				return i+1; /* invalid number (start with 0)*/}
			nums[i] = (array[i][0] -48)*100 + (array[i][1] - 48)*10 + array[i][2] -48;
		}
		if (strlen(array[i])  > 3){/*invalid number = too big*/
			 return i+1;
		} 
		if (strlen(array[i]) == 1){
			 nums[i] = array[i][0]-48;	
		}
	} 
	return 0;
}

int valid_nums(int len, int nums[],int min,int max){
	int i;
	for (i = 0 ; i < len ; i++){
		if (nums[i] < min || nums[i] > max){ 
			return i+1; /*numers is too big or too small*/
		}
	}
	return 0;
}





/*count number of empty cells in mat*/
int empty_cells_count(int** mat, int N){
	int i,j,cnt=0;
	for (i=0; i< N ; i++){
		for (j=0; j<N; j++){
			if (mat[i][j]%(N+1) == 0){cnt++;}
		}	
	}
	return cnt;
}

/*free the 2d array*/
void free_mat(int ** mat,int N){
	int i;
	for (i = 0 ;i < N; i++){
		free(mat[i]);
	}
}



