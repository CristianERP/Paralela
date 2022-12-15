#include <iostream>
#include <string>
#include <math.h>

#include "mpi.h"

using namespace std;

struct Vector3
{
  double e[3] = {0};

  Vector3() {}
  ~Vector3() {}

  inline Vector3(double e0, double e1, double e2)
  {
    this->e[0] = e0;
    this->e[1] = e1;
    this->e[2] = e2;
  }
};

struct OrbitalEntity
{
  double e[7] = {0};

  OrbitalEntity() {}
  ~OrbitalEntity() {}

  inline OrbitalEntity(double e0, double e1, double e2, double e3, double e4, double e5, double e6)
  {
    this->e[0] = e0;
    this->e[1] = e1;
    this->e[2] = e2;
    this->e[3] = e3;
    this->e[4] = e4;
    this->e[5] = e5;
    this->e[6] = e6;
  }
};

int main(int argc, char *argv[])
{
  // Se establecen las condiciones iniciales de la simulación, las "Semillas"
  OrbitalEntity *orbital_entities;
  int N_ASTEROIDS = 0;
  int rank, size;
  double start_wtime; 
  double end_wtime;
  double *result;
  // N_ASTEROIDS es para agregar más cuerpos.
  orbital_entities = (OrbitalEntity *)malloc(sizeof(OrbitalEntity) * (5 + N_ASTEROIDS));
  result = (double *)malloc(sizeof(double) * 6);

  orbital_entities[0] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.989e30};             // a star similar to the sun
  orbital_entities[1] = {778.570e9, 0.0, 0.0, 0.0, 13e3, 0.0, 1898.19e24};    // a planet similar to jupiter
  orbital_entities[2] = {1433.529e9, 0.0, 0.0, 0.0, 9.68e3, 0.0, 568.34e24};  // a planet similar to saturn
  orbital_entities[3] = {2872.463e9, 0.0, 0.0, 0.0, 6.80e3, 0.0, 86.813e24};  // a planet similar to uranus
  orbital_entities[4] = {4495.060e9, 0.0, 0.0, 0.0, 5.43e3, 0.0, 102.413e24}; // a planet similar to neptune

  // más condiciones iniciales, tiempo inicial, final, paso de tiempo, etc
  double t_0 = 0;
  double t = t_0;
  double dt = 86400;
  double t_end = 86400 * 365 * 10; // approximately a decade in seconds
  double BIG_G = 6.67e-11;         // gravitational constant
    
  MPI_Status status;
 
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size); // Obtenemos el numero de procesos en el comunicador global
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  // Propagación de la simulación,

  while (t < t_end) // se repite hasta que se alcanza el tiempo máximo, que es una década.
  {
    
      if(rank == 0){
            
            start_wtime = MPI_Wtime();

            MPI_Bcast(orbital_entities, 5, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            
            MPI_Recv(result, 6, MPI_DOUBLE, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (t == 0 || t == t_end - dt) 
            {
                cout << "\nCuerpo # 1" << endl;
                for(int i = 0; i < 3; i++ ){
                    cout << result[i] << "  ";
                }
                cout << endl;
            }

            MPI_Recv(result, 6, MPI_DOUBLE, 2, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (t == 0 || t == t_end - dt) 
            {
                cout << "\nCuerpo # 2" << endl;
                for(int i = 0; i < 3; i++ ){
                    cout << result[i] << " ";
                }
                cout << endl;
            }

            MPI_Recv(result, 6, MPI_DOUBLE, 3, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (t == 0 || t == t_end - dt) 
            {
                cout << "\nCuerpo # 3" << endl;
                for(int i = 0; i < 3; i++ ){
                    cout << result[i] << " ";
                }
                cout << endl;
            }

            MPI_Recv(result, 6, MPI_DOUBLE, 4, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (t == 0 || t == t_end - dt) 
            {
                cout << "\nCuerpo # 4" << endl;
                for(int i = 0; i < 3; i++ ){
                    cout << result[i] << " ";
                }
                cout << endl;
            }

            MPI_Recv(result, 6, MPI_DOUBLE, 5, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (t == 0 || t == t_end - dt) 
            {
                cout << "\nCuerpo # 5" << endl;
                for(int i = 0; i < 3; i++ ){
                    cout << result[i] << " ";
                }
                cout << endl;
            }

        }
        else{

            MPI_Bcast(orbital_entities, 5, MPI_DOUBLE, 0, MPI_COMM_WORLD);

            for (int m1_idx = rank - 1; m1_idx < 5 + N_ASTEROIDS; m1_idx = m1_idx + size - 1) // se toma una partícula de referencia, m1_idx es el índice de esa partícula
            {
            Vector3 a_g = {0, 0, 0}; // vector aceleración para una partícula

            for (int m2_idx = 0; m2_idx < 5 + N_ASTEROIDS; m2_idx++)
            /* se suman las fuerzas para cada partícula, m2_idx representa la partícula que afecta a la de referencia para cada ocasión */
            {
                if (m1_idx != m2_idx) // la partícula no se afecta a ella misma.
                {
                // vector posición generado. La distancia entre las dos partículas
                Vector3 r_vector;
                // acá se calcula el vector diferencia de posición.
                r_vector.e[0] = orbital_entities[m1_idx].e[0] - orbital_entities[m2_idx].e[0];
                r_vector.e[1] = orbital_entities[m1_idx].e[1] - orbital_entities[m2_idx].e[1];
                r_vector.e[2] = orbital_entities[m1_idx].e[2] - orbital_entities[m2_idx].e[2];

                // se calcula la magnitud del vector r.
                double r_mag = sqrt(r_vector.e[0] * r_vector.e[0] + r_vector.e[1] * r_vector.e[1] + r_vector.e[2] * r_vector.e[2]);
                // se calcula la aceleración generada por la fuerza entre el par de partículas (se excluye la masa de la parícula de referencia, como con el campo eléctrico)
                double acceleration = -1.0 * BIG_G * (orbital_entities[m2_idx].e[6]) / pow(r_mag, 2.0);
                // acá básicamente se calculó la magnitud, ahora se calcula el conjunto de vectores unitarios de la dirección
                Vector3 r_unit_vector = {r_vector.e[0] / r_mag, r_vector.e[1] / r_mag, r_vector.e[2] / r_mag};
                // aceleración por vectores unitarios.
                a_g.e[0] += acceleration * r_unit_vector.e[0];
                a_g.e[1] += acceleration * r_unit_vector.e[1];
                a_g.e[2] += acceleration * r_unit_vector.e[2];
                }
            }

            // se acumulan las velocidades: vxi = vxi-1 + axi * dt
            // todas las aceleraciones generadas sobre m1_idx
            orbital_entities[m1_idx].e[3] += a_g.e[0] * dt;
            orbital_entities[m1_idx].e[4] += a_g.e[1] * dt;
            orbital_entities[m1_idx].e[5] += a_g.e[2] * dt;
            // acá finalizan las operaciones para el conjunto de particulas sobre una particula m1_idx
            }
        }

    // acá se calculan las posiciones para todas las particulas, a partir de las velocidades generadas.
    for (int entity_idx = 0; entity_idx < 5 + N_ASTEROIDS; entity_idx++)
    {
    //   if (t == 0 || t == t_end - dt) 
    //   {
    //     //std::cout << "Particula " << entity_idx << ", tiempo " << t << " : (" << orbital_entities[entity_idx].e[0] << "," << orbital_entities[entity_idx].e[1] << "," << orbital_entities[entity_idx].e[2] << ")" << '\n';
    //   }

      orbital_entities[entity_idx].e[0] += orbital_entities[entity_idx].e[3] * dt;
      orbital_entities[entity_idx].e[1] += orbital_entities[entity_idx].e[4] * dt;
      orbital_entities[entity_idx].e[2] += orbital_entities[entity_idx].e[5] * dt;
    }

    result[0] = orbital_entities[rank-1].e[0];
    result[1] = orbital_entities[rank-1].e[1];
    result[2] = orbital_entities[rank-1].e[2];
    result[3] = orbital_entities[rank-1].e[3];
    result[4] = orbital_entities[rank-1].e[4];
    result[5] = orbital_entities[rank-1].e[5];

    MPI_Send(result, 6, MPI_DOUBLE, 0, 10, MPI_COMM_WORLD);

    // una vez terminado esto, se avanza en el tiempo.
    t += dt;
    // acá debería haber una estructura para guardar las posiciones para cada t.
  }
  MPI_Finalize();
  return 0;
}