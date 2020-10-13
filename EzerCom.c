/*
EzerCom.c

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "EzerCom.h"
#include "LinkedLst.h"

/*check if there is more then 1 values hold for the same cell in guess command
if more then 1 return -1, if only 1 return the index, if non return -2;  fill to_rnd with items > X*/
int bigger(double * k,int N, float X,double* to_rnd){
	int i;
	int more_then_one=0; int index = 0;
	for (i = 0; i < N ; i++){
		if (k[i] > X){
			index = i;
			if (more_then_one == 0 ){more_then_one = 1;}
			if (more_then_one == 1){more_then_one = 2;}
			to_rnd[i] = k[i];
		}
	}
	if (more_then_one == 1){ return index;} /*only 1 candidate*/
	if (more_then_one == 0){ return -2;} /*no candidates*/
	return -1; /*more then 1 candidates*/
}
/*return index of the picked cell of to*/
int weighted_rnd_pick( double * to){
	int i= 0;double sum = 0;float rnd= 0 ;srand((unsigned)time(NULL));
	rnd = (float)rand()/RAND_MAX;
	while (1){
   		sum += to[i];
   		if (rnd < sum){ return i;}
   		i++;
	}
	return i;
}


/*return cell value for guess command */
int pick_value_for_guess(double *k,int N, float X){
	int i;
	double* to_rnd=(double*) malloc(N*sizeof(double)); for (i = 0; i < N; i++){to_rnd[i] = 0;}
	i = bigger(k,N,X,to_rnd);
	
	if (i == -2){ free(to_rnd);return -1;} /*no cell is picked*/
	if (i == -1 ){ i =  weighted_rnd_pick(to_rnd); free(to_rnd); return i;} /*need to pick more then 1 */
	free(to_rnd);
	return i;
}

/*
 loads the cell values of the sudoku from a saved sudoku file ,use type=1
checking the validity of the file according to the expected values of the soduko board
return -1 if there is problem in reading the file
return 0 if the file is invalid
return 1 if the board loaded succesfully*/
int sload_mat(int** mat,char* path, int n, int m,int type){
  FILE* fp;
  char str[6] = {'\0'};
  int  num=0,  pos = 0,cnt=0,double_digit=0,i;
  if((fp = fopen(path, "r"))==NULL){return -1;}
  while(fscanf(fp, "%5s",str) == 1){
	if (type == 1 && cnt < 2){for (i = 0 ; i < 6; i++){str[i] = '\0';}cnt++;continue;}
	if (type == 1){
	if (pos == n*m*n*m){return 0;}
	if (str[0] < '0' || str[0] > '9'){return 0;}
        num = str[0] -'0' ;
	if (str[1] < '9' && str[1] > '0'){
		num = num*10 + (str[1] -'0');double_digit = 1;}
	if (num > n*m){return 0;}
	if (double_digit == 0){
		if (str[1] == '.'){
			num = num+(n*m)+1;
			if (str[2] != '\0'){return 0;}
		}
		else{if((str[1] > '9' || str[1] < '0') && str[1] != '\0'){return 0;}}
	}
	if (double_digit == 1){
		if (str[2] == '.'){
			if (str[3] != '\0'){return 0;}
			num = num+(n*m)+1;}
		}
		
	}
	if (type == 0){
	if (pos == 2 ){return 1;}
	if (str[0] < '0' || str[0] > '9'){return 0;}
	num = str[0] -'0' ;
	if (str[1] < '9' && str[1] > '0'){
		num = num*10 + (str[1] -'0');double_digit = 1;}
	if (double_digit == 0){
		if (str[1] != '\0'){return 0;}}
	if (double_digit ==1 ){
		if (str[2] != '\0'){return 0;}}
	}
        mat[pos/(n*m)][pos%(n*m)]=num;
	pos++;
	for (i = 0 ; i < 6; i++){str[i] = '\0';}
	if (double_digit == 1){double_digit =0;}
	
 }
  fclose(fp);
  if(pos==n*m*n*m){return 1;}
  return 0;
}

/*loading the wanted to be upload file 2 first arguements by calling sload_mat function */
int sload_size(char* path,int** mat){ /* loads the size of the sudoku from a saved sudoku file , returns m*10+n, insert helper mat[1][2]*/
  int i;
  i = sload_mat(mat,path,2,1,0);
  if (i == 0 ){return 0;}
  if (i == -1 ){return -1;}
  return mat[0][0]*10+mat[0][1];
}



