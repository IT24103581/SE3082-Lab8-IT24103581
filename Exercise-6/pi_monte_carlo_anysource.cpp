// Exercise 6: Monte Carlo Pi with MPI_Send + MPI_Recv using MPI_ANY_SOURCE
#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <random>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const long long TOTAL_TRIALS = 10000000;
    long long local_trials = TOTAL_TRIALS / size;
    if (rank == size - 1) local_trials += TOTAL_TRIALS % size;  // leftover trials

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    // Each rank uses its own seed so the random streams differ
    std::mt19937_64 gen(12345 + rank * 7919);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    long long local_inside = 0;
    for (long long i = 0; i < local_trials; i++) {
        double x = dist(gen), y = dist(gen);
        if (x * x + y * y <= 1.0) local_inside++;
    }

    if (rank != 0) {
        // Workers send their count to rank 0 (tag 0)
        MPI_Send(&local_inside, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    } else {
        long long total_inside = local_inside;
        std::cout << "Receive order (MPI_ANY_SOURCE): ";
        for (int i = 1; i < size; i++) {
            long long received;
            MPI_Status status;
            // Accept from whichever worker finishes first
            MPI_Recv(&received, 1, MPI_LONG_LONG, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            std::cout << "rank " << status.MPI_SOURCE << " ";
            total_inside += received;
        }
        std::cout << "\n";

        double end = MPI_Wtime();
        double pi = 4.0 * (double)total_inside / (double)TOTAL_TRIALS;

        std::cout << std::fixed << std::setprecision(10);
        std::cout << "Number of processes: " << size << "\n";
        std::cout << "Total trials: " << TOTAL_TRIALS << "\n";
        std::cout << "Points inside circle: " << total_inside << "\n";
        std::cout << "Estimated Pi: " << pi << "\n";
        std::cout << "Actual Pi: 3.1415926536\n";
        std::cout << "Execution time: " << end - start << " seconds\n";
    }

    MPI_Finalize();
    return 0;
}
