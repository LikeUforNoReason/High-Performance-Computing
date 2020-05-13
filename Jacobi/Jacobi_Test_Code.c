# include<stdio.h>
# include<math.h>
# define N 6

/*
*Solve linear equations using Jacobian
*/ 
int main(){
	float NF2(float *x,float *y);
	float A[N][N],b[N],sum=0;
	float x[N],y[N]={0},x0[N];
	int i,j,n=0;
	
	//Input coefficient matrix 
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			scanf("%f",&A[i][j]);
		}
	}
	
	//Input constant matrix
	for(i=0;i<N;i++){
 		scanf("%f",&b[i]);
	} 
	
	//Enter the initial value of the solution
	 for(i=0;i<N;i++){
 		scanf("%f",&x0[i]);
 	} 
	
	//Output coefficient matrix
	printf("Output coefficient matrix:\n");
	 for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			printf("%3.1f    ",A[i][j]);
		}
		printf("\n");
	} 
	
	//Output constant matrix
	printf("Output constant matrix:\n");
	for(i=0;i<N;i++){
		printf("%3.1f\n",b[i]);
	} 
	
	//Output the initial value of the solution
	printf("Output the initial value of the solution:\n");
	for(i=0;i<N;i++) 
	{
				printf("%3.1f\n",x0[i]);
	}
	/*
	*Solving Linear Equations Using Jacobian Iteration
	*/
	for(i=0;i<N;i++)
	{
		x[i]=x0[i];
	}
	for(n=0;;n++){
				//Calculate the next value
	for(i=0;i<N;i++){
		sum=0;
		for(j=0;j<N;j++){
			if(j!=i){
				sum=sum+A[i][j]*x[j];
			}
		}
		y[i]=(1/A[i][i])*(b[i]-sum);
		//sum=0;
	}
	//Judgment error 
		if(NF2(x,y)>0.01){
				for(i=0;i<N;i++){
		x[i]=y[i];
	}
	} 
			else
			break;
	} 
	printf("After %d Jacobian iterations, solve the set of equations:\n",n+1);
	for(i=0;i<N;i++){
		printf("%f      ",y[i]);
	}
	
	return 0;
}
//Find the second norm function of the difference between two vectors
float NF2(float *x,float *y){
int i;
float z,sum1=0;
for(i=0;i<N;i++){
	sum1=sum1+pow(y[i]-x[i],2);
}
z=sqrt(sum1);	
return z;
}
