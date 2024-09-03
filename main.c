#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define PIPE_PATH "./votos_pipe"

// void esperar_musica() {
//     int tiempo = rand() % 10 + 1; // Tiempo aleatorio entre 1 y 10 segundos
//     sleep(tiempo);
// }

// void eliminar_jugador(int jugador) {
//     char comando[100];
//     sprintf(comando, "./amurra_y_reclama %d", jugador);
//     execl("/bin/sh", "sh", "-c", comando, NULL);
//     perror("Error al ejecutar amurra_y_reclama");
//     exit(1);
// }


int main() {

//srand(time(NULL));

  if (access(PIPE_PATH, F_OK) == 0) {
        // El archivo existe, así que lo eliminamos
        if (unlink(PIPE_PATH) != 0) {
            perror("Error al eliminar el archivo FIFO");
            exit(EXIT_FAILURE);
        } else {
            printf("FIFO existente eliminada.\n");
        }
    }
  
    int N;
    pid_t padre = getpid();

    printf("Ingrese el número de jugadores: ");
    scanf("%d", &N);

    printf("Soy el padre de todos, con el pid %d\n", padre);

    
    // Crear la tubería con nombre antes de crear el gestor y jugadores
    mkfifo(PIPE_PATH, 0666);

    pid_t gestor = fork();

    if (gestor == 0) {
        printf("Soy el gestor con el pid %d y mi padre tiene el pid %d\n", getpid(), getppid());
        execl("./gestor_votos", "", NULL);
        perror("Error al ejecutar gestor_votos"); // Si execl falla
        // exit(1);
        return 1;
    }

        printf("Soy lanza los jugadores %d y mi padre tiene el pid %d\n", getpid(), getppid());

        // Crear jugadores
        for (int i = 0; i < N; i++) {
            pid_t jugador_pid = fork();
            if (jugador_pid == 0) {
                printf("Soy el jugador %d con el pid %d y mi padre tiene el pid %d\n", i + 1, getpid(), getppid());
                
                // Ejecutar jugador_vota con el número del jugador
                char jugador_num_str[10];
                sprintf(jugador_num_str, "%d", i + 1); // +1 porque los jugadores empiezan desde 1
                execl("./jugador_vota", "jugador_vota", jugador_num_str, NULL);
                perror("Error al ejecutar jugador_vota"); // Si execl falla
                // exit(1);
            }
        }
        // Esperar a que terminen todos los procesos hijos
        for (int i = 0; i < N; i++) {
            wait(NULL);
            //printf("ME MORIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII");
        }

        // Esperar a que termine el gestor
        //wait(NULL);

        // Eliminar la tubería después de que todos los procesos hayan terminado
        unlink(PIPE_PATH);
    


    return 0;
}




