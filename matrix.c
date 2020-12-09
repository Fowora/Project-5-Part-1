#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int matA[MAX][MAX]; 
int matB[MAX][MAX]; 

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX]; 
int matProductResult[MAX][MAX]; 

int MAX;
int turn_zero = 0, first_turn = 0, second_turn = 0; 
void fillMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            matrix[i][j] = rand()%10+1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for(int i = 0; i<MAX; i++) {
        for(int j = 0; j<MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) { // pass in the number of the ith thread
    int i, j;
    int k = turn_zero++;
    for (i = k; i < (k+1); i++){
      for (j = 0; j < MAX; j++){
        *((int*)matSumResult + i*MAX + j) = *(matA + i*MAX + j) - *(matB + i*MAX + j);
      }
    }
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) { // pass in the number of the ith thread
    int i, j;
    int k = first_turn++;
    for (i = k; i < (k+1); i++){
      for (j = 0; j < MAX; j++){
        *((int*)matDiffResult + i*MAX + j) = *(matA + i*MAX + j) - *(matB + i*MAX + j);
      }
    }
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) { // pass in the number of the ith thread
    int i, j, l;
    int k = second_turn++;
    for (i = k; i < (k+1); i++){
      for (j = 0; j < MAX; j++){
        for (l = 0; l < MAX; l++){
        *((int*)matProductResult + i*MAX + l) += *(matA + i*MAX + l) * *(matB + i*MAX + j);
        }
      }
  }
}  

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() {
    srand(time(0));  // Do Not Remove. Just ignore and continue below.
    
    // 0. Get the matrix size from the command line and assign it to MAX
    printf("What NxN matrix size would you like? Please enter an integer N.\n");
    scanf("%d", &MAX);
  
    // 
    matA = (int *)malloc(MAX * MAX * sizeof(int));
    matB = (int *)malloc(MAX * MAX * sizeof(int));
    matSumResult = (int *)malloc(MAX * MAX * sizeof(int));
    matDiffResult = (int *)malloc(MAX * MAX * sizeof(int));
    matProductResult = (int *)malloc(MAX * MAX * sizeof(int));
  
    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);
  
    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);
    
    // 3. Create pthread_t objects for our threads.
    pthread_t threads[MAX];
    // 4. Create a thread for each cell of each matrix operation.
      int i;
      for (i=0; i < MAX; i++){
          int* arg;
          pthread_create(&threads[i], NULL, computeSum, (void*)(arg));
          pthread_create(&threads[i], NULL, computeDiff, (void*)(arg));
          pthread_create(&threads[i], NULL, computeProduct, (void*)(arg));
      }
  
    // 5. Wait for all threads to finish.
      for (i = 0; i < MAX*MAX; i++){
        pthread_join(threads[i], NULL);
      }
    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);
    free(matA);
    free(matB);
    free(matSumResult);
    free(matDiffResult);
    free(matProductResult);
    return 0;
  
}