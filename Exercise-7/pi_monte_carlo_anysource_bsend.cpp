// Exercise 7: Monte Carlo Pi with MPI_Bsend (buffered send) + MPI_ANY_SOURCE receive
#include <mpi.h>
#include <iostream>
#include <iomanip>
#include <random>
#include <vector>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const long long TOTAL_TRIALS = 10000000;
    long long local_trials = TOTAL_TRIALS / size;
    if (rank == size - 1) local_trials += TOTAL_TRIALS % size;

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    std::mt19937_64 gen(12345 + rank * 7919);
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    long long local_inside = 0;
    for (long long i = 0; i < local_trials; i++) {
        double x = dist(gen), y = dist(gen);
        if (x * x + y * y <= 1.0) local_inside++;
    }

    if (rank != 0) {
        // Attach a buffer big enough for one message plus MPI's overhead
        int buf_size = sizeof(long long) + MPI_BSEND_OVERHEAD;
        std::vector<char> buffer(buf_size);
        MPI_Buffer_attach(buffer.data(), buf_size);

        // Bsend copies the data into the buffer and returns immediately.
        // local_inside is not modified afterwards.
        MPI_Bsend(&local_inside, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);

        // Detach blocks until the buffered message has been delivered
        void* detached;
        int detached_size;
        MPI_Buffer_detach(&detached, &detached_size);
    } else {
        long long total_inside = local_inside;
        std::cout << "Receive order (MPI_ANY_SOURCE): ";
        for (int i = 1; i < size; i++) {
            long long received;
            MPI_Status status;
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
