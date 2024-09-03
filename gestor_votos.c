#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define MAX_JUGADORES 100
#define PIPE_PATH "./votos_pipe"

int main() {
    int fd, votos[MAX_JUGADORES] = {0}; 
    for (int ix=0;ix < MAX_JUGADORES; ix++){
        votos[ix]=0;
    }

    int voto, i, jugador_eliminado = -1, mayor_votos = 0;

    // Crear la tubería con nombre
    // mkfifo(PIPE_PATH, 0666);

    // Abrir la tubería en modo de solo lectura
    fd = open(PIPE_PATH, O_RDONLY);

    // Leer los votos desde la tubería
    for (i = 0; i < MAX_JUGADORES; i++) {
        if (read(fd, &voto, sizeof(voto)) > 0) {
            if (voto >= 1 && voto <= MAX_JUGADORES) {
                votos[voto - 1]++; // Incrementar el conteo de votos para el jugador
            } else {
                printf("Voto inválido recibido: %d\n", voto);
            }
        }
    }

    // Determinar el jugador con mayor cantidad de votos
    for (i = 0; i < MAX_JUGADORES; i++) {
        if (votos[i] > mayor_votos) {
            mayor_votos = votos[i];
            jugador_eliminado = i + 1; // Ajustar índice para representar al jugador real
        }
    }

    // Mostrar el resultado
    if (jugador_eliminado != -1) {
        printf("El Jugador %d ha sido eliminado con %d votos.\n", jugador_eliminado, mayor_votos);
        execl("./amurre", "amurre", NULL);
    } else {
        printf("No se recibieron votos válidos.\n");
    }

    wait(NULL); // Esperar a que todos los procesos hijos terminen
    close(fd); // Cerrar la tubería
    unlink(PIPE_PATH); // Eliminar la tubería con nombre

    return 0;
}