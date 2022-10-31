#include<iostream>
#include <chrono>
#include <ctime>
#include <ratio>

using namespace std::chrono;
using namespace std;


int main(int argc, char *argv[]){
    unsigned t0, t1;

    int n = atoi(argv[1]);

    int *matriz1;
    int *matriz2;
    int *result;

 
    t0=clock();

    matriz1 = new int[n*n];
    matriz2 = new int[n*n];
    result = new int[n*n];

    for(int index=0;index<n*n;index++){
        matriz1[index] = rand() % 10;    
        matriz2[index] = rand() % 10;
    }

    for(int filaM2=0;filaM2<n;filaM2){
            int suma = 0;
            for(int col=0;col<n;col++){
                // suma = suma + (matriz1[filaM1*n+col]*matriz2[col*n+filaM2]);
                cout << matriz1[filaM2*n+col];
                cout << matriz2[col*n+filaM2];

            }
            // result[filaM1*n+filaM2] = suma;
        }

    // for(int filaM1=0;filaM1<n;filaM1++){

    //     for(int filaM2=0;filaM2<n;filaM2){
    //         int suma = 0;
    //         for(int col=0;col<n;col++){
    //             // suma = suma + (matriz1[filaM1*n+col]*matriz2[col*n+filaM2]);
    //             cout << matriz1[filaM1*n+col];
    //             cout << matriz2[col*n+filaM2];

    //         }
    //         // result[filaM1*n+filaM2] = suma;
    //     }
    // }

    // for(int i=0;i<n*n;i++){
    //     cout << result[i];
    // }

    // Code to execute
    t1 = clock();

    double duracion = (double(t1-t0)/CLOCKS_PER_SEC);
    
    cout << n;

    return 0;
}


