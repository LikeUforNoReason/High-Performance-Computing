/*-------------------------
CPSC High-Performance Computing for Power System Modeling and Simulation
Final Project: Parallel implementation of Cannon’s algorithm
Author: Haotian Deng and Biyang Fu
Date: Apr 2020     Spring 2020 
------------------------------------*/


#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <time.h> 
#include <math.h> 
#include <omp.h>

#define TRUE 1
#define FALSE 0
#define MAX_VALUE_randValue 100

/*------------------------Function declaration------------------------------------*/

//transfer your input string to int
int transferToInt(char *buffer);	

//computation of matrix multiplication
void matrixMultiply(int n, double *a, double *b, double *c); 

//return a randam value betweent[0 , MAX_VALUE_randValue)
double randValue();

//allocates memory for matrix
double *** init_subMatrix(int divideSize, int subMatrixSize, int random);

//free memory for matrix
void deletMatrix(double ***Arr, int divideSize);

//Print matrix
void printMatrix(double ***Arr, int divideSize, int subMatrixSize, int originSize);


/*--------------------------------main function--------------------------------------*/



int main(int argc, char *argv[]) {
	int i, j, k, subMatrixSize, nthread, originSize, divideSize;
	int **divideMatrix, locationA[2], locationB[2], locationC[2];
	double ***A, ***Aa, ***B, ***Bb, ***C;
	double begin, end;

	begin = omp_get_wtime();
	srand(time(NULL));

	//block matrix size
	divideSize = transferToInt(argv[1]);

	//Original Matrix size
	originSize = transferToInt(argv[2]);

	//Number of threads
	nthread = transferToInt(argv[3]);

	if (originSize == -1 || divideSize == -1 || nthread == -1 || originSize % divideSize != 0) {
		printf("Check your arguments again.\n");
	}
	//Submatrix size
	subMatrixSize = originSize / divideSize;

	A = init_subMatrix(divideSize, subMatrixSize, TRUE);
	B = init_subMatrix(divideSize, subMatrixSize, TRUE);
	C = init_subMatrix(divideSize, subMatrixSize, FALSE);
    
	printf("Matrix A:\n");
	printMatrix(A, divideSize, subMatrixSize, originSize);
	printf("-------------------------------------------------------\n");
	printf("Matrix B:\n");
	printMatrix(B, divideSize, subMatrixSize, originSize);
	printf("-------------------------------------------------------\n");

	//Allocates memory for submatrix A and B
	Aa = malloc(divideSize * sizeof(double**));
	if (Aa == NULL) {
		return EXIT_FAILURE;
	}
	Bb = malloc(divideSize * sizeof(double**));
	if (Bb == NULL) {
		return EXIT_FAILURE;
	}
	for (i = 0; i < divideSize; i++) {
		Aa[i] = malloc(divideSize * sizeof(double*));
		if (Aa[i] == NULL) {
			return EXIT_FAILURE;
		}
		Bb[i] = malloc(divideSize * sizeof(double*));
		if (Bb[i] == NULL) {
			return EXIT_FAILURE;
		}
	}

	//Initial submatrix. 
	for (i = 0; i < divideSize; i++) {
		for (j = 0; j < divideSize; j++) {
			locationA[0] = i; //row
			locationB[0] = i; //row
			locationA[1] = j; //column
			locationB[1] = j; //column
			if (locationA[1] - locationA[0] < 0) 
				locationA[1] = locationA[1] - locationA[0] + divideSize;
			else
				locationA[1] = locationA[1] - locationA[0];
			if (locationB[0] - locationB[1] < 0)
				locationB[0] = locationB[0] - locationB[1] + divideSize;
			else
				locationB[0] = locationB[0] - locationB[1];

			Aa[locationA[0]][locationA[1]] = A[i][j];
			Bb[locationB[0]][locationB[1]] = B[i][j];
		}
	}

	//Free pointers of matrix
	for (i = 0; i < divideSize; i++) {
		free(A[i]);
		free(B[i]);
	}
	free(A);
	free(B);

	divideMatrix = malloc(divideSize * sizeof(double*));
	for (i = 0; i < divideSize; i++) {
		divideMatrix[i] = malloc(divideSize * sizeof(double));
		for (j = 0; j < divideSize; j++) {
			divideMatrix[i][j] = -1;
		}
	}
	/*******************************************/
#pragma omp parallel shared( Aa, Bb, C, divideMatrix, divideSize, subMatrixSize, originSize ) private( i, j, k, locationA, locationB, locationC ) num_threads(nthread)
	{
#pragma omp critical
		{
			//create block matrix
			for (i = 0; i < divideSize; i++) {
				for (j = 0; j < divideSize; j++) {
					if (divideMatrix[i][j] < 0) {
						divideMatrix[i][j] = omp_get_thread_num();
						;
						locationA[0] = i;
						locationA[1] = j;
						locationB[0] = i;
						locationB[1] = j;
						locationC[0] = i;
						locationC[1] = j;
						i = divideSize;
						break;
					}
				}
			}
		}

		for (i = 0; i < divideSize; i++) {
			matrixMultiply(subMatrixSize, Aa[locationA[0]][locationA[1]],
					Bb[locationB[0]][locationB[1]], C[locationC[0]][locationC[1]]);
			if (locationA[1] - 1 < 0)
				locationA[1] = locationA[1] - 1 + divideSize;
			else
				locationA[1] = locationA[1] - 1;
			if (locationB[0] - 1 < 0)
				locationB[0] = locationB[0] - 1 + divideSize;
			else
				locationB[0] = locationB[0] - 1;
		}
	}

	for (i = 0; i < divideSize; i++) {
		free(divideMatrix[i]);
	}
	free(divideMatrix);

    printf("Matrix Result:\n");
	printMatrix(C, divideSize, subMatrixSize, originSize);

	deletMatrix(Aa, divideSize);
	deletMatrix(Bb, divideSize);
	deletMatrix(C, divideSize);

	free(Aa);
	free(Bb);
	free(C);

	end = omp_get_wtime();

	printf("\nelapsed time: %f\n", end - begin);
}


