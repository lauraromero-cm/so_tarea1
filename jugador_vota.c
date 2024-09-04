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

int generar_numero_aleatorio(int maxJugadores) {

    return  (rand() % maxJugadores) + 1;
}


int main(int argc, char *argv[]) {
    int fd, voto,jugador;

    // Verificar que se haya pasado el número de jugador a votar
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <jugador_a_eliminar>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    jugador = atoi(argv[1]); // Convertir el argumento en un número entero   
    jugador--;

   int  maxJugadores = atoi(argv[2]);
    printf("El jugador %d ha votado.\n", jugador);
    // Abrir la tubería en modo de solo escritura
    char PIPE_PATH_USER[100];
    sprintf(PIPE_PATH_USER,"%s/%d",PIPE_PATH,jugador);

    fd = open(PIPE_PATH_USER, O_WRONLY);
    if (fd == -1) {
        perror("Error al abrir la tubería para escribir");
       //exit(EXIT_FAILURE);
    }

    // Enviar el voto a través de la tubería


    unsigned int seed = time(NULL) ^ (getpid() << 16) ^ (uintptr_t)&seed;
    srand(seed);



    voto = generar_numero_aleatorio(maxJugadores);

    if (write(fd, &voto, sizeof(voto)) < 0) {
        perror("Error al enviar el voto");
        exit(EXIT_FAILURE);
    }


    close(fd); // Cerrar la tubería

    return 0;
}
