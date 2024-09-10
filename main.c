#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "lib/dir.h"

int main() {
    int jugadores;
    pid_t PID_Padre = getpid();  // obtener el PID del proceso principal
    printf("Ingrese el número de jugadores: ");
    scanf("%d", &jugadores);
    printf("Soy el padre de todos, con el pid %d\n", jugadores);
    FolderPipes(jugadores);  // crear las pipes para la cantidad de jugadores 
    pid_t PID_Gestor = fork();  // crear un proceso hijo que actuará como gestor (el observador)
    // En la rubrica dice que el observador NO tiene relacion padre-hijo con los N jugadores (Asi que cumple con la rubrica)
    if (PID_Gestor == 0) {
        printf("Soy el gestor con el pid %d y mi padre tiene el pid %d\n", getpid(), getppid());
        char jugador[100];
        sprintf(jugador, "%d", jugadores);
        execl("./gestor_votos", jugador, NULL);  // ejecutar el programa del gestor
        perror("Error al ejecutar gestor_votos");  
        return 1;
    }
    //El observador es simplemente otro proceso hijo del proceso principal, al igual que los jugadores (no hay relacion directa entre el observador y los jugadores)
    printf("Soy lanza los jugadores %d y mi padre tiene el pid %d\n", getpid(), getppid());
    // crear procesos hijos para cada jugador
    for (int ix = 0; ix < jugadores; ix++) {
        pid_t jugador_pid = fork();
        if (jugador_pid == 0) {
            printf("Soy el jugador %d con el pid %d y mi padre tiene el pid %d\n", ix , getpid(), getppid());
            // ejecutar el programa de cada jugador
            char jugadores_str[100];
            char jugador_str[100];
            sprintf(jugadores_str, "%d", jugadores);
            sprintf(jugador_str, "%d", ix );  // numero del jugador
            execl("./jugador_vota", "jugador_vota", jugador_str, jugadores_str, NULL);
            perror("Error al ejecutar jugador_vota");  
        }
    }
    // esperar a que todos los procesos hijos terminen
    for (int ix = 0; ix < jugadores; ix++) {
        wait(NULL);
    }
    Clear();  //limpiar las pipes después de que todos los jugadores terminen
    return 0;
}
