#include <stdio.h>
#include<stdlib.h>
#include<pthread.h> //library to use the API for threads
#include <sys/time.h>

//global variables which are used by all the threads

int** matA, ** matB, ** matC, ** matC_single;
int row1, col1, row2, col2;
int num_threads; //number of threads


void* compute(void* arg) //function to compute the matrices mutiplication
{
	int* refValue = (int*)arg; //equalizing the value of the group of rows refered by the thread (e.g. first 4 rows, then the next 4 rows etc.)
	int i, j, k;
	for (int i = *refValue; i < (*refValue + (row1 / num_threads)); i++) //this will compute the rows of matrix C which are divided by number of threads
	{
		for (int j = 0; j < col2; j++)
		{
			for (int k = 0; k < col1; k++)
				matC[i][j] = matC[i][j] + matA[i][k] * matB[k][j];
		}
	}
}



int main()
{

		int tmp;
		int i, j, k, r; //variables used in loops

		//--------------------------------------------------------//

		//taking the raws and columns of two matrices

		printf("Please insert the number of rows of Matrix 1    : ");
		scanf("%d",&row1);

		printf("Please insert the number of columns of Matrix 1 : ");
 		scanf("%d",&col1);

		printf("Please insert the number of rows of Matrix 2    : ");
		scanf("%d",&row2);

		printf("Please insert the number of columns of Matrix 2 : ");
		scanf("%d",&col2);

		//--------------------------------------------------------//

		if (col1 != row2) //checking whether the two matrices are multiplicable
		{
			printf("\nYour given matrices cannot be multiplied with each other. \n ");
		}

		else //if multiplicable
		{
		    //declaring the global pointers as 2d arrays dynamically to store the matrices
			matA = (int**)malloc(row1 * sizeof(int*));
			for (int i = 0; i < row1; i++)
			{
				matA[i] = (int*)malloc(col1 * sizeof(int*));
			}

			matB = (int**)malloc(row2 * sizeof(int*));
			for (int i = 0; i < row2; i++)
			{
				matB[i] = (int*)malloc(col2 * sizeof(int*));
			}

			matC = (int**)malloc(row1 * sizeof(int*));
			for (int i = 0; i < row1; i++)
			{
				matC[i] = (int*)malloc(col2 * sizeof(int*));
			}

			matC_single = (int**)malloc(row1 * sizeof(int*));
			for (int i = 0; i < row1; i++)
			{
				matC_single[i] = (int*)malloc(col2 * sizeof(int*));
			}
            //--------------------------------------------------------//

            printf("\n\+++++++++++++++++++++++++\n");

			int op; //to check for the user preference of input using the text file or randomly filling the matrices

			printf("\nSelect the option\n\n");
			printf("Fill the matrix with the values given in a file - Enter 1\n");
			printf("Fill the matrix with random values - Enter 2\n");
			scanf("%d", &op);
            printf("\n\+++++++++++++++++++++++++\n");

			if (op == 1) //input using text files
			{
				FILE* file1 = fopen("matA.txt", "r");
				FILE* file2 = fopen("matB.txt", "r");

				printf("\nMatrix 1\n\n");

				for (int t = 0; t < row1; t++)
				{
					for (int u = 0; u < col1; u++)
					{
						fscanf(file1, "%d", &tmp); //reading and storing the txt file data in to the matrix A
						printf("%d ", tmp);
						matA[t][u] = tmp;
					}
					printf("\n");
				}

				fclose(file1);

				printf("\nMatrix 2\n\n");
				for (int t = 0; t < row2; t++)
				{
					for (int u = 0; u < col2; u++)
					{
						fscanf(file2, "%d", &tmp); //reading and storing the txt file data in to the matrix A
						printf("%d ", tmp);
						matB[t][u] = tmp;
					}
					printf("\n");
				}
				fclose(file2);
			}


			else
			{
				for (i = 0; i < row1; i++)
				{
					for (j = 0; j < col1; j++)
						matA[i][j] = rand() % 10; //randomly filling the matrices
				}

				for (i = 0; i < row2; i++)
				{
					for (j = 0; j < col2; j++)
						matB[i][j] = rand() % 10; //randomly filling the matrices
				}
			}

			for (i = 0; i < row1; i++)
			{
				for (j = 0; j < col2; j++)
				{
					matC[i][j] = 0; //initializing the matrices to get rid of garbage values
					matC_single[i][j] = 0; //initializing the matrices to get rid of garbage values
				}
			}

			printf("\nSingle threaded \n"); //single threaded is serially executing in the main thread

			struct timeval stop, start, stopMulti, startMulti; //these variables are the flags used for measuring the computational time

			gettimeofday(&start, NULL); //start time of main thread multiplication

			for (int i = 0; i < row1; i++)
			{
				for (int j = 0; j < col2; j++)
				{
					for (int k = 0; k < col1; k++)
					{
						matC_single[i][j] = matC_single[i][j] + matA[i][k] * matB[k][j]; //matrix multipication algorithm

					}
					printf("%d ", matC_single[i][j]);
				}
				printf("\n");
			}

			gettimeofday(&stop, NULL); //end time of main thread multiplication

            printf("\n\++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
			printf("\nPlease enter number of threads (Multiple of row 1): "); //user prefered amount of threads can be used

			scanf("%d", &num_threads);

			pthread_t tid[num_threads]; //creating an array of threads

			int rownos[num_threads]; //number of threads are allocated for the number of rows

			gettimeofday(&startMulti, NULL); //start time of main thread multiplication

			for (i = 0; i < num_threads; i++)
			{
				rownos[i] = i * (row1 / num_threads); //threads are grouped according to the number of rows in matrix C
				pthread_create(&tid[i], NULL, compute, &rownos[i]); //creating the threads and passing the group of rows of into the compute function
			}

			//threads are subjected to wait until other threads complete the action
			for (i = 0; i < num_threads; i++)
				pthread_join(tid[i], NULL);

			gettimeofday(&stopMulti, NULL); //end time of main thread multiplication


			for (i = 0; i < row1; i++) //printing the resultant matrix
			{
				for (j = 0; j < col2; j++)
				{
					printf("%d", matC[i][j]);
					printf(" ");
				}
				printf("\n");
			}
            printf("\n\++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

			//calculating the computational time for the parallel multiplication using threads
            printf("\nComputation time of single thread : %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec); //measuring and outputting the computational time

			printf("\nComputation time of multi threads : %lu us\n", (stopMulti.tv_sec - startMulti.tv_sec) * 1000000 + stopMulti.tv_usec - startMulti.tv_usec);

            printf("\n\++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");

		}
		system("pause");

}
