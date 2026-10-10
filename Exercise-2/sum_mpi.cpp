
#include <cstdio>
#include <mpi.h>

int main(int argc, char** argv)
{
    int rank, size;
    const long long N = 10000000;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long numbersPerProcess = N / size;
    long long remainder = N % size;

    long long start = rank * numbersPerProcess
                    + (rank < remainder ? rank : remainder) + 1;

    long long count = numbersPerProcess
                    + (rank < remainder ? 1 : 0);

    long long end = start + count - 1;
    long long localSum = 0;

    MPI_Barrier(MPI_COMM_WORLD);
    double startTime = MPI_Wtime();

    for (long long i = start; i <= end; i++)
    {
        localSum += i;
    }

    long long totalSum = 0;

    MPI_Reduce(&localSum, &totalSum, 1, MPI_LONG_LONG_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);

    double elapsedTime = MPI_Wtime() - startTime;
    double maxTime = 0.0;

    MPI_Reduce(&elapsedTime, &maxTime, 1, MPI_DOUBLE,
               MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Number of processes: %d\n", size);
        printf("Numbers: 1 to %lld\n", N);
        printf("Total sum: %lld\n", totalSum);
        printf("Execution time: %.6f seconds\n", maxTime);
    }

    MPI_Finalize();
    return 0;
}
