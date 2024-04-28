/* Orden de ejecucion: programa0, programa2, programa1 
   Proceso que implemente un handler que notifique cuando reciba las principales señales enviadas por el programa 1
   Grupo 3:
   * Maria Luján Melgarejo Acosta       5111244 
   * Diego Nicolas Moreno Saldivar      5689969
*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int senhal) {
    const char* nombre_senhal;
    switch (senhal) {
        case SIGINT:
            nombre_senhal = "SIGINT";
            break;
        case SIGALRM:
            nombre_senhal = "SIGALRM";
            break;
        case SIGUSR1:
            nombre_senhal = "SIGUSR1";
            break;
        case SIGUSR2:
            nombre_senhal = "SIGUSR2";
            break;
        case SIGTERM:
            nombre_senhal = "SIGTERM";
            break;
        default:
            nombre_senhal = "Señal desconocida";
    }
    printf("Recibido %s\n", nombre_senhal);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handler; // Asigna la función de manejador de señales
    sigemptyset(&sa.sa_mask);       // Inicializa el conjunto de señales bloqueadas durante la ejecución del manejador
    sa.sa_flags = 0;                

    // Registra el manejador para varias señales comunes
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("No se puede manejar SIGINT"); // Manejar el error adecuadamente
        return EXIT_FAILURE;
    }
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("No se puede manejar SIGALRM");
        return EXIT_FAILURE;
    }
    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("No se puede manejar SIGUSR1");
        return EXIT_FAILURE;
    }
    if (sigaction(SIGUSR2, &sa, NULL) == -1) {
        perror("No se puede manejar SIGUSR2");
        return EXIT_FAILURE;
    }
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("No se puede manejar SIGTERM");
        return EXIT_FAILURE;
    }

    /* Imprime el PID para información, aunque no lo escriba en el archivo
    printf("Programa 2 running with PID: %d\n", getpid());*/

    // Bucle infinito para esperar señales
    while (1) {
        pause(); // El proceso espera aquí hasta que recibe una señal
    }

    return 0; // Este punto nunca debe alcanzarse
}
