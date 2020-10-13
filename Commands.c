/*
 *Commands.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include "Commands.h"





/*delete the redo part of the stages list
updating the board and the stages list*/
void set_command (lnkd* stage,int ** board,int arr[],int before,int N,int n,int m){
	int* row;int* col;int* cube; int i,r,c,row_index,col_index;	
	row=(int*) malloc(N*sizeof(int));col=(int*) malloc(N*sizeof(int));cube=(int*) malloc(N*sizeof(int));
	if (stage->next != NULL){stage->next = clean_stages_list(stage->next);}
	stage = new_stage(stage);
	board[arr[1]-1][arr[0]-1] =arr[2];
	fill_lists(board,N,n,m,row,col,cube,arr[1]-1,arr[0]-1);
	find_erroneous(board,N,arr[1]-1,arr[0]-1,n,m,before);
	upt_changes(stage,arr[1]-1,arr[0]-1,before,board[arr[1]-1][arr[0]-1]);
	row_index = arr[1]-1; col_index = arr[0]-1;
	for ( i = 0; i < N ; i++){
		r = n*(row_index/n)+i%n; c = m*(col_index/m)+i/n;
		if (board[row_index][i] != row[i] && i != col_index){upt_changes(stage,row_index,i,row[i],board[row_index][i]);}
		if (board[i][col_index] != col[i] && i != row_index){upt_changes(stage,i,col_index,col[i],board[i][col_index]);}
		if (board[r][c] != cube[i] && (r != row_index || c != col_index)){upt_changes(stage,r,c,cube[i],board[r][c]);}
	}
	free(row);free(col);free(cube);

	
}
/*
mat = 2d array
n,m = sudoku dimensions
N = mat size
mark errors = indicator if to show errors
mode_status = game status
*/
void PrintScreen(int** mat, int n, int m, int N,int mark_errors,int mode_status){ /* prints the sudoku */
  int i,j;
  int pos=0, pos1=0;
  if(mode_status == 2){mark_errors =1;}
  for(i=0;i<N+m+1;i++){ /* N+m+1  rows*/
    if(i%(n+1)==0){ /*seperator row*/
      for(j=0;j<4*N+n+1;j++){printf("-");}printf("\n");}

    else{ 
      for(j=0;j<4*N+n+1;j++){
        if(j%(m*4+1)==0){printf("|");} /* seperator*/

        else{if((j%(m*4+1))%4==0){/* fixed point*/
           if(pos!=0){pos1=pos-1;}
           if((mat[pos1/N][pos1%N]/(N+1))==1){printf(".");}/*decode fixed*/
             else{if((mat[pos1/N][pos1%N]/(N+1))==2 && (mark_errors == 1 || mode_status == 2)){printf("*");}
               else{printf(" ");}}}

          else{if((j%(m*4+1)+1)%4==0){/* numbers*/
            if(mat[pos/N][pos%N]%(N+1)==0){printf(" ");}
            else{printf("%d",(mat[pos/N][pos%N]%(N+1))%10);}
            pos++;}

            else{if(((j%(m*4+1)+2)%4==0) && ((mat[pos/N][pos%N]%(N+1))/10)>0){
                printf("%d",mat[pos/N][pos%N]%(N+1)/10);}/*if the number is bigger then 10*/
              else{printf(" ");}} /*space*/

}}}
    printf("\n");
}}}
/*
mat = 2d array
n,m = sudoku dimensions
path = file path
save the file in path
*/
int ssave(int** mat, int n, int m, char* path,int edit){ /*saves the sudoku as a file*/
  int i, j, num;
  FILE* fp;
  char space=' ', line='\n', dot='.', tmp;
  if((fp = fopen(path, "w"))==NULL){return -1;} /*error*/
  tmp='0'+m;/* writes first line*/    
  fwrite(&tmp , 1 , sizeof(char) , fp );
  fwrite(&space , 1 , sizeof(char) , fp );
  tmp='0'+n;
  fwrite(&tmp , 1 , sizeof(char) , fp );
  fwrite(&line , 1 , sizeof(char) , fp );
  for(i=0;i<n*m;i++){
    for(j=0;j<n*m;j++){
      num=mat[i][j]%(n*m+1);
      if(num>9){ /* if num has 2 digits to save */
        tmp='0'+num/10; 
        fwrite(&tmp , 1 , sizeof(char) , fp );}
      tmp='0'+num%10; 
      fwrite(&tmp , 1 , sizeof(char) , fp );
      if(mat[i][j]/(n*m+1)==1 || (edit == 2 && mat[i][j]%(n*m+1) != 0 ) ){fwrite(&dot , 1 , sizeof(char) , fp );}
      if(j!=n*m-1){fwrite(&space , 1 , sizeof(char) , fp );}
}
   fwrite(&line , 1 , sizeof(char) , fp );
}
  fclose(fp);
  return 1;
}

