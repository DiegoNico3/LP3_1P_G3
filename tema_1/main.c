/* 
Este programa utiliza un árbol binario completo y el algoritmo mergesort para ordenar un array de enteros en paralelo, 
manejando múltiples procesos y mostrando la estructura y resultados del árbol.
   Grupo 3:
   * Maria Luján Melgarejo Acosta       5111244 
   * Diego Nicolas Moreno Saldivar      5689969
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define SHM_ARRAY "arreglo_compartido"

typedef struct {
    int counter_process;
    int size;
    int shared_array[];
} Shared;

void printArray(int arr[], int size) {
    printf("{");
    for (int i = 0; i < size - 1; i++)
        printf("%d,", arr[i]);
    printf("%d}", arr[size - 1]);
}

void printArrayInTree(int arr[], int start, int end, int totalDepth, int maxDepth) {
    for (int i = 0; i < pow(2, maxDepth); i++) {
        for (int k = pow((totalDepth - maxDepth),2); k > 0;k--) {
            printf("\t");
        }
    }
    printf("{");
    for (int i = start; i < end -1; i++) {
        printf("%d, ", arr[i]);
    }
    printf("%d}", arr[end - 1]);

}


void divideAndPrintTree(int arr[], int start, int end, int depth, int maxDepth, int totalDepth) {
    if (depth >= maxDepth) {
        printArrayInTree(arr, start, end, totalDepth, maxDepth); 
        return;
    }

    int mid = (start + end) / 2;
    divideAndPrintTree(arr, start, mid, depth + 1, maxDepth, totalDepth);
    divideAndPrintTree(arr, mid, end, depth + 1, maxDepth, totalDepth);
}

void printArrayInProcess(int arr[], int start, int end, int totalDepth, int maxDepth, Shared *shared) {
    printf("Proceso %d: ", shared->counter_process);
    printf("{");
    for (int i = start; i < end -1; i++) {
        printf("%d, ", arr[i]);
    }
    printf("%d}\n", arr[end - 1]);
    shared->counter_process++;

}


void divideAndPrintProcess(int arr[], int start, int end, int depth, int maxDepth, int totalDepth,Shared *shared) {
    if (depth >= maxDepth) {
        printArrayInProcess(arr, start, end, totalDepth, maxDepth, shared); 
        return;
    }

    int mid = (start + end) / 2;
    divideAndPrintProcess(arr, start, mid, depth + 1, maxDepth, totalDepth, shared);
    divideAndPrintProcess(arr, mid, end, depth + 1, maxDepth, totalDepth, shared);
}



void buildHeapFromInorder(int *inorden, int *heap, int start, int end, int index) {
    if (start > end) return;

    int mid = (start + end) / 2;  // Encontrar la posicion central
    heap[index] = inorden[mid];   // Establecer el elemento central como la raiz actual

    // Construir recursivamente el subarbol izquierdo y derecho
    buildHeapFromInorder(inorden, heap, start, mid - 1, 2 * index + 1);
    buildHeapFromInorder(inorden, heap, mid + 1, end, 2 * index + 2);
}



void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergesort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

    mergesort(arr, l, m);
    mergesort(arr, m + 1, r);

    merge(arr, l, m, r);
  }
}

void crear_arbol_binario(int nivel_actual, int nivel_maximo, int arr[], int n,Shared *shared, int heap[], int tamHeap) {
    int contador = shared->counter_process;

    if (nivel_actual > nivel_maximo)
        return; // Caso base: si el nivel actual supera el nivel maximo, retornar.

    pid_t hijo_izquierdo, hijo_derecho;

    if (nivel_actual == nivel_maximo) {
        mergesort(arr, 0, n - 1);

        for (int i = 0; i < tamHeap; i++) {
            if (contador == heap[i]){
                printf("Proceso %d: lista ordenada: ",i);
            }
        }

        //printf("Proceso: %d, lista ordenada: ",contador);
        printArray(arr, n);
        printf("\n");
        shared->counter_process++;
        exit(0);
    }

    int mid = n / 2;

    // Crear hijo izquierdo
    hijo_izquierdo = fork();
    if (hijo_izquierdo == 0) { // En el proceso del hijo izquierdo

        crear_arbol_binario(nivel_actual + 1, nivel_maximo, arr, mid, shared,heap,tamHeap); // Llamada recursiva

        printf(" => ");
        printArray(arr, mid);
        printf("\n");

        exit(0); // Salir para evitar que el hijo izquierdo cree mas procesos
    } else {
        wait(NULL); // Esperar a que el hijo izquierdo termine
    }
    
    contador = shared->counter_process++;
    
  // Crear hijo derecho
    hijo_derecho = fork();
    if (hijo_derecho == 0) { // En el proceso del hijo derecho
        crear_arbol_binario(nivel_actual + 1, nivel_maximo, arr + mid, n - mid, shared,heap,tamHeap); // Llamada recursiva

        printf(" => ");
        printArray(arr + mid, n - mid);
        printf("\n");

        exit(0); // Salir para evitar que el hijo derecho cree mas procesos
    } else {
        wait(NULL); // Esperar a que el hijo derecho termine
    }

    merge(arr, 0, mid - 1, n - 1);

    for (int i = 0; i < tamHeap; i++) {
        if (contador == heap[i]){
            printf("Proceso %d: lista izquierda ",i);
        }
    }

    //printf("proceso %d: lista izquierda ",contador);
    printArray(arr, mid);

    printf(" lista derecha ");
    printArray(arr + mid, n - mid);
}

int main(int argc, char *argv[]) {
    //shm_unlink(SHM_ARRAY); //en caso de error, descomentar esta linea

    if (argc != 3) {
        printf("Uso: %s <número de procesos> <lista de enteros separados por comas (ejemplo: ./proceso 7 5,4,8,9,3,1,4,7,8,9,5,4,8,7,9,6)>\n",
           argv[0]);
    return 1;
    }    

    int numProcesos = atoi(argv[1]);
    int totalDepth = 0;              
    int currentPowerOfTwo = 1;    // auxiliar

    //obtenemos la profundidad total del arbol binario
    while (currentPowerOfTwo <= numProcesos) {
        totalDepth++;   
        currentPowerOfTwo *= 2;
  }
    
    if (currentPowerOfTwo - 1 > numProcesos || numProcesos <= 0){
        printf("Ingrese la cantidad de procesos de tal manera que coincida con el número de nodos de un árbol binario completo. Por ejemplo, puedes seleccionar entre 1, 3, 7, 15, etc.");
        return 1;
  }
    //inicializamos el arreglo de enteros
    int n = (strlen(argv[2]) + 1) / 2; //longitud de la lista de enteros
    int arr[n];
    for (int i = 0; i < n; i++) { 
        arr[i] = argv[2][i * 2] - '0';
    }
    
    //abrir o crear el segmento de memoria compartida
    int fd_shm = shm_open(SHM_ARRAY, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR); 
    ftruncate(fd_shm, sizeof(Shared) + n * sizeof(int));

    Shared *shared = mmap(NULL, sizeof(Shared) + n * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd_shm, 0); //mapear el segmento de memoria compartida
    shared->size = n;//
    shared->counter_process = 0;

    int heap[numProcesos];    //monticulo que nos ayuda en la enumeracion de los procesos
    int inorden[numProcesos]; //nuestros procesos en recorrido inorden
    
    // Inicializar el array del montículo
    for (int i = 0; i < numProcesos; i++) {
        heap[i] = -1;      // Usa un valor sentinela para identificar nodos no asignados
        inorden[i] = i;    //llenamos el arbol con los procesos
    }

    // Construir el montículo a partir del recorrido en inorden
    buildHeapFromInorder(inorden, heap, 0, numProcesos - 1, 0);


    for (int i = 0; i < shared->size; i++) {
        shared->shared_array[i] = arr[i];
    }

    //esquema de arbol binario
    printf("===esquema de arbol===\n");
    int contador = 0;
    for (int j = 0; j < totalDepth; j++) {
        for (int i = 0; i < pow(2, j); i++) {
            for (int k = pow((totalDepth - j) + 1,2); k > 0;k--) {
                printf("\t");
                
            }
            printf("Proceso: %d ",contador);
            contador++;
        }
        printf("\n");
        divideAndPrintTree(shared->shared_array, 0, n, 0, j, totalDepth);
        printf("\n");
    }
    //mapeo de procesos
    printf("===mapeos===\n");
    
    for (int j = 0; j < totalDepth; j++) {
        divideAndPrintProcess(shared->shared_array, 0, n, 0, j, totalDepth,shared);
    }
    //esquema de procesamiento
    printf("===procesamiento===\n");
    shared->counter_process = 0;
    crear_arbol_binario(1, totalDepth, shared->shared_array, n, shared,heap,numProcesos);

    //concluye el proceso 0
    printf(" => ");
    printArray(shared->shared_array, n);
    printf("\n");
    shm_unlink(SHM_ARRAY);//elimina el segmento de memoria compartida
    return 0;
}
