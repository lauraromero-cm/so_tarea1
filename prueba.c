#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>

#define MAX_JUGADORES 10

// Función aleatoria para la duración de la música
void esperar_musica() {
    int tiempo = rand() % 3 + 1;  // tiempo de espera aleatorio entre 1 y 3 segundos
    sleep(tiempo);
}

// Función para realizar el proceso de votación
void votar(int jugador_id, int* eliminados, int N) {
    int voto;
    while (voto == jugador_id || eliminados[voto] == 1){
        voto = rand() % N;
    }  // Evitar que un jugador se vote a sí mismo o vote a un eliminado
    printf("Jugador %d vota por eliminar al Jugador %d\n", jugador_id, voto);
    char pipe_name[20];
    sprintf(pipe_name, "pipe%d", jugador_id);
    int fd = open(pipe_name, O_WRONLY);
    write(fd, &voto, sizeof(voto));
    close(fd);
}

// Función principal del juego
int main() {
    int N;
    srand(time(NULL));
    pid_t padre = getpid();

    printf("Ingrese el número de jugadores (2-10): ");
    scanf("%d", &N);

    if (N < 2 || N > MAX_JUGADORES) {
        printf("Número de jugadores inválido. Debe estar entre 2 y 10.\n");
        return 1;
    }

    pid_t jugadores[MAX_JUGADORES];
    int eliminados[MAX_JUGADORES]; // Array para rastrear jugadores eliminados
    memset(eliminados, 0, sizeof(eliminados)); // Inicialmente, nadie está eliminado

    // Crear named pipes para cada jugador
    for (int i = 0; i < N; i++) {
        char pipe_name[20];
        sprintf(pipe_name, "pipe%d", i);
        mkfifo(pipe_name, 0666);
    }

    if(getpid() == padre){
        for(int i = 0; i < 2; i++){
            if(getpid() == padre){
                if (i == 0){
                    jugadores =fork();
                }else{
                    gestor 
                }
            }
        }
    }

    // Crear procesos jugadores
    for (int i = 0; i < N; i++) {
        if (padre == getpid()) {
            jugadores[i] = fork();
        }
        if (jugadores[i] == 0) {  // Código del hijo (jugador)
            while (1) {
                if (eliminados[i] == 1) {
                    exit(0);  // El jugador eliminado deja de votar
                }
                esperar_musica();
                votar(i, eliminados, N);  // Pasar array de eliminados a la función votar
            }
            exit(0);
        }
    }

    // Proceso observador
    while (N > 1) {
        int votos[MAX_JUGADORES] = {0}; // Inicializar array de votos a cero

        // Recibir votos de los jugadores
        for (int i = 0; i < N; i++) {
            if (eliminados[i] == 1) continue; // Saltar jugadores eliminados
            int voto;
            char pipe_name[20];
            sprintf(pipe_name, "pipe%d", i);
            int fd = open(pipe_name, O_RDONLY);
            read(fd, &voto, sizeof(voto));
            close(fd);
            if (eliminados[voto] == 0) {
                votos[voto]++;
            }
        }

        // Determinar el jugador más votado
        int max_votos = 0;
        int jugador_eliminado = -1;
        for (int i = 0; i < N; i++) {
            if (eliminados[i] == 1) continue; // Saltar jugadores eliminados
            if (votos[i] > max_votos) {
                max_votos = votos[i];
                jugador_eliminado = i;
            } else if (votos[i] == max_votos && max_votos > 0) {
                // En caso de empate, elegir aleatoriamente entre los empatados
                if (rand() % 2 == 0) {
                    jugador_eliminado = i;
                }
            }
        }

        // Eliminar al jugador más votado
        if (jugador_eliminado != -1) {
            printf("El Jugador %d ha sido eliminado.\n", jugador_eliminado);
            eliminados[jugador_eliminado] = 1; // Marcar al jugador como eliminado
            kill(jugadores[jugador_eliminado], SIGKILL);
            N--;
        }
    }

    // Determinar el jugador ganador
    for (int i = 0; i < MAX_JUGADORES; i++) {
        if (eliminados[i] == 0) { // El que no está eliminado es el ganador
            printf("¡El Jugador %d es el ganador!\n", i);
            kill(jugadores[i], SIGKILL);
            break;
        }
    }

    // Eliminar los named pipes
    for (int i = 0; i < MAX_JUGADORES; i++) {
        char pipe_name[20];
        sprintf(pipe_name, "pipe%d", i);
        unlink(pipe_name);
    }

    return 0;
}


