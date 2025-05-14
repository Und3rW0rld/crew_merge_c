#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
//Nicolas Orozco y Santiago Acevedo

#define N 4  // Número de procesadores

typedef struct Vi {
    int value;   // el valor pivote
    int index;   // índice de subsecuencia
    char origin; // 'A' o 'B'
} Vi;

void imprimir_arreglo(const char *nombre, int *arr, int tamaño) {
    printf("%s = { ", nombre);
    for (int i = 0; i < tamaño; i++) {
        printf(i==tamaño-1 ? "%d " : "%d, ", arr[i]);
    }
    printf("}\n");
}

void crew_merge(int *A, int *B, int *C, int r, int s) {
    // STEP 1: pivotes
    int Ap[N-1], Bp[N-1];
    for (int i = 0; i < N-1; i++) {
        int posA = (i+1) * ceil((float)r/N);
        int posB = (i+1) * ceil((float)s/N);
        Ap[i] = A[posA-1];
        Bp[i] = B[posB-1];
    }
    printf("Si el programa se ejecutase en paralelo, entonces los arreglos A’ y B’ quedarían así:\n");
    imprimir_arreglo("A’", Ap, N-1);
    imprimir_arreglo("B’", Bp, N-1);
    
    // STEP 2: mezclar pivotes en V
    Vi V[2*(N-1)];
    for (int i = 0; i < N-1; i++) {
        int j = 0;
        while (j < N-1 && Ap[i] >= Bp[j]) j++;
        int pos = (j < N-1) ? (i + j) : (i + (N-1));
        V[pos] = (Vi){Ap[i], i+1, 'A'};
    }
    for (int i = 0; i < N-1; i++) {
        int j = 0;
        while (j < N-1 && Bp[i] >= Ap[j]) j++;
        int pos = (j < N-1) ? (i + j) : (i + (N-1));
        V[pos] = (Vi){Bp[i], i+1, 'B'};
    }
    printf("\nEl arreglo V quedaría así:\nV = { ");
    for (int i = 0; i < 2*(N-1); i++) {
        printf(i==2*(N-1)-1 ? "{%d, %d, %c} " : "{%d, %d, %c}, ", V[i].value, V[i].index, V[i].origin);
    }
    printf("}\n");
    
    // STEP 3.1 y 3.2: construir Q
    int Q[N][2]; // pares (x,y)
    Q[0][0] = 1; Q[0][1] = 1;
    for (int i = 2; i <= N; i++) {
        Vi pivot = V[2*i - 3];
        if (pivot.origin == 'A') {
            int j = 0;
            while (j < s && B[j] <= pivot.value) j++;
            Q[i-1][0] = pivot.index * ceil((float)r/N);
            Q[i-1][1] = j + 1;
        } else {
            int j = 0;
            while (j < r && A[j] <= pivot.value) j++;
            Q[i-1][0] = j + 1;
            Q[i-1][1] = pivot.index * ceil((float)s/N);
        }
    }
    printf("\nEl arreglo Q quedaría así:\n");
    for (int i = 0; i < N; i++) {
        printf("Posición %d: (%d, %d)\n", i+1, Q[i][0], Q[i][1]);
    }
    
    // STEP 3.3: merge secuencial por procesador
    printf("\n");
    for (int p = 1; p <= N; p++) {
        int x = Q[p-1][0];
        int y = Q[p-1][1];
        int x_end = (p < N ? Q[p][0] - 1 : r);
        int y_end = (p < N ? Q[p][1] - 1 : s);
        int lenA = x_end >= x ? x_end - x + 1 : 0;
        int lenB = y_end >= y ? y_end - y + 1 : 0;
        int *subA = &A[x-1];
        int *subB = &B[y-1];
        
        printf("----------\n");
        printf("Al procesador %d le correspondería mezclar los siguientes elementos:\n", p);
        for (int i=0; i<lenA; i++) printf(i==lenA-1 ? "%d" : "%d, ", subA[i]);
        printf("\nY\n");
        for (int i=0; i<lenB; i++) printf(i==lenB-1 ? "%d" : "%d, ", subB[i]);
        printf("\nY hubiera generado:\n");
        
        int iA = 0, iB = 0, total = lenA + lenB;
        int *temp = malloc(total * sizeof(int));
        int k = 0;
        while (iA < lenA && iB < lenB) temp[k++] = subA[iA] <= subB[iB] ? subA[iA++] : subB[iB++];
        while (iA < lenA) temp[k++] = subA[iA++];
        while (iB < lenB) temp[k++] = subB[iB++];
        for (int i=0; i<total; i++) printf(i==total-1 ? "%d" : "%d, ", temp[i]);
        printf("\n");
        
        int posC = x + y - 2;
        for (int i=0; i<total; i++) C[posC + i] = temp[i];
        free(temp);
    }
    printf("----------\nY el resultado final hubiera sido:\n----------\n");
    imprimir_arreglo("C", C, r+s);
    printf("-------------------------------------Fin Impresión del Programa--------------------------------------\n");
}

int main() {
    int A[] = {2,3,4,6,11,12,13,15,16,20,22,24};
    int B[] = {1,5,7,8,9,10,14,17,18,19,21,23};
    int r = sizeof(A)/sizeof(A[0]);
    int s = sizeof(B)/sizeof(B[0]);
    int C[r+s];
    printf("-------------------------------------Impresión del Programa--------------------------------------\n");
    crew_merge(A, B, C, r, s);
    return 0;
}
