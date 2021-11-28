#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdbool.h>
#include "mmio.h"
#include "mmiohighlevel.h"

typedef struct 
{
	VALUE_TYPE *value;
	int *columnindex;
	int *rowpointer;

}SMatrix;

int main(int argc, char ** argv)
{
    /*
	for (int k = 0; k < 120; k++)
	{
		int k1=k+1;
        memset(C0, 0, sizeof(VALUE_TYPE)*mC*nC);

		gettimeofday(&t1, NULL);

        for (int mi = 0; mi < mC; mi++)
        {
            for (int ni = 0; ni < nC; ni++)
            {
                for (int ki = 0; ki < nA; ki++)
                    C0[mi * nB + ni] += A0[mi * nA + ki] * B0[k][ki * nB + ni];
            }
        }
		gettimeofday(&t2,NULL);
        double time_gemm = (t2.tv_sec - t1.tv_sec) * 1000.0 + (t2.tv_usec - t1.tv_usec) / 1000.0;

		gettimeofday(&t1, NULL);

		for (int i = 0; i < mC*nC; i++)
		{
			C0[i] += bias;

			if (C0[i] <= 0)
			{
				C0[i] = 0;
			}
			else if (C0[i] >= 32)
			{
				C0[i] = 32;
			}
		}
		gettimeofday(&t2,NULL);
        double time_biasrelu = (t2.tv_sec - t1.tv_sec) * 1000.0 + (t2.tv_usec - t1.tv_usec) / 1000.0;
		printf("k = %d, GEMM time: %4.5f ms, Bias+ReLU time: %4.5f ms\n",
		       k+1, time_gemm, time_biasrelu);

        free(B0[k]);

		memcpy(A0, C0, (mC*nC)*sizeof(VALUE_TYPE));
	}
	*/

    int rank;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int nodeNum = 4;

    struct timeval t1, t2;
    if (rank == 0) {
	      struct timeval t1, t2, t3, t4;
	      int size1 = 0;
	      int size2 = 0;
	      int *tc1;
	      int *tc2;
	      double bias = -0.3000;

	      int mA;
	      int nA;
	      int nnzA;
	      int isSymmetricA;
	      SMatrix A;

	      int mB;
	      int nB;
	      int nnzB;
	      int isSymmetricB;
	      SMatrix B[120];

	      int mC,nC;
	      int nnzC_golden = 0;

          // load matrix data from file
	      gettimeofday(&t3, NULL);
	      char filename1[]="sparse-images-1024.tsv";
	      mmio_info(&mA, &nA, &nnzA, &isSymmetricA, filename1);
	      A.value=(VALUE_TYPE*)malloc((nnzA)*sizeof(VALUE_TYPE));
	      A.columnindex=(int*)malloc((nnzA)*sizeof(int));
	      A.rowpointer=(int*)malloc((mA+1)*sizeof(int));
	      mmio_data(A.rowpointer, A.columnindex, A.value, filename1);
	      printf("input matrix A: ( %i, %i ) nnz = %i\n", mA, nA, nnzA);


	      char neuronfile1[] = "neuron1024/n1024-l";
	      char neuronfile2[] = ".tsv";
	      char filename3[60];

	      VALUE_TYPE *B0[120];
	      for (int k = 0; k < 120; k++)
	      {
	      	char filenum[5];
	      	int k1=k+1;
	      	snprintf(filenum,sizeof(filenum),"%d",k1);

	      	strcpy(filename3, neuronfile1);
	      	strcat(filename3, filenum);
	      	strcat(filename3, neuronfile2);

	      	mmio_info(&mB, &nB, &nnzB, &isSymmetricB, filename3);
	      	B[k].value=(VALUE_TYPE*)malloc((nnzB)*sizeof(VALUE_TYPE));
	      	B[k].columnindex=(int*)malloc((nnzB)*sizeof(int));
	      	B[k].rowpointer=(int*)malloc((mB+1)*sizeof(int));
	      	mmio_data(B[k].rowpointer, B[k].columnindex, B[k].value, filename3);

	      }
	      gettimeofday(&t4,NULL);
	      double time_load = (t4.tv_sec - t3.tv_sec) * 1000.0 + (t4.tv_usec - t3.tv_usec) / 1000.0;
	      printf("Weight matrix load time: %f ms \n",time_load);


          mC = mA;
	        nC = nB;
	        VALUE_TYPE *C0 =(VALUE_TYPE *)malloc((mC*nC)*sizeof(VALUE_TYPE));

          gettimeofday(&t3, NULL);

        MPI_Send(A, localHighA * localLenA, MPI_INT, myRow * p + i, 1, MPI_COMM_WORLD);
        MPI_Send(A, localHighA * localLenA, MPI_INT, myRow * p + i, 1, MPI_COMM_WORLD);
        MPI_Send(A, localHighA * localLenA, MPI_INT, myRow * p + i, 1, MPI_COMM_WORLD);
        MPI_Send(A, localHighA * localLenA, MPI_INT, myRow * p + i, 1, MPI_COMM_WORLD);
        MPI_Send(A, localHighA * localLenA, MPI_INT, myRow * p + i, 1, MPI_COMM_WORLD);
        MPI_Send(A, localHighA * localLenA, MPI_INT, myRow * p + i, 1, MPI_COMM_WORLD);
        MPI_Send(A, localHighA * localLenA, MPI_INT, myRow * p + i, 1, MPI_COMM_WORLD);

    }

    for (i = 0; i < p; i++) {
        {
            MPI_Send(A, localHighA * localLenA, MPI_INT, myRow * p + i, 1, MPI_COMM_WORLD);
            MPI_Send(B, localHighB * localLenB, MPI_INT, myRow * p + i, 2, MPI_COMM_WORLD);

        }
        {
            MPI_Send(A, localHighA * localLenA, MPI_INT, i * p + myCol, 1, MPI_COMM_WORLD);
            MPI_Send(B, localHighB * localLenB, MPI_INT, i * p + myCol, 2, MPI_COMM_WORLD);

        }

    }

    int *receiveA = (int *) malloc(localLenA * localHighA * sizeof(int));
    int *receiveB = (int *) malloc(localLenB * localHighB * sizeof(int));
    int *resultC = (int *) malloc(localHighA * localLenB * sizeof(int));
    for (i = 0; i < localHighA * localLenB; i++)resultC[i] = 0;

    for (i = 0; i < p; i++) {
        MPI_Recv(receiveA, localHighA * localLenA, MPI_INT,
                 myRow * p + i, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(receiveB, localHighB * localLenB, MPI_INT,
                 i * p + myCol, 2, MPI_COMM_WORLD, &status);

        MatrixMultiply(receiveA, receiveB, resultC, localHighA, localLenA, localLenB);
        MatrixAdd(C, resultC, localHighA, localLenB);

    }

    MPI_Send(C, localHighA * localLenB,
             MPI_INT, 0, rank + 400, MPI_COMM_WORLD);
    if (rank == 0) {
        int **matrixC = (int **) malloc(matrixHighA * sizeof(int *));

        for (i = 0; i < matrixHighA; i++) {
            matrixC[i] = (int *) malloc(matrixLenB * sizeof(int));
        }

        for (i = 0; i < nodeNum; i++) {
            int *receiveCTemp = (int *) malloc(localLenB * localHighA * sizeof(int));
            MPI_Recv(receiveCTemp, localHighA * localLenB, MPI_INT, i, i + 400, MPI_COMM_WORLD, &status);
            l = 0;
            for (j = 0; j < localHighA; j++)
                for (k = 0; k < localLenB; k++) {
                    matrixC[j + (int) (i / p) * localHighA][k + (int) (i % p) * localLenB]
                            = receiveCTemp[l++];
                }
            free(receiveCTemp);
        }
        gettimeofday(&t2, NULL);
        int errcnt = 0;
        for (int i = 0; i < matrixHighA * matrixLenB; ++i) {
            if (matrixC[i / matrixLenB][i % matrixLenB] != C_golden[i]) {
                ++errcnt;
            }
        }
        if (errcnt) {
            printf("MPI not pass! %d in total %d results!\n", errcnt, matrixHighA * matrixLenB);
        } else {
            printf("Pass!\n");
        }
        double times = (t2.tv_usec - t1.tv_usec) / 1000000.0 + (t2.tv_sec - t1.tv_sec) * 1000.0;
        printf("calculate A: %d * %d ,B: %d * %d time is %4.5f ms %4.5f gflops\n ", matrixHighA, matrixLenA,
               matrixHighB, matrixLenB, times,
               2.0 * matrixHighA * matrixLenA * matrixLenB / times / 1000000000);
    }
    MPI_Finalize();
 

	return 0;
}

		
