#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size, n;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        cin >> n;
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int rows = n / size;
    vector<double> A, B(n * n), C, local_A(rows * n), local_C(rows * n, 0.0);

    if (rank == 0) {
        A.resize(n * n);
        C.resize(n * n);
        for (int i = 0; i < n * n; i++) cin >> A[i];
        for (int i = 0; i < n * n; i++) cin >> B[i];
    }

    MPI_Bcast(B.data(), n * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(A.data(), rows * n, MPI_DOUBLE, local_A.data(), rows * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                local_C[i * n + j] += local_A[i * n + k] * B[k * n + j];
            }
        }
    }

    MPI_Gather(local_C.data(), rows * n, MPI_DOUBLE, C.data(), rows * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int i = 0; i < n * n; i++) cout << C[i] << " ";
        cout << endl;
    }

    MPI_Finalize();
    return 0;
}