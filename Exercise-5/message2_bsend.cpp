
#include <cstdio>
#include <cstdlib>
#include <mpi.h>

int main(void)
{
    int rank;
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char name[30];
    int len;
    MPI_Get_processor_name(name, &len);

    int x[10], y[10];

    if (rank == 1) {
        for (int r = 0; r < 10; r++)
            x[r] = 10 * r;

        int bufferSize = 10 * sizeof(int) + MPI_BSEND_OVERHEAD;
        void* buffer = malloc(bufferSize);

        if (buffer == NULL) {
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        MPI_Buffer_attach(buffer, bufferSize);

        printf("Sending buffered message from rank 1 to rank 3\n");

        MPI_Bsend(x, 10, MPI_INT, 3, 0, MPI_COMM_WORLD);

        MPI_Buffer_detach(&buffer, &bufferSize);
        free(buffer);
    }
    else if (rank == 3) {
        MPI_Recv(y, 10, MPI_INT, 1, 0,
                 MPI_COMM_WORLD, &status);

        printf("In rank 3, the values of y are:\n");

        for (int r = 0; r < 10; r++)
            printf(" %d ", y[r]);

        printf("\n");
    }
    else {
        printf("Just a normal process from rank %d machine %s\n",
               rank, name);
    }

    MPI_Finalize();
    return 0;
}