/*getting matrix and index of row and col and update the cell values options array 
according to the valid possible values in the cell mat[row_index][col_index]
*/
void find_valid_nums(int **mat,int n,int m,int row_index,int col_index,int *  options){
	int i;int r; int c;
	int N = n*m;
	for (i=0 ; i < N ; i++){
		r = n*(row_index/n)+i%n; c = m*(col_index/m)+i/n;
		if ( mat[i][col_index]%(N+1) != 0){
			options[mat[i][col_index]%(N+1)-1] = 0;
		}
		if ( mat[row_index][i]%(N+1) != 0){
			options[mat[row_index][i]%(N+1)-1] = 0;
		}
		if (mat[r][c]%(N+1) != 0){
			options[mat[r][c]%(N+1)-1] = 0;
		}
	}
}
/*return index of the cell with value of 1 if only 1 cell exisst
 the numbers of cells with 1 value is different from 1 return -1
  */
int find_one_valid(int N, int* opt){
	int i;int cnt= 0;
	for (i=0 ; i < N ; i++){	
		if (opt[i] == 1 ){
			if(cnt == 0){cnt = i+1;}
			else{return -1;}
		}
	}
	if (cnt == 0){return -1;}
	return cnt;
	
}

/*return 1 if the cell is fixed and 2 if its erroneous otherwise return 0*/
int check_erroneous_fixed(int** mat, int N,int row_index,int col_index){
	int i = mat[row_index][col_index]/(N+1);
	if (i == 1) {return 1;}
	if (i == 2) {return 2;}
	return 0;
}




/*call this function only when mat[row][col] ==  before && before !=0 && mat[row][col] is errornoues
checks for cells that have been marked as erroneous if they are still erroneous after 
the set command, return 1 if so, and 0 else*/
int is_erroneous(int** mat,int N,int row_index,int col_index,int n,int m,int before){
	int i,c,r;
	for (i = 0; i < N ; i++){
		r = n*(row_index/n)+i%n; c = m*(col_index/m)+i/n;
		if ( (i != col_index) && mat[row_index][i]%(N+1) == before%(N+1)){return 1;}
		if ( (i != row_index) && mat[i][col_index]%(N+1) == before%(N+1)){return 1;}
		if ( (c != col_index || r != row_index) && mat[r][c]%(N+1) == before%(N+1)){return 1;}
	}
	return 0;
}

/*
called after set command
going over the cells in the new val col
update the cell itself and the other cells to be erronoues if neeed*/
int find_erroneous_col(int **mat,int N,int row_index,int col_index){
	int i,is_erroneous = 0;int err_fix;
	for (i=0; i < N; i++){
		if (i != row_index &&  (mat[i][col_index]%(N+1) == mat[row_index][col_index]%(N+1))  ){
			if (mat[row_index][col_index]%(N+1) != 0){ mat[row_index][col_index] = (mat[row_index][col_index])%(N+1)+2*N+2;}
			if((err_fix = check_erroneous_fixed(mat,N,i,col_index)) == 0 ){
				mat[i][col_index] = mat[i][col_index]+2*N+2;}
			if(err_fix == 1){return 0;} /*one of the erroneous cells are fixed*/
			if (is_erroneous == 0){ is_erroneous =1;}
		}
	}
	if (is_erroneous == 1 && check_erroneous_fixed(mat,N,row_index,col_index) == 0) { 
		mat[row_index][col_index] = mat[row_index][col_index]+2*N+2;
	}
	return 1;
}


/*called after set command
   going over the cells in the new val row
  update the cell itself and the other cells to be erronoues if neeed*/
