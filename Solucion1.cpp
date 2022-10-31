#include <iostream>
#include <chrono>
#include <ctime>
#include <ratio>

using namespace std::chrono;
using namespace std;

int main(int argc, char *argv[])
{

    unsigned t0, t1;

    t0 = clock();

    int n = atoi(argv[1]);

    int *matriz1;
    int *matriz2;
    int *result;

    matriz1 = (int *)malloc(sizeof(int) * (n * n));
    matriz2 = (int *)malloc(sizeof(int) * (n * n));
    result = (int *)malloc(sizeof(int) * (n * n));

    for (int index = 0; index < n * n; index++)
    {
        matriz1[index] = rand() % 100;
        matriz2[index] = rand() % 100;
    }

    for (int i = 0; i < n; i++)
    {
        for (int fila = 0; fila < n; fila++)
        {
            int suma = 0;
            for (int col = 0; col < n; col++)
            {
                suma = suma + matriz1[i * n + col] * matriz2[col * n + fila];
            }
            result[i * n + fila] = suma;
        }
    }

    cout << "Matriz A" << endl;

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << matriz1[i * n + j] << " ";
        }

        cout << endl;
    }

    cout << "Matriz B" << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << matriz2[i * n + j] << " ";
        }

        cout << endl;
    }

    cout << "Resultado" << endl;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << result[i * n + j] << " ";
        }

        cout << endl;
    }
    t1 = clock();

    double duracion = (double(t1 - t0) / CLOCKS_PER_SEC);

    cout << "Tiempo de ejecucion malloc: " << duracion;

    delete[] matriz1;
    delete[] matriz2;
    delete[] result;

    return 0;
}
