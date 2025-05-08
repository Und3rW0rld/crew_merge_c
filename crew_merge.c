#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

typedef struct Vi
{
    int i; // índice
    int j; // índice
    char *s; // cadena de caracteres
} Vi;

// Definir la función crew_merge con parámetros punteros A, B y C, y tamaños r y s
void crew_merge(int *A, int *B, int *C, int r, int s)
{
    // Número de procesadores
    int N = 4;

    // Inicializar los arreglos A' y B'
    int Ap[N-1];
    int Bp[N-1];

    printf("Si el programa se ejecutase en paralelo, entonces los arreglos A’ y B’ quedarían así:\n");

    // STEP 1:
    // Sección para dividir y asignar los elementos de A y B a A' y B'
    for (int i = 0; i < N - 1; i++) //Do in parallel
    {
        //P<sub>i</sub> do:
        int posA = (i + 1) * ceil((float)r / N);
        int posB = (i + 1) * ceil((float)s / N);

        if (posA <= r && posB <= s) {
            Ap[i] = A[posA - 1];
            Bp[i] = B[posB - 1];
        }
    }

    // Imprimir los arreglos A' y B'
    printf("A’ = { ");
    for (int i = 0; i < N - 1; i++)
    {
        if (i == N - 2) {
            printf("%d ", Ap[i]);
        } else {
            printf("%d, ", Ap[i]);
        }
    }
    printf("}\n");

    printf("B’ = { ");

    for (int i = 0; i < N - 1; i++)
    {
        if (i == N - 2) {
            printf("%d ", Bp[i]);
        } else {
            printf("%d, ", Bp[i]);
        }
    }
    printf("}\n");

    int sizeAp;
    if ( sizeof(Ap) == 0){
        sizeAp = 0;
    }else {
        sizeAp = sizeof(Ap) / sizeof(Ap[0]);
    }
    int sizeBp;
    if ( sizeof(Bp) == 0){
        sizeBp = 0;
    }else {
        sizeBp = sizeof(Bp) / sizeof(Bp[0]);
    }

    Vi V[ sizeAp + sizeBp ]; // Arreglo para almacenar los resultados de la fusión

    // STEP 2:
    for ( int i = 0; i < N - 1; i++) // Do in parallel
    {
        bool exist = 0;
        int j;
        for ( j = 0; j < N - 1; j++ )
        {   
            if ( Ap[i] < Bp[j] ){
                exist = 1; 
                break;
            }
        }
        if ( exist ){
            V[ i + j ] = (Vi){Ap[i], i+1, "A"}; // Asignar el índice y la cadena "A"
        }else {
            V [ i + N - 1 ] = (Vi){Ap[i], i+1, "A"}; // Asignar el índice y la cadena "A"
        }

    }

    for ( int i = 0; i < N - 1; i++) // Do in parallel
    {
        bool exist = 0;
        int j;
        for ( j = 0; j < N - 1; j++ )
        {   
            if ( Bp[i] < Ap[j] ){
                exist = 1; 
                break;
            }
        }
        if ( exist ){
            V[ i + j ] = (Vi){Bp[i], i+1, "B"}; // Asignar el índice y la cadena "B"
        }else {
            V [ i + N - 1 ] = (Vi){Bp[i], i+1, "B"}; // Asignar el índice y la cadena "B"
        }
    }

    printf("El arreglo V quedaría así:\n");
    int sizeV;
    if ( sizeof(V) == 0){
        sizeV = 0;
    }else {
        sizeV = sizeof(V) / sizeof(V[0]);
    }
    printf("V = { ");
    for (int i = 0; i < sizeV; i++)
    {
        if (i == sizeV - 1) {
            printf("{%d, %d, %s} ", V[i].i, V[i].j, V[i].s);
        } else {
            printf("{%d, %d, %s}, ", V[i].i, V[i].j, V[i].s);
        }
    }
    printf("}\n");


}

int main()
{
    // Arreglo ordenado A
    int A[] = {2, 3, 4, 6, 11, 12, 13, 15, 16, 20, 22, 24};

    // Arreglo ordenado B
    int B[] = {1, 5, 7, 8, 9, 10, 14, 17, 18, 19, 21, 23};

    // Tamaños de los arreglos
    int r = sizeof(A) / sizeof(A[0]);
    int s = sizeof(B) / sizeof(B[0]);

    // Arreglo C para los resultados de la fusión
    int C[r + s];

    // Logs de inicio
    printf("-------------------------------------Impresión del Programa--------------------------------------\n");

    // Llamar a la función crew_merge pasando los arreglos A, B y C y sus tamaños
    crew_merge(A, B, C, r, s);

    return 0;
}
