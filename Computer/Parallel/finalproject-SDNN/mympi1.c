#include<stdio.h>
#include<mpi.h>
#define VALUE_TYPE int

typedef struct 
{
	VALUE_TYPE *value;
	int *columnindex;
	int *rowpointer;

}SMatrix;

int main(int argc, char ** argv) {
    int rank;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int i;
    int mA;
	  int nA;
	  int nnzA;
	  SMatrix A;
    int mmA = mA/32;

	  int mB;
	  int nB;
	  int nnzB;
	  SMatrix B[120];
	  SMatrix B0[120];

	  int mC,nC;
	  int *C;
	  int *C0;
    mC = mA;
	  nC = nB;

	  C0 =(int *)malloc((mmA*nC)*sizeof(int));

	  if(rank == 0) {
	      C =(int *)malloc((mC*nC)*sizeof(int));
    }

        /*
	      A.value=(VALUE_TYPE*)malloc((nnzA)*sizeof(VALUE_TYPE));
	      A.columnindex=(int*)malloc((nnzA)*sizeof(int));
	      A.rowpointer=(int*)malloc((mA+1)*sizeof(int));

	      VALUE_TYPE *B0[120];
	      for (int k = 0; k < 120; k++)
	      {
	      	B[k].value=(VALUE_TYPE*)malloc((nnzB)*sizeof(VALUE_TYPE));
	      	B[k].columnindex=(int*)malloc((nnzB)*sizeof(int));
	      	B[k].rowpointer=(int*)malloc((mB+1)*sizeof(int));
	      }
	      */

        int *sendAV[32];
        int *sendAC[32];
        int *sendAR[32];
        for(int j = 0; j < 32; j++) {
            sendAV[j] = (int*)malloc(((A.rowpointer[(j+1)*mmA] - A.rowpointer[j*mmA]))*sizeof(int));
            for(int k = A.rowpointer[j*mmA], k0 = 0; k < A.rowpointer[(j+1)*mmA]; k++) {
                sendAV[j][k0++] = A.value[k];
            }
            sendAC[j] = (int*)malloc(((A.rowpointer[(j+1)*mmA] - A.rowpointer[j*mmA]))*sizeof(int));
            for(int k = A.rowpointer[j*mmA], k0 = 0; k < A.rowpointer[(j+1)*mmA]; k++) {
                sendAC[j][k0++] = A.columnindex[k];
            }
            sendAR[j] = (int*)malloc((mmA+1)*sizeof(int));
            for(int k = j*mmA, k0 = 0; k <= (j+1)*mmA; k++) {
                sendAR[j][k0++] = A.columnindex[k];
            }
            MPI_Send(sendAR[j], mmA+1, MPI_INT, 0, 1, MPI_COMM_WORLD);
            MPI_Send(sendAV[j], (A.rowpointer[(j+1)*mmA] - A.rowpointer[j*mmA]), MPI_INT, 0, 2, MPI_COMM_WORLD);
            MPI_Send(sendAC[j], (A.rowpointer[(j+1)*mmA] - A.rowpointer[j*mmA]), MPI_INT, 0, 3, MPI_COMM_WORLD);
        }
        
	  for (int k = 0; k < 120; k++) {
	      B0[k].value=(VALUE_TYPE*)malloc((nnzB)*sizeof(VALUE_TYPE));
	      B0[k].columnindex=(int*)malloc((nnzB)*sizeof(int));
	      B0[k].rowpointer=(int*)malloc((mB+1)*sizeof(int));

        /*
        MPI_Send(sendBV[k], nnzB, MPI_INT,  0, 1, MPI_COMM_WORLD);
        MPI_Send(sendBC[k], nnzB, MPI_INT,  0, 2, MPI_COMM_WORLD);
        MPI_Send(sendBR[k], mB+1, MPI_INT,  0, 3, MPI_COMM_WORLD);
        */
        MPI_Bcast(B[k].value, nnzB, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(B[k].columnindex, nnzB, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(B[k].rowpointer, mB+1, MPI_INT, 0, MPI_COMM_WORLD);
	  }

    int *receiveACTemp = (int *) malloc((mmA+1) * sizeof(int));
    int *receiveAVTemp = (int *) malloc((A.rowpointer[(i+1)*mmA] - A.rowpointer[i*mmA]) * sizeof(int));
    int *receiveARTemp = (int *) malloc((A.rowpointer[(i+1)*mmA] - A.rowpointer[i*mmA]) * sizeof(int));
    MPI_Recv(receiveACTemp, mmA+1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(receiveAVTemp, (A.rowpointer[i*mmA+1] - A.rowpointer[(i-1)*mmA+1]), MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
    MPI_Recv(receiveARTemp, (A.rowpointer[i*mmA+1] - A.rowpointer[(i-1)*mmA+1]), MPI_INT, 0, 3, MPI_COMM_WORLD, &status);
 
    MPI_Gather(C, mmA*nC, MPI_INT, C0, mmA*nC, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Finalize();

}
