#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define PIPE_PATH "./votos_pipe"

int main(int argc, char *argv[]) {
    int fd, voto;

    // Verificar que se haya pasado el número de jugador a votar
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <jugador_a_eliminar>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    voto = atoi(argv[1]); // Convertir el argumento en un número entero
       
    // Verificar que el voto sea válido y que el jugador no esté votando por sí mismo
    // Nota: Se asume que el número del jugador se pasa como argumento y no como el PID
    // Esto debe ser ajustado según la lógica del juego
    int jugador_num = getpid(); // Este es un ejemplo, debes usar la lógica correcta para obtener el número del jugador

    if (voto < 1 || voto == jugador_num) {
        fprintf(stderr, "Número de jugador inválido o el jugador no puede votar por sí mismo.\n");
        exit(EXIT_FAILURE);
    }

    printf("El jugador %d ha votado.\n", voto);

    // Abrir la tubería en modo de solo escritura
    
    fd = open(PIPE_PATH, O_WRONLY);
    if (fd == -1) {
        perror("Error al abrir la tubería para escribir");
       //exit(EXIT_FAILURE);
    }

    // Enviar el voto a través de la tubería
    if (write(fd, &voto, sizeof(voto)) < 0) {
        perror("Error al enviar el voto");
        exit(EXIT_FAILURE);
    }

   // wait(NULL); // Esperar a que el gestor termine

    close(fd); // Cerrar la tubería

    return 0;
}
