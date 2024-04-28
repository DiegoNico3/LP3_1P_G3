/* Orden de ejecucion: programa0, programa2, programa1 
   Este programa está diseñado para crear procesos hijos utilizando 
   la llamada al sistema fork() y luego genera el archivo donde coloca el pid, señal y delay de los 3 procesos
   Grupo 3:
   * Maria Luján Melgarejo Acosta       5111244 
   * Diego Nicolas Moreno Saldivar      5689969
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() {
    FILE *file = fopen("procesar_senhal.txt", "w");
    if (file == NULL) {
        perror("No se pudo abrir el archivo");
        return 1;
    }

    pid_t pid;
    int senhales[3] = {2, 14, 15}; // Señales para manejar
    int delays[3] = {2, 5, 7}; // Tiempos de retraso

    for (int i = 0; i < 3; ++i) {
        pid = fork();

        if (pid < 0) {
            perror("Error: fallo el fork");
            return 1;
        }

        if (pid == 0) { // Proceso hijo
            // Cada proceso hijo debe ejecutar el programa2 aquí.
            execl("./programa2", "programa2", NULL);
            perror("Error: fallo el execl");
            exit(1); // Si execl falla, termina el proceso hijo.
        } else {
            // El proceso padre escribe en el archivo.
            fprintf(file, "%d %d %d\n", pid, senhales[i], delays[i]);
        }
    }

    fclose(file);
    return 0; // El proceso padre termina después de crear los procesos hijo.
}
