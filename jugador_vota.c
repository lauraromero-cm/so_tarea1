#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>

#define PIPE_PATH "pipes"
#define MAX_JUGADORES 100


int generar_numero_aleatorio(int *array,int maxJugadores) {
    int voto;
    do {
        voto = (rand() % maxJugadores) + 1;
    } while(array[voto]!=0);


    return  voto;
}


void ZeroArray(int *array, int n)
{
    for (int ix = 0; ix < n; ix++)
    {
        array[ix] = 0;
    }
}

int CantidadJugadoresVivo(int *array, int n)
{

    int cantidad = n;
    for (int ix = 0; ix < n; ix++)
    {
        cantidad -= array[ix];
    }
    return cantidad;
}


int main(int argc, char *argv[]) {
    int fd[2], voto,jugador;

    // Verificar que se haya pasado el número de jugador a votar
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <jugador_a_eliminar>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    jugador = atoi(argv[1]); // Convertir el argumento en un número entero   
    jugador--;

   int  maxJugadores = atoi(argv[2]);


    int votos[MAX_JUGADORES] = {0};
    int Jugadores[MAX_JUGADORES] = {0};

    printf("El jugador %d ha votado.\n", jugador);
    // Abrir la tubería en modo de solo escritura
    char PIPE_PATH_USER[100];
    sprintf(PIPE_PATH_USER,"%s/j-g_%d",PIPE_PATH,jugador);
    fd[0] = open(PIPE_PATH_USER, O_WRONLY);
    if (fd[0] == -1) {
        perror("Error al abrir la tubería para escribir");
       //exit(EXIT_FAILURE);
    }

    sprintf(PIPE_PATH_USER,"%s/g-j_%d",PIPE_PATH,jugador);
    fd[1] = open(PIPE_PATH_USER, O_RDONLY);
    if (fd[1] == -1) {
        perror("Error al abrir la tubería para escribir");
       //exit(EXIT_FAILURE);
    }

    // Enviar el voto a través de la tubería
    unsigned int seed = time(NULL) ^ (getpid() << 16) ^ (uintptr_t)&seed;
    srand(seed);


    ZeroArray(Jugadores, maxJugadores);

    while(1==1){
            voto = generar_numero_aleatorio(Jugadores,maxJugadores);

            if (write(fd[0], &voto, sizeof(voto)) < 0) {
                perror("Error al enviar el voto");
                exit(EXIT_FAILURE);
            }   

             if (read(fd[1], &voto, sizeof(voto)) > 0)
            {
                printf("EL MUERTO FUE :%d\n",voto);
                if (voto-1==jugador){
                    break;
                }
                Jugadores[voto-1]=1;

            }



    }







    printf("Me Mori po :%d\n",jugador);


    close(fd[0]); // Cerrar la tubería
    close(fd[1]); 
    return 0;
}
