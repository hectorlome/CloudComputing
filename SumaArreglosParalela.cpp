// SolucionParalela.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.

#include <iostream>
#include <ctime> //Libreria para acceder a time
#include <set> //Libreria para acceder a std::set
#include <iomanip> //Libreria para formatear datos de salida
#include <omp.h> //OpenMP libreria

//#define N 1000000 //Cantidad de elementos en los arreglos
//#define chunk 1000 //Tamaño de los pedazos del arreglo para cada hilo
#define mostrar 20 //Cantidad de datos a mostrar

int main()
{
    int N; //Tamaño de los pedazos del arreglo para cada hilo
    std::cout << "Suma arreglos en paralelo con OpenMP\n\n";

    //Pedir N arreglos al usuario, regresar error en caso de numero invalido
    std::cout << "Ingrese la cantidad de elementos (ej. 1000) en el arreglo, debe ser un valor positivo: ";
    std::cin >> N;

    if (N <= 0)
    {
        std::cout << "Cantidad no valida." << std::endl;
        return 1;
    }

    float* a = new float[N];
    float* b = new float[N];
    float* c = new float[N];
    int i;
    srand((unsigned int)time(NULL)); //Asegurar que se generen diferentes numeros random cada ejecucion

    //Inicializacion de los arreglos
    for (i = 0; i < N; i++)
    {
        a[i] = (float)(rand() % N) * 1.31f; //Genera un numero random entre 0 y N y lo multiplica por 1.31 para tener un valor float
        b[i] = (float)(rand() % N) * 1.25f; //Genera un numero random entre 0 y N y lo multiplia por 1.25 para tener un valor float
    }

    //Genera un conjunto de indices aleatorios ordenados y unicos para mostrar resultados
    std::set<int> rand_idx;
    while (rand_idx.size() < mostrar)
        rand_idx.insert(rand() % N);

    std::cout << "\nSe muestran " << mostrar << " resultados aleatorios: \n" << std::endl;

    //Header de la tabla de salida
    std::cout << std::right
        << std::setw(10) << "Indice" << " | "
        << std::setw(10) << "ID Hilo" << " | "
        << std::setw(10) << "Arreglo 1" << " | "
        << std::setw(10) << "Arreglo 2" << " | "
        << std::setw(10) << "Resultado" << std::endl;

    //parallel - crea los hilos para trabajar paralelamente 
    //shared - comparte las variables para no crear copias de datos para cada hilo
    //schedule(static) - reparte el trabajo de manera equitativa
#pragma omp parallel for \
    shared(a,b,c, N) private(i)\
    schedule(static)

    for (i = 0; i < N; i++)
    {
        //Suma de arreglos c = a + b
        c[i] = a[i] + b[i];

        //Si el indice i se encuentra en la coleccion rand_idx imprime los datos
        if (rand_idx.count(i))
        {
#pragma omp critical //Para evitar que los hilos escriban al mismo tiempo
            //Mostrar resultados
            std::cout << std::internal
                << std::setw(10) << i << " | "
                << std::setw(10) << omp_get_thread_num() << " | "
                << std::setw(10) << a[i] << " | "
                << std::setw(10) << b[i] << " | "
                << std::setw(10) << c[i] << std::endl;
        }
    }

    //Eliminar arreglos para evitar memory leaks
    delete[] a;
    delete[] b;
    delete[] c;

}