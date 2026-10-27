#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Comm parent;
    MPI_Comm_get_parent(&parent);

    if (parent == MPI_COMM_NULL) {
        cout << "I am " << rank << " process from " << size << " processes! My parent is none." << endl;
        int n;
        cin >> n;
        MPI_Comm intercomm;
        MPI_Comm_spawn(argv[0], MPI_ARGV_NULL, n, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &intercomm, MPI_ERRCODES_IGNORE);
    } else {
        cout << "I am " << rank << " process from " << size << " processes! My parent is 0." << endl;
    }

    MPI_Finalize();
    return 0;
}