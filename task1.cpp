#include <mpi.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    string word = "Parallelization";
    char local_char;
    
    MPI_Scatter(word.data(), 1, MPI_CHAR, &local_char, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    
    string reversed_word = word;
    if (rank == 0) {
        for (int i = 0; i < size / 2; i++) {
            swap(reversed_word[i], reversed_word[size - i - 1]);
        }
    }
    
    MPI_Scatter(reversed_word.data(), 1, MPI_CHAR, &local_char, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    
    MPI_Finalize();
    return 0;
}