/*
stage = stages linked list
mat = 2d array
n,m = sudoku dimensions
N = mat size
X = threshold
if the borad is errorneous or have no solution return -1 and print message
otherwise fill possible values according to lp and update stages list*/
int guess_command(lnkd* stage,int ** mat, int N,int n, int m,float X){
	int i,j,k,p,t;int* opt;int changed = 0;
  	double*** res=(double***) malloc(N*sizeof(double**));
  	int* lst1=(int*) malloc(N*sizeof(int)); /* allocated lists for uses in other functions */
  	int* lst2=(int*) malloc(N*sizeof(int));
  	for(i=0;i<N;i++){res[i]=(double**) malloc(N*sizeof(double*));
    		for(j=0;j<N;j++){res[i][j]=(double*) malloc(N*sizeof(double));
      			for(k=0;k<N;k++){res[i][j][k]=0;}}}
	i = check_valid_full(mat,lst1,lst2,-1,N,n,m);
	if (i == 0){printf("the board is erroneous, invalid command \n");
		clean_3d_mat(res,N);free(res);free(lst1);free(lst2);
		return -1;}	
	p = lp(N,n,m,mat,res,0);
	if (p <= 0){printf("no solution for this board\n");
		clean_3d_mat(res,N);free(res);free(lst1);free(lst2);	
 		return -1;}
	opt=(int*) malloc(N*sizeof(int));
	for (i = 0; i < N ; i++){ for (j = 0 ; j< N ; j++){ if(mat[i][j]%(N+1) == 0){
		for(t = 0 ; t< N ; t++){opt[t] = 1;}
		clean(lst1,N); clean(lst2,N);
		find_valid_nums(mat,n,m,i,j,opt);
		for (t = 0; t< N; t++){if (opt[t] == 0){res[i][j][t] = 0.0;}}
		k = pick_value_for_guess(res[i][j],N,X);
		mat[i][j] = k+1;
		p = check_valid_full(mat,lst1,lst2,i*N+j,N,n,m);
		if (p == 0) {mat[i][j] = 0;}
		if( k+1 != 0 && p != 0){
		        if (changed == 0){
			if (stage->next != NULL){stage->next = clean_stages_list(stage->next);}
				stage = new_stage(stage);
				changed = 1;}
			upt_changes(stage,i,j,0,k+1);
	     	    }

	}}}
	for(i=0;i<N;i++){
    		for(j=0;j<N;j++){
      			free(res[i][j]);}
    		free(res[i]);
	}
	free(opt);
  	free(res);
	free(lst1);free(lst2);
	return changed;

	
}
/*validate = 1 when using for validate command 
validate = 0 when using for save command
validate = 2 when use in com_manager*/
int validate_command(int **mat,int N,int n,int m,int validate){
	int i,j,k;	
  	int* lst1=(int*) malloc(N*sizeof(int)); /* allocated lists for uses in other functions */
  	int* lst2=(int*) malloc(N*sizeof(int));
	double*** res=(double***) malloc(N*sizeof(double**));
	for(i=0;i<N;i++){res[i]=(double**) malloc(N*sizeof(double*));
    		for(j=0;j<N;j++){res[i][j]=(double*) malloc(N*sizeof(double));
      			for(k=0;k<N;k++){res[i][j][k]=0;}}}
	i = check_valid_full(mat,lst1,lst2,-1,N,n,m);
	if (i == 0 && validate == 1){printf("the board is erroneous, invalid command \n");
		clean_3d_mat(res,N);free(res);free(lst1);free(lst2); return -1;}
	if (i == 0 && validate == 0){printf("the board is erroneous and cant be saved in edit mode\n");
		clean_3d_mat(res,N);free(res);free(lst1);free(lst2);return -1;}
	if (i == 0 && validate == 2){return -2;}
	if(validate == 1 || validate == 0){i = lp(N,n,m,mat,res,1);}
	if (i <= 0 && validate == 1){printf("no solution for this board\n"); 
		clean_3d_mat(res,N);free(res);free(lst1);free(lst2);return -1;}
	if (i <= 0 && validate == 0){printf("there is no solution for this board and cant be saved in edit mode\n");
		clean_3d_mat(res,N);free(res);free(lst1);free(lst2);return -1;}
	if(validate == 1){printf("the board is solvable\n");}
	for(i=0;i<N;i++){
    		for(j=0;j<N;j++){
      			free(res[i][j]);}
    		free(res[i]);
	}
  	free(res);
	free(lst1);free(lst2);
	return 1;



}


