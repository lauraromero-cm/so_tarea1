#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define PIPE_PATH "pipes"

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



int remove_directory(const char *path) {
    DIR *dir = opendir(path);
    struct dirent *entry;
    char filepath[1024];

    if (dir == NULL) {
        perror("Error al abrir la carpeta");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Ignorar las entradas "." y ".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(filepath, &st) == 0 && S_ISDIR(st.st_mode)) {
            // Si es una carpeta, eliminarla recursivamente
            remove_directory(filepath);
        } else {
            // Si es un archivo, eliminarlo
            // printf("%s ",filepath);
            if (remove(filepath) != 0) {
                perror("Error al eliminar archivo");
            }
        }
    }

    closedir(dir);

    // Finalmente eliminar la carpeta
    if (rmdir(path) != 0) {
        perror("Error al eliminar la carpeta");
        return -1;
    }

    return 0;
}

void Clear(){
if (remove_directory(PIPE_PATH) == 0) {
        printf("Carpeta '%s' eliminada exitosamente.\n", PIPE_PATH);
    } else {
        printf("Error al eliminar la carpeta '%s'.\n", PIPE_PATH);
    }
   if (mkdir(PIPE_PATH, 0755) == 0) {
        printf("Carpeta creada exitosamente.\n");
    } else {
        perror("Error al crear la carpeta");
    }

}

void FolderPipes(int N){

    Clear();
   
    // Crear la tubería con nombre antes de crear el gestor y jugadores
    // mkfifo(PIPE_PATH, 0666);

    char PIPE_PATH_USER[100];
    for (int i = 0; i < N; i++) {


        sprintf(PIPE_PATH_USER,"%s/%d",PIPE_PATH,i);
        // printf("%s\n",PIPE_PATH_USER);
        if (access(PIPE_PATH_USER, F_OK) == 0) {
        // El archivo existe, así que lo eliminamos
            if (unlink(PIPE_PATH) != 0) {
                perror("Error al eliminar el archivo FIFO");
                exit(EXIT_FAILURE);
            } else {
                printf("FIFO exis  tente eliminada.\n");
            }
        }
        mkfifo(PIPE_PATH_USER, 0666);
    }




}


int main() {

//srand(time(NULL));

 int N;
    pid_t padre = getpid();

    printf("Ingrese el número de jugadores: ");
    scanf("%d", &N);

    printf("Soy el padre de todos, con el pid %d\n", padre);


FolderPipes(N);
    

    pid_t gestor = fork();

    mkfifo(PIPE_PATH, 0666);
    if (gestor == 0) {
        printf("Soy el gestor con el pid %d y mi padre tiene el pid %d\n", getpid(), getppid());
        char USERS[100];
        sprintf(USERS,"%d",N);
        execl("./gestor_votos", USERS, NULL);
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
                char USERS[100];
                sprintf(USERS,"%d",N);
                sprintf(jugador_num_str, "%d", i + 1); // +1 porque los jugadores empiezan desde 1
                execl("./jugador_vota", "jugador_vota", jugador_num_str,USERS, NULL);
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

    


    Clear();

    return 0;
}




