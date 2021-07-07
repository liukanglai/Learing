#include<stdio.h>
#include<mpi.h>

int main(int argc, char **argv) {
    int mA, nA, nnA;
    int mB, nB, nnB;
    int mC, nC, nnC;
    int A0[mA*nA];
    int B0[120][mB*nB];
    int C0[mA*nB];
    int Ai = mA*nA/32;

    int rank;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    for(int k = 0; k < 32; k++){
        MPI_Bcast(B0[k], mB*nB, MPI_INT, 0, MPI_COMM_WORLD);
    }
    int *recv;
    recv = (int*)malloc(Ai*sizeof(int));
    MPI_Scatter(A0, Ai, MPI_INT, recv, Ai, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0) {
        // 
    }

    MPI_Gather(recv, Ai, MPI_INT, C0, Ai, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Finalize();
	  return 0;
}