/*reading the file in the path
return NULL if error in reading the file or invalid matrix according to the status
calling to mat_file_check for checking the matrix and update erroneous cells if needed
*/
int** file_load(char *path,int* lst1,int status){
	int **hlp; static int** new;
	int n,m,i,j,N;
	hlp=(int**) malloc(sizeof(int*)*(1));
	for(i=0;i<1;i++){hlp[i]=(int*) malloc(sizeof(int)*(2));
    		for(j=0;j<2;j++){hlp[i][j]=0;}}

	i = sload_size(path,hlp);	/*printf("%d\n",mat[0][0]);*/
	if (i == -1){
		printf("error occurd while loading the file\n");free(hlp[0]);free(hlp); 
		return NULL;}
	if (i == 0 ){printf("error has occurred, the file invalid\n");free(hlp[0]);free(hlp); 
	        return NULL;}
	m = i%10;
	n = i/10;
	N = n*m;
	new=(int**) malloc(sizeof(int*)*(N));
	for(i=0;i<N;i++){new[i]=(int*) malloc(sizeof(int)*(N));
    		for(j=0;j<N;j++){new[i][j]=0;}}

	j = sload_mat(new,path,n,m,1); 
	free(hlp[0]);free(hlp); 
	if (j == -1  ){  
		printf("error occurd while loading the file\n");
		for(i=0;i<N;i++){new[i]=(int*) malloc(sizeof(int)*(N));for(j=0;j<N;j++){new[i][j]=0;}}
		return NULL;}
	if (j == 0 ){printf("error has occurred, the file invalid\n"); return NULL;}
	lst1[0] = n; lst1[1] = m;
	
	j = mat_file_check(new,N,n,m,status);
	if (j == 0 && status == 1){
		printf("error in loading the file, the file have erroneous fixed cells\n");
		for(i=0;i<N;i++){new[i]=(int*) malloc(sizeof(int)*(N));for(j=0;j<N;j++){new[i][j]=0;}}
		return NULL;
	}
	printf("file loaded succesfully\n");
	return new;
	

}
/*mat = 2d array - soduko board
 N = mat size
 free mat from memory and return 9*9 2d array with 0 values*/
int**  edit_command(int** mat,int N){
	static int** fill;
	int i,j;
	for(i=0;i<N;i++){
    		free(mat[i]);}
	free(mat);
	fill=(int**) malloc(sizeof(int*)*(9));
	for(i=0;i<9;i++){fill[i]=(int*) malloc(sizeof(int)*(9));
    		for(j=0;j<9;j++){fill[i][j]=0;}} 
	return fill;

	

}
/*/*mat = 2d array soduko board
 n,m = sudoku dimensions
 N = mat size
 row_index, col_index = mat cell indexed
 run ilp to solve the board and printing possibile value for mat[row_index][col_index]
 if the board is unsolvable printing a message
*/
void hint_command(int ** mat,int m ,int n, int N,int row_index,int col_index){
	int i,j,k;
	double*** res=(double***) malloc(N*sizeof(double**));
	for(i=0;i<N;i++){res[i]=(double**) malloc(N*sizeof(double*));
    		for(j=0;j<N;j++){res[i][j]=(double*) malloc(N*sizeof(double));
      			for(k=0;k<N;k++){res[i][j][k]=0;}}}
	i = lp(N,n,m,mat,res,1);
	if (i <= 0){printf("the board is unsolvable, hint command invalid for this board\n");
		clean_3d_mat(res,N);free(res);return ;}
	i =  pick_value_for_guess(res[row_index][col_index],N,0.5);
	printf("optional value for this cell is %d\n",i+1);
	clean_3d_mat(res,N);free(res);
}

