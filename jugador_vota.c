#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include "lib/utils.h"

#define PIPE_PATH "pipes"
#define MAX_JUGADORES 100

int main(int argc, char *argv[])
{
    int fd[2], voto, jugador;

    // Verificar que se haya pasado el número de jugador a votar
    if (argc != 3)
    {
        fprintf(stderr, "Uso: %s <jugador_a_eliminar>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    jugador = atoi(argv[1]); // Convertir el argumento en un número entero

    int maxJugadores = atoi(argv[2]);

    int votos[MAX_JUGADORES] = {0};
    int Jugadores[MAX_JUGADORES] = {0};

    // Abrir la tubería en modo de solo escritura
    char PIPE_PATH_USER[100];

    sprintf(PIPE_PATH_USER, "%s/j-g_%d", PIPE_PATH, jugador);
    ConnectPipe(&fd[0], PIPE_PATH_USER, O_WRONLY);

    sprintf(PIPE_PATH_USER, "%s/g-j_%d", PIPE_PATH, jugador);
    ConnectPipe(&fd[1], PIPE_PATH_USER, O_RDONLY);

    srand(1016818390 % getpid());
    ZeroArray(Jugadores, maxJugadores);

    char process[100];
    sprintf(process, "musica");
    int musica;
    voto = -1;
    while (voto != jugador)
    {
        do
        {
            if (read(fd[1], &musica, sizeof(musica)) > 0)
            {
                sleep(1);
                voto = generar_numero_aleatorio(Jugadores, maxJugadores);
            }

        } while (musica==-4);

        send(&fd[0], voto);

        if (read(fd[1], &voto, sizeof(voto)) > 0)
        {
            Jugadores[voto] = 1;
        }
    }

    pid_t PID_Gestor = fork(); // crear un proceso hijo que actuará como gestor (el observador)
    if (PID_Gestor != 0)
    {

        execl("./amurra_y_reclama", "amurra_y_reclama", NULL);
        return 0;
    }

    close(fd[0]); // Cerrar la tubería
    close(fd[1]);

    return 0;
}
