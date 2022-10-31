#include<iostream>
#include <chrono>
#include <ctime>
#include <ratio>

using namespace std::chrono;
using namespace std;


int main(int argc, char *argv[]){

    int n = atoi(argv[1]);

    unsigned t0, t1;
 
    t0=clock();

    for(int index = 0;index<100000;index++){

    }
    // Code to execute
    t1 = clock();

    double duracion = (double(t1-t0)/CLOCKS_PER_SEC);
    
    cout << n;

    return 0;
}


