/* Orden de ejecucion: programa0, programa2, programa1 
   Este programa envía de forma planificada señales a los procesos detallados en un archivo de texto
   Grupo 3:
   * Maria Luján Melgarejo Acosta       5111244 
   * Diego Nicolas Moreno Saldivar      5689969
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int main() {
    FILE *file;
    int procesoId, senhal, segundosDelay;

    // Abrimos el archivo que contiene los PIDs, las señales y los delays
    file = fopen("procesar_senhal.txt", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Leemos el archivo línea por línea
    while (fscanf(file, "%d %d %d", &procesoId, &senhal, &segundosDelay) ==3){
        // Dormimos por el delay especificado
        sleep(segundosDelay);

        // Enviamos la señal al proceso
        if (kill(procesoId, senhal) == -1) {
            perror("kill failed");
        } else {
            printf("Señal %d enviada al proceso %d después de %d segundos.\n", senhal, procesoId, segundosDelay);
        }
    }

    fclose(file);
    return 0;
}