/*mat = 2d array soduko board
 n,m = sudoku dimensions
 N = mat size
 row_index, col_index = mat cell indexed
 print possible value of the cell mat[row_index][col_index] using the lp function
 if the board is unsolvable return a message*/
void guess_hint_command(int** mat, int m, int n, int N,int row_index,int col_index){
	int i,j,k;
	double*** res=(double***) malloc(N*sizeof(double**));
	for(i=0;i<N;i++){res[i]=(double**) malloc(N*sizeof(double*));
    		for(j=0;j<N;j++){res[i][j]=(double*) malloc(N*sizeof(double));
      			for(k=0;k<N;k++){res[i][j][k]=0;}}}
	i = lp(N,n,m,mat,res,0);
	if (i <= 0){printf("the board is unsolvable, hint command invalid for this board\n");
		clean_3d_mat(res,N);free(res);return ;}
	printf("possible values for this cell are:\n");
	for (i = 0; i < N ; i++){
		if (res[row_index][col_index][i] > 0){
			printf("%d with score of %f ",i+1,res[row_index][col_index][i]);
		}
	}
	printf("\n");
	clean_3d_mat(res,N);free(res);

}
/*mat = 2d array - soduko board
  n , m = soduko dimension,
  N = array size
  checking number of solutions with the count solution method
  return number of possible solutions for this board
  if the board have no solutions printing it for the user*/
void num_solutions_command(int** mat,int n,int m,int N){
	int i,j;
	int **hlp;
	hlp=(int**) malloc(sizeof(int*)*(N));
	for(i=0;i<N;i++){hlp[i]=(int*) malloc(sizeof(int)*(N));
    		for(j=0;j<N;j++){hlp[i][j]=mat[i][j];}}
	i = count_solutions(hlp,n,m);
	if (i>0){
		printf("there are %d solutions for this board\n",i);
		for (j = 0 ; j< N; j++){free(hlp[j]);} free(hlp);
		 return;}
	printf("the board is unsolvable\n");
	for (j = 0 ; j< N; j++){free(hlp[j]);} free(hlp);
	return;
}	
/*stage = stages linked list
  mat = 2d array - soduko board
  n,m = sudoku dimension
  fills the mat in with the 'obvious values' and update the stages linked list*/
int autofill_command(lnkd* stage,int** mat, int n, int m){
  	int* opt; int i;int j;int check;int k; int changed=0;
	int** fill;
	int N = n*m;
	opt=(int*) malloc(N*sizeof(int));for(i = 0 ; i< N ; i++){opt[i] = 1;}
	fill=(int**) malloc(sizeof(int*)*(N));
	for(i=0;i<N;i++){fill[i]=(int*) malloc(sizeof(int)*(N));
    		for(j=0;j<N;j++){fill[i][j]=0;}} 
	for (i=0; i < N ; i++){
		for (j = 0; j < N ; j++){
			if(mat[i][j] == 0){
			find_valid_nums(mat,n,m,i,j,opt);
			check = find_one_valid(N,opt);
			if (check >= 0){fill[i][j] = check;}
			for(k = 0 ; k< N ; k++){opt[k] = 1;}
			}
		}
	}
	for (i=0; i < N ; i++){
		for (j = 0; j < N ; j++){
			if(mat[i][j]%(n+1) == 0 && fill[i][j] !=0 ){
				if (changed == 0){
					if (stage->next != NULL){stage->next = clean_stages_list(stage->next);}
					stage = new_stage(stage);
					changed = 1;}
				upt_changes(stage,i,j,mat[i][j],fill[i][j]);
				mat[i][j] = fill[i][j];
			}
		}
	}
	free(opt);
	for (i=0; i < N ; i++){
		free(fill[i]);
	}
	free(fill);
	return changed;


	

}
/*mat = 2d array
  stage = stages linked list
  N  = mat sizes
  return the previous stage if exist, if not printing en error*/
