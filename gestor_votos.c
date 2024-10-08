#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "lib/utils.h"
#include <time.h>

#define MAX_JUGADORES 100
#define PIPE_PATH "pipes"

//PROCESO OBSEVADOR (GESTOR DE VOTOS)


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

    int fd[2][MAX_JUGADORES], votos[MAX_JUGADORES] = {0};
    int Jugadores[MAX_JUGADORES] = {0};
    char PIPE_PATH_USER[100];


    ZeroArray(Jugadores, jugadores);

    sprintf(PIPE_PATH_USER, "%s/j-g", PIPE_PATH);
    ConnectMassivePipes(PIPE_PATH_USER,fd[0],jugadores,O_RDONLY);

    sprintf(PIPE_PATH_USER, "%s/g-j", PIPE_PATH);
    ConnectMassivePipes(PIPE_PATH_USER,fd[1],jugadores,O_WRONLY);
  

    int voto, i, jugador_eliminado = -1, mayor_votos = 0;

    int ronda = 0;
    srand(1016818390%getpid() );

    for (ronda = 0; CantidadJugadoresVivo(Jugadores, jugadores) > 1; ronda++)
    {
        printf("\n");
        printf("RONDA N°%d Jugadores Vivos: %d \n", ronda+1, CantidadJugadoresVivo(Jugadores, jugadores));
        printf("\n");
        ZeroArray(votos, MAX_JUGADORES);
        int tiempo_musica = aleatorio(2)+1;

        dance(tiempo_musica,fd[1],Jugadores,jugadores);


        // Leer los votos desde la tubería
        for (i = 0; i < jugadores; i++)
        {
            if (Jugadores[i] == 1)
            {
                continue;
            }

            if (read(fd[0][i], &voto, sizeof(voto)) > 0)
            {

                if (voto >= 0 && voto <= jugadores) // Verificar que el voto sea válido
                {
                    printf("VOTO RECIBIDO %d : %d \n",i,voto);
                    votos[voto]++; // Incrementar el conteo de votos para el jugador
                }
                else
                {
                    printf("Voto inválido recibido: %d\n", voto);
                }
            } else {
                printf("No vi llegar el voto del :  %d\n", i);
            }
            
        }

        // Determinar el jugador con mayor cantidad de votos

        int jugador_eliminado = JugadorMax(votos,jugadores,-1);

        // printf("EL JUGAR CON BAN ES  : %d\n",jugador_eliminado);
        int jugador_eliminado2 = JugadorMax(votos,jugadores,jugador_eliminado);

      

        if (jugador_eliminado== jugador_eliminado2){
            jugador_eliminado=-1;
            sendMassive((fd[1]),-1,Jugadores,jugadores);
            continue;
        }

       if (jugador_eliminado == -1){
            printf("No se recibieron votos válidos.\n");
            continue;
        }

        
        printf("EL JUGADOR ELIMINADO ES %d \n",jugador_eliminado);
     
        // Mostrar el resultado
        sendMassive((fd[1]),jugador_eliminado,Jugadores,jugadores);
        Jugadores[jugador_eliminado] = 1;
        
    

        close(fd[0][jugador_eliminado]);
        close(fd[1][jugador_eliminado]);
        sleep(1);
        printf("El Jugador %d ha sido eliminado con %d votos.\n", jugador_eliminado, votos[jugador_eliminado]);
        printf("Siguiente Ronda : Jugadores vivos :%d \n ", CantidadJugadoresVivo(Jugadores, jugadores));

    }
    printf("\n");
    printf("EL GANADOR DEL JUEGO ES!!!!!: %d \n",JugadorGanador(Jugadores, jugadores));
    printf("\n");


    return 0;
}