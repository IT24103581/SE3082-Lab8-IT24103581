
#include <cstdio>
#include <random>
#include <chrono>
#include <mpi.h>

int main(int argc, char** argv)
{
    int rank, size;
    const long long totalTrials = 10000000;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long trialsPerProcess = totalTrials / size;
    long long remainder = totalTrials % size;

    long long localTrials = trialsPerProcess
                          + (rank < remainder ? 1 : 0);

    unsigned long long seed =
        std::chrono::high_resolution_clock::now()
            .time_since_epoch().count() + rank;

    std::mt19937_64 generator(seed);
    std::uniform_real_distribution<double> distribution(-1.0, 1.0);

    long long localInside = 0;

    MPI_Barrier(MPI_COMM_WORLD);
    double startTime = MPI_Wtime();

    for (long long i = 0; i < localTrials; i++)
    {
        double x = distribution(generator);
        double y = distribution(generator);

        if (x * x + y * y <= 1.0)
        {
            localInside++;
        }
    }

    long long totalInside = 0;

    MPI_Reduce(&localInside, &totalInside, 1, MPI_LONG_LONG_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);

    double elapsedTime = MPI_Wtime() - startTime;
    double maxTime = 0.0;

    MPI_Reduce(&elapsedTime, &maxTime, 1, MPI_DOUBLE,
               MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        double pi = 4.0 * totalInside / totalTrials;

        printf("Number of processes: %d\n", size);
        printf("Total trials: %lld\n", totalTrials);
        printf("Points inside circle: %lld\n", totalInside);
        printf("Estimated Pi: %.10f\n", pi);
        printf("Actual Pi: %.10f\n", 3.1415926536);
        printf("Execution time: %.6f seconds\n", maxTime);
    }

    MPI_Finalize();
    return 0;
}
