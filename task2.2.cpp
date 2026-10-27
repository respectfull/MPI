#include <mpi.h>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size, n;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) cin >> n;
    
    for (int i = 1; i < size; i++) {
        if (rank == 0) MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        else if (rank == i) MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    int rows = n / size;
    vector<double> A, B(n * n), C, local_A(rows * n), local_C(rows * n, 0.0);

    if (rank == 0) {
        A.resize(n * n);
        C.resize(n * n);
        for (int i = 0; i < n * n; i++) cin >> A[i];
        for (int i = 0; i < n * n; i++) cin >> B[i];
        
        for (int i = 1; i < size; i++) {
            MPI_Send(B.data(), n * n, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
            MPI_Send(A.data() + i * rows * n, rows * n, MPI_DOUBLE, i, 2, MPI_COMM_WORLD);
        }
        for (int i = 0; i < rows * n; i++) local_A[i] = A[i];
    } else {
        MPI_Recv(B.data(), n * n, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(local_A.data(), rows * n, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                local_C[i * n + j] += local_A[i * n + k] * B[k * n + j];
            }
        }
    }

    if (rank == 0) {
        for (int i = 0; i < rows * n; i++) C[i] = local_C[i];
        for (int i = 1; i < size; i++) {
            MPI_Recv(C.data() + i * rows * n, rows * n, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        for (int i = 0; i < n * n; i++) cout << C[i] << " ";
        cout << endl;
    } else {
        MPI_Send(local_C.data(), rows * n, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}