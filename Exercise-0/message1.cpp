
#include <cstdio>
#include <mpi.h>

int main(int argc, char** argv)
{
    int rank;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int x[10], y[10];

    if (rank == 1) {
        for (int r = 0; r < 10; r++)
            x[r] = 10 * r;

        printf("Sending message from rank 1 to rank 3\n");

        MPI_Ssend(x, 10, MPI_INT, 3, 0, MPI_COMM_WORLD);
    }
    else if (rank == 3) {
        MPI_Recv(y, 10, MPI_INT, 1, 0,
                 MPI_COMM_WORLD, &status);

        printf("Values received by rank 3:\n");

        for (int r = 0; r < 10; r++)
            printf("%d ", y[r]);

        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