int find_erroneous_row(int** mat,int N, int row_index,int col_index){
	int i,is_erroneous =0; int err_fix;
	for (i=0;i < N ; i++){
		if ( i != col_index &&  (mat[row_index][i]%(N+1) == mat[row_index][col_index]%(N+1)) ){
			if (mat[row_index][col_index]%(N+1) != 0){ mat[row_index][col_index] = mat[row_index][col_index]%(N+1)+2*N+2;}
			if((err_fix = check_erroneous_fixed(mat,N,row_index,i)) == 0 ){
				mat[row_index][i] = mat[row_index][i]+2*N+2;}
			if(err_fix == 1){return 0;}
			if (is_erroneous == 0){ is_erroneous =1;}
		}
	}
	if (is_erroneous == 1 && check_erroneous_fixed(mat,N,row_index,col_index) == 0) { 
		mat[row_index][col_index] = mat[row_index][col_index]+2*N+2;
	}
	return 1;

}
/*called after set command
going over the cells in the new val box
update the cell itself and the other cells to be erronoues if neeed*/
int find_erroneous_box(int** mat,int N, int row_index,int col_index,int n,int m){
	int i,is_erroneous = 0,r,c;int err_fix;
	for (i = 0 ; i < N ; i++){
		r = n*(row_index/n)+i%n; c = m*(col_index/m)+i/n;
		if (c != col_index || r != row_index){
			if (mat[r][c]%(N+1) == mat[row_index][col_index]%(N+1)){
			if (mat[row_index][col_index]%(N+1) != 0){ mat[row_index][col_index] = mat[row_index][col_index]%(N+1)+2*N+2;}
				if((err_fix = check_erroneous_fixed(mat,N,r,c)) == 0 ){
					mat[r][c] = mat[r][c] +2*N+2; }
				if(err_fix == 1){return 0;}
				if (is_erroneous == 0){ is_erroneous =1;}
			}
		}
	}
	if (is_erroneous == 1 && check_erroneous_fixed(mat,N,row_index,col_index) == 0) { 
		mat[row_index][col_index] = mat[row_index][col_index]+2*N+2;
	}
	return 1;
}

/*
called afte set command 
mat= board, N = n*m , row_index, col_index = new val index, before = the value of the 
cell before the changing
updating the erronoues values of the new cell and of the other new erroneous cells if needed
 */
void find_erroneous(int** mat,int N,int row_index,int col_index,int n,int m,int before){
	int i,c,r;
	if (mat[row_index][col_index] != 0){
	find_erroneous_col(mat,N,row_index,col_index);
	find_erroneous_row(mat,N,row_index,col_index);
	find_erroneous_box(mat,N,row_index,col_index,n,m);}
	if (mat[row_index][col_index]%(N+1) != before%(N+1)){
		for (i=0; i < N ; i++){
			r = n*(row_index/n)+i%n; c = m*(col_index/m)+i/n;
			if (  (mat[row_index][i]%(N+1) == before%(N+1))
			 && check_erroneous_fixed(mat,N,row_index,i) == 2 ){	/*check erronoues_fixed = 2 -> cell is erroneous*/	
				if (is_erroneous(mat,N,row_index,i,n,m,before) == 0){
					mat[row_index][i] = mat[row_index][i]%(N+1);}
			}
			if (  (mat[i][col_index]%(N+1) == before%(N+1))
			 && check_erroneous_fixed(mat,N,i,col_index) == 2 ){ 
				if (is_erroneous(mat,N,i,col_index,n,m,before) == 0){
					mat[i][col_index] = mat[i][col_index]%(N+1);}
			}
			if (  (mat[r][c]%(N+1) == before%(N+1))
			 && check_erroneous_fixed(mat,N,r,c) == 2 ){ 
				if (is_erroneous(mat,N,r,c,n,m,before) == 0){
					mat[r][c] = mat[r][c]%(N+1);}
			}
		}
	}
		
	
}
/*filling the lists of row col and cube with the matrix values*/
void fill_lists(int** mat,int N,int n,int m,int* row,int* col,int* cube,int row_index,int col_index){
	int i,r,c;
	for ( i = 0 ; i< N; i++){
		r = n*(row_index/n)+i%n; c = m*(col_index/m)+i/n;
		row[i] = mat[row_index][i];
		col[i] = mat[i][col_index];
		cube[i] = mat[r][c];
	}
}


/*calling command = 1-> solve
calling command = 2 -> edit
getting a matrix from uploaded file, checking if its rows are legal and update the 
erroneous\fix cells marks acoording to the values and the calling_commands*/
int hlp_mat_check_row(int** mat,int N, int row_index,int col_index,int calling_command){
	int i ,fix_err_i, fix_err_index , index_collision = 0;
	fix_err_index = check_erroneous_fixed(mat,N,row_index,col_index);
	for (i=0;i < N ; i++){
		if ( i != col_index &&  (mat[row_index][i]%(N+1) == mat[row_index][col_index]%(N+1)) && mat[row_index][i]%(N+1) != 0 ){
			index_collision = 1;
			fix_err_i = check_erroneous_fixed(mat,N,row_index,i);
			if (fix_err_index == fix_err_i){
				if (fix_err_index == 1 && calling_command == 1){printf("1   %d\n" ,mat[row_index][col_index]);return 0;  }
			}
			if (fix_err_index != 1 || calling_command != 1){
				mat[row_index][col_index] = mat[row_index][col_index]%(N+1)+2*N+2;}
		}
	}
	if (calling_command == 2 && fix_err_index == 1 && index_collision == 0){ /*changing not erroneous fixed cells into regular */
		mat[row_index][col_index] = mat[row_index][col_index]%(N+1);
	}
	return 1;

}

