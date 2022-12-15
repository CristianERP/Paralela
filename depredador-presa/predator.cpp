#include <iostream>
#include <stdlib.h>
#include "mpi.h"
using namespace std;

const int  N = 19;                  //Area
enum state { EMPTY , RABBIT , WOLF, STATES };

const int DRAB = 4, DWOLF = 8, CYCLES = 10;

class Living;                       
typedef Living* world[N][N];

class Living {                      

public:
   virtual state  who() = 0;        
   virtual Living*  next(world w) = 0;

protected:
   int  row, column;               
   void sums(world w,int sm[]);
};


void Living::sums(world w, int sm[]) {
   int  i, j;

   sm[EMPTY] = sm[RABBIT] = sm[WOLF] = 0;
   for (i = -1; i <= 1; ++i)
      for ( j = -1; j <= 1; ++j)
         sm[w[row + i][column +j] -> who()]++;
}


class Wolf : public Living {  //Clase lobo depredador

public:
   Wolf(int r, int c, int a = 0) : age(a)
      { row = r; column = c; }
   state  who() { return WOLF; }   
   Living*  next(world w);

protected:
   int  age;                      
};


class Rabbit : public Living { //Clase conejo presa

public:
   Rabbit(int r, int c, int a = 0) : age(a)
      { row = r; column = c; }
   state  who() { return RABBIT; }
   Living*  next(world w);

protected:
   int  age;
};


class Empty : public Living {

public:
   Empty(int r, int c) { row = r; column = c; }
   state  who() { return EMPTY; }
   Living*  next(world w);
};


Living* Rabbit::next(world w) {

   int  sum[STATES];

   sums(w, sum);
   if (sum[WOLF] >= sum[RABBIT] )       
      return (new Empty(row, column));
   else if (age > DRAB)                
      return (new Empty(row, column));
   else
      return (new Rabbit(row, column, age + 1));
}

Living* Wolf::next(world w) {

   int  sum[STATES];

   sums(w, sum);
   if (sum[WOLF] > 5)                   
      return (new Empty(row, column));
   else if (age > DWOLF)                  
      return (new Empty(row, column));
   else
      return (new Wolf(row, column, age + 1));
}

Living* Empty::next(world w) {  

   int  sum[STATES];

   sums(w, sum);
   if  ( (sum[WOLF] > 1) && (sum[RABBIT] >1) )
      return (new Wolf(row, column));
   else if ( (sum[RABBIT] > 1) )
      return (new Rabbit(row, column));
   else
      return (new Empty(row, column));
}


void init(world w) {  

   int  i, j;

   for (i = 0; i < N; ++i)
      for (j = 0; j < N; ++j)
         w[i][j] = new Empty(i,j);
}



void update(world w_new, world w_old) {

   int  i, j;

   for (i = 1; i < N - 1; ++i)      
      for (j = 1; j < N - 1; ++j)
         w_new[i][j] = w_old[i][j] -> next(w_old);
}


void dele(world w) { 

   int  i, j;

   for (i = 1; i < N - 1; ++i)
      for (j = 1; j < N - 1; ++j)
         delete(w[i][j]);
}


void random_eden(world w) {
   srand(time(NULL));
   int i,j,rand_n;

   for (i = 2; i < N - 2; ++i)
      for (j = 2; j < N - 2; ++j) {
          rand_n= rand(); 
          rand_n= rand_n%5;
          switch(rand_n) {
              case 0: w[i][j] = new Empty(i, j);  break;
              case 1: case 2:
              case 3: w[i][j] = new Rabbit(i, j); break;
              case 4: w[i][j] = new Wolf(i, j);    break;
          }
     }
}

void print_state(world w) {

   int i, j, a;

   for(i = 0; i < N; ++i) {
      cout << endl;
      for(j = 0; j < N; ++j) {
	     a = (int)(w[i][j] -> who());
	     switch(a) {
	        case EMPTY:  cout << "."; break;
	        case RABBIT: cout << "r"; break;
	        case WOLF:    cout << "W"; break;
		 }
      }
   }
   cout << endl;
}


int main(int argc, char *argv[]) {

   world  odd, even;
   int i;

   init(odd);  init(even);
   random_eden(even);
   print_state(even);             
   
   int rank, size, startVal, endVal;
   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtenemos el numero de procesos en el comunicador global
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Status status;
   startVal = (CYCLES * rank / size);
   endVal = CYCLES * (rank + 1)/size;

   for (i = startVal; i < endVal; ++i) {    //Simulacion
      if (i % 2) {
         update(even, odd);
         print_state(even);
         dele(odd);
      }
      else {
         update(odd, even);
         print_state(odd);
         dele(even);
      }
   }
   MPI_Finalize();
   return 0;
}