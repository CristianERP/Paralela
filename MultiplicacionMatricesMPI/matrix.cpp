#include "mpi.h"

using namespace std;

void print_matrix(int matrix[], int n){   
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", matrix[i * n + j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int rank;
    int size;
    int *matrix_a;
    int *matrix_b;
    int *matrix_result;
    int *result;
    int n;
    int sources;
    int num_row;
    int residue;
    double start_wtime; 
    double end_wtime;

    n = atoi(argv[1]);

    matrix_a = (int *)malloc(sizeof(int) * (n * n));
    matrix_b = (int *)malloc(sizeof(int) * (n * n));
    matrix_result = (int *)malloc(sizeof(int) * (n * n));
    result = (int *)malloc(sizeof(int) * (n));
    
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    


    if (size < 2 || n < 2) {
        printf("Se necesitan al menos dos procesos o mayor dimension de matriz. \n");
        MPI_Abort(MPI_COMM_WORLD,  1);
        exit(1);
    }

    sources = size - 1;
    num_row = (int)(n/sources);
    residue = (n % sources);

    if(rank == 0){
        for (int index = 0; index < n * n; index++)
        {
            matrix_a[index] = rand() % 10;
            matrix_b[index] = rand() % 10;
        }

        start_wtime = MPI_Wtime();

        MPI_Bcast(matrix_a, n * n, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(matrix_b, n * n, MPI_INT, 0, MPI_COMM_WORLD);

        for(int source = 1; source <= sources; source++){
            for(int i = 0; i < num_row; i++){
                MPI_Recv(result, n, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

                for (int j = 0; j < n; j++)
                {
                    matrix_result[((source-1)+(i*sources))*n + j] = result[j];
                }
            }
        }

        if(residue > 0){
            for(int source = 1; source <= residue; source++)
            {    
                MPI_Recv(result, n, MPI_INT, source, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

                for (int j = 0; j < n; j++)
                {
                    matrix_result[((source-1)+(num_row*sources))*n + j] = result[j];
                }
                
        }
        }

        printf("Matriz A \n");
        print_matrix(matrix_a, n);

        printf("Matriz B \n");
        print_matrix(matrix_b, n);

        printf("Matriz Resultado \n");
        print_matrix(matrix_result, n);

        end_wtime = MPI_Wtime();
        printf("Tiempo de ejecucion = %f\n", end_wtime - start_wtime);
    }
    else{
        MPI_Bcast(matrix_a, n * n, MPI_INT, 0, MPI_COMM_WORLD);
	    MPI_Bcast(matrix_b, n * n, MPI_INT, 0, MPI_COMM_WORLD);

        for (int i = rank - 1; i < n; i = i + sources)
        {
            for (int row = 0; row < n; row++)
            {
                int sum = 0;
                for (int col = 0; col < n; col++)
                {
                    sum = sum + matrix_a[i * n + col] * matrix_b[col * n + row];
                }
                result[row] = sum;
            }
            
            MPI_Send(result, n, MPI_INT, 0, 10, MPI_COMM_WORLD);
        }

    }
    MPI_Finalize();
    return 0;
}