/*calling command = 1-> solve
calling command = 2 -> edit
getting a matrix from uploaded file, checking if its cols are legal and update the 
erroneous\fix cells marks acoording to the values and the calling_commands*/
int hlp_mat_check_col(int** mat,int N, int row_index,int col_index,int calling_command){
	int i ,fix_err_i, fix_err_index , index_collision = 0;
	fix_err_index = check_erroneous_fixed(mat,N,row_index,col_index);
	for (i=0;i < N ; i++){
		if ( i != row_index &&  (mat[i][col_index]%(N+1) == mat[row_index][col_index]%(N+1)) && mat[i][col_index]%(N+1) != 0 ){
			index_collision = 1;
			fix_err_i = check_erroneous_fixed(mat,N,i,col_index);
			if (fix_err_index == fix_err_i){
				if (fix_err_index == 1 && calling_command == 1){return 0;}
			}
			if (fix_err_index != 1 || calling_command != 1){
				mat[row_index][col_index] = mat[row_index][col_index]%(N+1)+2*N+2;}
		}
	}
	if (calling_command == 2 && fix_err_index == 1 && index_collision == 0){ /*changing not erroneous fixed cells into regular */
		mat[row_index][col_index] = mat[row_index][col_index]%(N+1);
	}
	return 1;

}


/*calling command = 1-> solve
calling command = 2 -> edit
getting a matrix from uploaded file, checking if its cols are legal and update the 
erroneous\fix cells marks acoording to the values and the calling_commands*/
int hlp_mat_check_box(int** mat,int N, int row_index,int col_index,int n, int m,int calling_command){
	int i ,fix_err_i, fix_err_index ,r,c; int index_collision = 0;
	fix_err_index = check_erroneous_fixed(mat,N,row_index,col_index);
	for (i=0;i < N ; i++){
		r = n*(row_index/n)+i%n; c = m*(col_index/m)+i/n;
		if ( (r != row_index || c != col_index)  &&  (mat[r][c]%(N+1) == mat[row_index][col_index]%(N+1)) ){
			index_collision = 1;
			fix_err_i = check_erroneous_fixed(mat,N,r,c);
			if (fix_err_index == fix_err_i){
				if (fix_err_index == 1 && calling_command == 1){  return 0;}
			}
			if (fix_err_index != 1 || calling_command != 1){
				mat[row_index][col_index] = mat[row_index][col_index]%(N+1)+2*N+2;}
		}
	}
	if (calling_command == 2 && fix_err_index == 1 && index_collision == 0){ /*changing not erroneous fixed cells into regular */
		mat[row_index][col_index] = mat[row_index][col_index]%(N+1);
	}
	return 1;

}




/*check the matrix, if in solve mode and there is erroneous fixed cell return 0
status = solve\edit
return 1 if valid matrix according to the status
update the 
erroneous\fix cells marks acoording to the values and the calling_commands
*/
int mat_file_check(int** mat,int N,int n,int m,int status){
	int i;int j;	
	for (i = 0 ; i < N ; i++){
		for (j=0 ; j< N; j++){
			if (mat[i][j]%(N+1) != 0){
					if(hlp_mat_check_col(mat,N,i,j,status) == 0 && status ==1 ){return 0;}
					if(hlp_mat_check_row(mat,N,i,j,status) == 0 && status ==1){return 0;}
					if(hlp_mat_check_box(mat,N,i,j,n,m,status)==0 && status ==1){return 0;}
			}
		}
	}
	return 1;
}

/*old,new = 2d array
stage = stages linked list
N = matrix size;
gets 2 soduko board, update the stage changes according to the diffrence between them*/
void mat_changes_updater(int** old,int** new ,lnkd* stage,int N){
	int i,j;
	if (stage->next != NULL){stage->next = clean_stages_list(stage->next);}
	stage = new_stage(stage);
	for (i = 0 ; i < N ; i++){
		for (j = 0; j < N ; j++){
			if(old[i][j] != new[i][j]){
				upt_changes(stage,i,j,old[i][j],new[i][j]);
			}
		}
	}
}

void clean_3d_mat(double*** res,int N){
	int i,j;
	for(i=0;i<N;i++){
    		for(j=0;j<N;j++){
      			free(res[i][j]);}
    		free(res[i]);
	}
}