/*-----------------------------------------*/



int transferToInt(char *buffer) {
	int i, temp;
	if (strlen(buffer) > 0) {
		for (i = 0; i < strlen(buffer); i++) {
			//TODO
			if (buffer[i] < '0' || buffer[i] > '9')
				return -1;
		}
		temp = atoi(buffer);
		return temp;
	}
	return -1;
}

void matrixMultiply(int n, double *a, double *b, double *c) {
	int i, j, k;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			for (k = 0; k < n; k++) {
				c[i * n + j] += a[i * n + k] * b[k * n + j];
			}
		}
	}
}

double randValue() {
	return rand() / (((double) RAND_MAX + 1) / MAX_VALUE_randValue);
}

double *** init_subMatrix(int divideSize, int subMatrixSize, int random) {
	
	double ***arr;
	int i, j, k;

	arr = malloc(divideSize * sizeof(double**));
	for (i = 0; i < divideSize; i++) {
		arr[i] = malloc(divideSize * sizeof(double*));
		for (j = 0; j < divideSize; j++) {
			arr[i][j] = malloc(subMatrixSize * subMatrixSize * sizeof(double));
			if (random)
				for (k = 0; k < subMatrixSize * subMatrixSize; k++) {
					arr[i][j][k] = randValue();
				}
			else
				for (k = 0; k < subMatrixSize * subMatrixSize; k++) {
					arr[i][j][k] = 0.00;
				}
		}
	}
	return arr;
}

void deletMatrix(double ***Arr, int divideSize) {

	int i, j;

	for (i = 0; i < divideSize; i++) {
		for (j = 0; j < divideSize; j++) {
			free(Arr[i][j]);
		}
		free(Arr[i]);
	}
}

void printMatrix(double ***Arr, int divideSize, int subMatrixSize, int originSize) {
	int rowC = 0, columnC = 0;
	int i, j, k;
	double **resultMatrix;

	resultMatrix = malloc(originSize * sizeof(double*));
	for (i = 0; i < originSize; i++) {
		resultMatrix[i] = malloc(originSize * sizeof(double));
	}

	
	for (i = 0; i < divideSize; i++) {
		for (j = 0; j < divideSize; j++) {
			rowC = i * subMatrixSize;
			columnC = j * subMatrixSize;
			for (k = 0; k < subMatrixSize * subMatrixSize; k++) {
				resultMatrix[rowC][columnC] = Arr[i][j][k];
				if ((k + 1) % (subMatrixSize) == 0) {
					rowC++;
					columnC = j * subMatrixSize;
				} else {
					columnC++;
				}
			}
		}
	}
	printf("-------------------------------------------------------\n");
	for (i = 0; i < originSize; i++) {
		for (j = 0; j < originSize; j++) {
			printf("%f ", resultMatrix[i][j]);
		}
		printf("\n");
	}

	for (i = 0; i < originSize; i++) {
		free(resultMatrix[i]);
	}
	free(resultMatrix);
}
