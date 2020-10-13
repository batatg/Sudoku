#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Liner.h"



  /* type==1 <-> ilp, type==0 <-> lp*/
int lp(int N,int n, int m, int** mat,double*** res,int type){
  GRBenv   *env   = NULL;
  GRBmodel *model = NULL;
  int       error = 0;
  int***     vars=(int ***) malloc(N*sizeof(int**)); /* vars[i][j][k]=1 <-> Xijk is a variable */
  double *   sol; 
  int*       ind=(int *) malloc(N*sizeof(int));
  int*       lst=(int *) malloc(N*sizeof(int));
  double*    val=(double *) malloc(N*sizeof(double));
  double*    obj;
  char*      vtype;
  char*      str=(char *) malloc((N+3)*sizeof(char));
  int       optimstatus;
  double    objval;
  int success=0;
  int i ,j ,k,num,count=1,pos;

  for(i=0;i<N;i++){/* set vars correct vals*/
    vars[i]=(int**) malloc(N*sizeof(int*));
    for(j=0;j<N;j++){
      vars[i][j]=(int*) malloc(N*sizeof(int));
      for(k=0;k<N;k++){
        if(mat[i][j]==0){
          mat[i][j]=k+1;
          vars[i][j][k]=check_valid_full(mat,ind,lst,i*N+j,N,n,m)*count; /* marks the index of the var!!*/
          if(vars[i][j][k]){count++;}
          mat[i][j]=0;}
        else{
          vars[i][j][k]=0;
}        
}}}

  obj=(double *) malloc(count*sizeof(double));
  sol=(double *) malloc(count*sizeof(double));
  vtype=(char *) malloc(count*sizeof(char));
  if(type){ /* target func*/ 
    obj[0]=1;
    for(i=1;i<count-1;i++){obj[i]=0;}}
  else{
    for(i=0;i<count-1;i++){obj[i]=rand()%5+1;}
}

  /* Create environment - log file is mip1.log */
  error = GRBloadenv(&env, "mip1.log");
  if (error) {
	  printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }
  
  error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
  if (error) {
	  printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

  /* Create an empty model named "mip1" */
  error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
  if (error) {
	  printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

  /* Add variables */
  
  if(type){for(i=0;i<count-1;i++){vtype[i] = GRB_BINARY;}}
  else{for(i=0;i<count-1;i++){vtype[i] = GRB_CONTINUOUS;}}
  /*printf("%d",count);*/
  /* add variables to model */
  error = GRBaddvars(model, count-1, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
  if (error) {
	  printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

  /* Change objective sense to maximization */
  error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
  if (error) {
	  printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

  /* update the model - to integrate new variables */

  error = GRBupdatemodel(model);
  if (error) {
	  printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }


  /* constraints  */
  for(i=0;i<N;i++){val[i]=1;}
  
  /* if lp - bound vars between 0 and 1 */ 
  if(!type){
    for(j=0;j<count-1;j++){
      ind[0]=j;val[0]=1;
      error = GRBaddconstr(model, 1, ind, val, GRB_LESS_EQUAL, 1, str);
      if (error) {
	    printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
	    return -1;}
      error = GRBaddconstr(model, 1, ind, val, GRB_GREATER_EQUAL, 0, str);
      if (error) {
	    printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
	    return -1;
    }}}

  for(j=0;j<N*N;j++){
    /* make sure that sum of vals in each row, coloum, cell and box is 1 */
    pos=0;
    for(i=0;i<N;i++){
      num=vars[i][j/N][j%N];
      if(num){ind[pos]=num-1;pos++;}}
    sprintf(str,"a%d",j);
    if(pos){error = GRBaddconstr(model, pos, ind, val, GRB_EQUAL, 1, str);
    if (error) {
	    printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
	    return -1;
    }}
    pos=0;
    for(i=0;i<N;i++){
      num=vars[j/N][j%N][i];
      if(num){ind[pos]=num-1;pos++;}}
    sprintf(str,"b%d",j);
    if(pos){error = GRBaddconstr(model, pos, ind, val, GRB_EQUAL, 1, str);
    if (error) {
	    printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
	    return -1;
    }}
    pos=0;
    for(i=0;i<N;i++){
      num=vars[j/N][i][j%N];
      if(num){ind[pos]=num-1;pos++;}}
    sprintf(str,"c%d",j);
    if(pos){error = GRBaddconstr(model, pos, ind, val, GRB_EQUAL, 1, str);
    if (error) {
	    printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
	    return -1;
    }}
    pos=0;
    for(i=0;i<N;i++){
      num=vars[n*((j/N)/n)+i%n][m*((j/N)%n)+i/n][j%N];
      if(num){ind[pos]=num-1;pos++;}}
    sprintf(str,"b%d",j);
    if(pos){error = GRBaddconstr(model, pos, ind, val, GRB_EQUAL, 1, str);
    if (error) {
	    printf("ERROR %d 1st GRBaddconstr(): %s\n", error, GRBgeterrormsg(env));
	    return -1;
    }}}

  /* Optimize model - need to call this before calculation */
  error = GRBoptimize(model);
  if (error) {
	  printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

  /* Write model to 'mip1.lp' - this is not necessary but very helpful */
  error = GRBwrite(model, "mip1.lp");
  if (error) {
	  printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

  /* Get solution information */

  error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
  if (error) {
	  printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

  /* get the objective -- the optimal result of the function */
  error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
  if (error) {
	  /*printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));*/
	  return -1;
  }

  /* get the solution - the assignment to each variable */
  /* 3-- number of variables, the size of "sol" should match */
  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, count-1, sol);
  if (error) {
	  printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
	  return -1;
  }

  /* print results */
  printf("\nOptimization complete\n");
  
  /* solution found */
  if (optimstatus == GRB_OPTIMAL) {
    success=1;
    printf("Optimal objective: %.4e\n", objval);
    num=0;
    pos=0;
    for(i=0;i<N;i++){
      for(j=0;j<N;j++){
        for(k=0;k<N;k++){
          if(vars[i][j][k]){res[i][j][k]=sol[vars[i][j][k]-1];}
          else{res[i][j][k]=(mat[i][j]==k+1);}
}}
}}
  /* no solution found */
  else if (optimstatus == GRB_INF_OR_UNBD) {
    printf("Model is infeasible or unbounded\n");
  }
  /* error or calculation stopped */
  else {
    printf("Optimization was stopped early\n");
  }

  /* IMPORTANT !!! - Free model and environment */
  GRBfreemodel(model);
  GRBfreeenv(env);
  free(sol);free(obj);free(val);free(ind);free(str);free(vtype);free(lst);
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      free(vars[i][j]);
}
    free(vars[i]);
}
  free(vars);
  return success;
}


int find_solution(int N,int n, int m, int** mat){ /*update a sol to the board */
  int i, j , k ,ret, num;  int type = 1;
  double*** res=(double***) malloc(N*sizeof(double**));
  for(i=0;i<N;i++){res[i]=(double**) malloc(N*sizeof(double*));
    for(j=0;j<N;j++){res[i][j]=(double*) malloc(N*sizeof(double));
      for(k=0;k<N;k++){res[i][j][k]=0;}}}
  ret = lp(N,n,m,mat,res,type); /*generate sol*/
  if(type == 1 && ret != 0){ /* update the board*/
    for(i=0;i<N;i++){
      for(j=0;j<N;j++){      
        for(k=0;k<N;k++){
          num=(int) res[i][j][k];
          if(num){mat[i][j]=k+1;}}}
}}
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      free(res[i][j]);}
    free(res[i]);
}
  free(res);
  if (ret == 0){return ret;}
  return ret;
}