lnkd* undo_command(int** mat, lnkd* stage,int N){
  if (stage->prv == NULL || stage->plays == NULL){
  printf("error has occurd, there is no previous move to this board state\n");
  return stage;
  }
  changes_print_upt(mat,stage,1,0,N);
  return stage->prv;
}

/*mat = 2d array
  stage = stages linked list
  N  = mat sizes
return the next stage if exist, if not - printing en error*/
lnkd* redo_command(int** mat, lnkd* stage,int N){
  if (stage->next == NULL){
  printf("error has occurd, there is no next move to this board state\n");
  return stage;
  }
  changes_print_upt(mat,stage->next,2,0,N);
  return stage->next;
}
/*gets a 2d array and stages linked list
return the first node in the stages linked list*/
lnkd* reset_command(int** mat,lnkd* stage){
	while(stage->prv != NULL && stage->plays != NULL){
		changes_print_upt(mat,stage,1,1,0);
		stage = stage->prv;
	}
	return stage;
}






int generate(int** board,lnkd* stage,int N,int n, int m,int x, int y){ /*generates the soduko*/
  int i,j,num, ret,num2, tmp, fixed_num,worked,counter ,tmp2;
  int** old;
  int* prem=malloc(sizeof(int)*N*N);
  int* prem2=malloc(sizeof(int)*N*N);
  int** mat=malloc(sizeof(int*)*N);
  old=(int**) malloc(sizeof(int*)*(N));
  	for(i=0;i<N;i++){old[i]=(int*) malloc(sizeof(int)*(N));
    		for(j=0;j<N;j++){old[i][j]=board[i][j];}}
  fixed_num=0;
  for(i=0;i<N;i++){
    mat[i]=malloc(sizeof(int)*N);
    for(j=0;j<N;j++){
      if(board[i][j]!=0){fixed_num++;}}}
  if(x+fixed_num>N*N){printf("x is too large!");return 0;}
  if(y>N*N){printf("y is too large!");return 0;}
  for(i=0;i<N*N;i++){prem[i]=i;prem2[i]=i;}
  worked=0;counter=0;
  while(worked==0 && counter<1000){
  counter++;
  for(i=0;i<N*N;i++){num=rand()%(N*N);num2=rand()%(N*N);tmp=prem[num2];tmp2=prem[num];prem[num2]=tmp2;prem[num]=tmp;} /* creates the premutation of the board generation order */
  for(i=0;i<N*N;i++){num=rand()%(N*N);num2=rand()%(N*N);tmp=prem2[num2];tmp2=prem2[num];prem2[num2]=tmp2;prem2[num]=tmp;} /*chooses the y numbers it keeps */
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){mat[i][j]=board[prem[i*N+j]/N][prem[i*N+j]%N];}}
  backtrack_main(mat,0,n,m,prem,x); /* uses count solutions to generate board*/
  for(i=0;i<N*N;i++){board[prem[i]/N][prem[i]%N]=mat[i/N][i%N];}
  worked=find_solution(N,n, m, board);/*  running ilp on generated board */
  if(worked<1){
    for(i=0;i<N*N;i++){
      board[i/N][i%N]=old[i/N][i%N];
}}
}
  if(worked<1){
    printf("cannot generate board\n");
    for(i=0;i<N*N;i++){
      board[i/N][i%N]=old[i/N][i%N];}
    ret=0;}
  else{for(i=0;i<N*N-y;i++){board[prem2[i]/N][prem2[i]%N]=0;ret=1;}} /*keeping only y cells */
  for(i=0;i<N;i++){free(mat[i]);}
  free(prem);
  free(mat);
  free(prem2);
  mat_changes_updater(old,board,stage,N);
  for (i = 0 ; i < N ; i ++){
	free(old[i]);}
  free(old);
  return ret;
}





