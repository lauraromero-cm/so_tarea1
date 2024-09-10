#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include "lib/pipes.h"
#include "lib/utils.h"

#define PIPE_PATH "pipes"
#define MAX_JUGADORES 100

int main(int argc, char *argv[]) {
    int fd[2], voto,jugador;

    // Verificar que se haya pasado el número de jugador a votar
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <jugador_a_eliminar>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    jugador = atoi(argv[1]); // Convertir el argumento en un número entero   

    int  maxJugadores = atoi(argv[2]);


    int votos[MAX_JUGADORES] = {0};
    int Jugadores[MAX_JUGADORES] = {0};

    printf("El jugador %d ha votado.\n", jugador);
    // Abrir la tubería en modo de solo escritura
    char PIPE_PATH_USER[100];


    sprintf(PIPE_PATH_USER,"%s/j-g_%d",PIPE_PATH,jugador);
    ConnectPipe(&fd[0],PIPE_PATH_USER,O_WRONLY);

    sprintf(PIPE_PATH_USER,"%s/g-j_%d",PIPE_PATH,jugador);
    ConnectPipe(&fd[1],PIPE_PATH_USER,O_RDONLY);

    // Enviar el voto a través de la tubería
    unsigned int seed = time(NULL) ^ (getpid() << 16) ^ (uintptr_t)&seed;
    srand(seed);

    //  srand(1016818390);
    ZeroArray(Jugadores, maxJugadores);

    while(1==1){ 
            voto = generar_numero_aleatorio(Jugadores,maxJugadores);

            send(&fd[0],voto);

             if (read(fd[1], &voto, sizeof(voto)) > 0)
            {
                printf("EL ELIMINADO FUE: %d \n",voto);
                if (voto==jugador){
                    break;
                }
                if (voto ==-1){
                    continue;
                }

                Jugadores[voto]=1;

            }

    }


    close(fd[0]); // Cerrar la tubería
    close(fd[1]); 

    
    return 0;
}
