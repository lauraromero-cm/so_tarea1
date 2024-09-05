#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAX_JUGADORES 100
#define PIPE_PATH "pipes"

// PROCESO OBSERVADOR (GESTOR DE VOTOS)

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

int JugadorGanador(int *array, int n)
{
    for (int ix = 0; ix < n; ix++)
    {
        if (array[ix]==0)
        return ix;
    }
    return n;
}


int main(int argc, char *argv[])
{

    if (argc != 1)
    {
        printf("Error en la cantidad de argumentos \n");
        exit(1);
    }

    int jugadores = atoi(argv[0]);

    if (jugadores >= MAX_JUGADORES)
    {
        printf("Supera la cantidad de jugadores maximos \n");
        exit(1);
    }

    int fd[MAX_JUGADORES][2], votos[MAX_JUGADORES] = {0};
    int Jugadores[MAX_JUGADORES] = {0};
    char PIPE_PATH_USER[100];

    ZeroArray(Jugadores, jugadores);
    int voto, i, jugador_eliminado = -1, mayor_votos = 0;

    int ronda = 0;

    for (i = 0; i < jugadores; i++)
        {

               sprintf(PIPE_PATH_USER, "%s/j-g_%d", PIPE_PATH, i);
               fd[i][0] = open(PIPE_PATH_USER, O_RDONLY);
               sprintf(PIPE_PATH_USER, "%s/g-j_%d", PIPE_PATH, i);
               fd[i][1] = open(PIPE_PATH_USER, O_WRONLY);
               
        }



    for (ronda = 0; CantidadJugadoresVivo(Jugadores, jugadores) > 1; ronda++)
    {
        printf("RONDA N°%d Jugadores Vivos: %d \n", ronda+1, CantidadJugadoresVivo(Jugadores, jugadores));
        ZeroArray(votos, MAX_JUGADORES);


        // Leer los votos desde la tubería
        for (i = 0; i < jugadores; i++)
        {
            if (Jugadores[i] == 1)
            {
                continue;
            }
      

            if (read(fd[i][0], &voto, sizeof(voto)) > 0)
            {
                if (voto >= 1 && voto <= jugadores)
                {
                    votos[voto - 1]++; // Incrementar el conteo de votos para el jugador
                }
                else
                {
                    printf("Voto inválido recibido: %d\n", voto);
                }
            }
            
        }

        // Determinar el jugador con mayor cantidad de votos
        for (i = 0; i < jugadores; i++)
        {
            if (votos[i] > mayor_votos)
            {
                mayor_votos = votos[i];
                jugador_eliminado = i + 1; // Ajustar índice para representar al jugador real
            }
    
        }
        // Mostrar el resultado
        if (jugador_eliminado != -1)
        {
            Jugadores[jugador_eliminado-1] = 1;
            
            for (int i=0;i<jugadores;i++){
                if ( Jugadores[i]==1){
                    continue;
                }

                if (write(fd[i][1], &jugador_eliminado, sizeof(jugador_eliminado)) < 0) {
                    perror("Error al enviar el voto");
                    exit(EXIT_FAILURE);
                }

            }
            close(fd[jugador_eliminado-1][0]);
            close(fd[jugador_eliminado-1][1]);
            printf("El Jugador %d ha sido eliminado con %d votos.\n", jugador_eliminado, mayor_votos);
            //execl("./amurra_y_reclama", "amurra_y_reclama", NULL);
        }
        else
        {
            printf("No se recibieron votos válidos.\n");
        }

        printf("Siguiente Ronda : Jugadores vivos :%d \n ", CantidadJugadoresVivo(Jugadores, jugadores));

    }

    printf("EL GANADOR ES %d",JugadorGanador(Jugadores, jugadores));

    return 0;
}