#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define PIPE_PATH "pipes"  // definicion del nombre del directorio que contendra las pipes

int remove_directory(const char *path) {
    DIR *dir = opendir(path);  // abre el directorio especificado
    struct dirent *entry;
    char filepath[1024];

    if (dir == NULL) {  // verifica si el directorio se pudo abrir correctamente
        perror("Error al abrir la carpeta");
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        snprintf(filepath, sizeof(filepath), "%s/%s", path, entry->d_name);  // construir la ruta completa

        struct stat st;
        if (stat(filepath, &st) == 0 && S_ISDIR(st.st_mode)) {
            // si es un directorio, se elimina recursivamente
            remove_directory(filepath);
        } else {
            // si es un archivo, se elimina directamente
            if (remove(filepath) != 0) {
                perror("Error al eliminar archivo");
            }
        }
    }

    closedir(dir);

    // finalmente se elimina el directorio
    if (rmdir(path) != 0) {
        perror("Error al eliminar la carpeta");
        return -1;
    }

    return 0;
}

void Clear() {
    if (remove_directory(PIPE_PATH) == 0) {
        printf("Carpeta '%s' eliminada exitosamente.\n", PIPE_PATH);
    } else {
        printf("Error al eliminar la carpeta '%s'.\n", PIPE_PATH);
    }

    // crear nuevamente el directorio después de eliminarlo
    if (mkdir(PIPE_PATH, 0755) == 0) {
        printf("Carpeta creada exitosamente.\n");
    } else {
        perror("Error al crear la carpeta");
    }
}

void FolderPipes(int N) {
    Clear();  // limpiar el directorio de pipes antes de crear nuevas pipes

    char PIPE_PATH_USER[100];
    for (int i = 0; i < N; i++) {
        sprintf(PIPE_PATH_USER, "%s/%d", PIPE_PATH, i);  // construir la ruta para cada pipe
        if (access(PIPE_PATH_USER, F_OK) == 0) {
            // si la pipe ya existe, se elimina
            if (unlink(PIPE_PATH) != 0) {
                perror("Error al eliminar el archivo FIFO");
                exit(EXIT_FAILURE);
            } else {
                printf("FIFO existente eliminada.\n");
            }
        }
        mkfifo(PIPE_PATH_USER, 0666);  // crear una nueva named pipe
    }
}

int main() {
    int N;
    pid_t padre = getpid();  // obtener el PID del proceso principal

    printf("Ingrese el número de jugadores: ");
    scanf("%d", &N);

    printf("Soy el padre de todos, con el pid %d\n", padre);

    FolderPipes(N);  // crear las pipes para la cantidad de jugadores 

    pid_t gestor = fork();  // crear un proceso hijo que actuará como gestor (el observador)

    mkfifo(PIPE_PATH, 0666);  //crear la pipe principal para el gestor
    if (gestor == 0) {
        printf("Soy el gestor con el pid %d y mi padre tiene el pid %d\n", getpid(), getppid());
        char USERS[100];
        sprintf(USERS, "%d", N);
        execl("./gestor_votos", USERS, NULL);  // ejecutar el programa del gestor
        perror("Error al ejecutar gestor_votos");  
        return 1;
    }

    printf("Soy lanza los jugadores %d y mi padre tiene el pid %d\n", getpid(), getppid());

    // crear procesos hijos para cada jugador
    for (int i = 0; i < N; i++) {
        pid_t jugador_pid = fork();
        if (jugador_pid == 0) {
            printf("Soy el jugador %d con el pid %d y mi padre tiene el pid %d\n", i + 1, getpid(), getppid());

            // ejecutar el programa de cada jugador
            char jugador_num_str[10];
            char USERS[100];
            sprintf(USERS, "%d", N);
            sprintf(jugador_num_str, "%d", i + 1);  // numero del jugador
            execl("./jugador_vota", "jugador_vota", jugador_num_str, USERS, NULL);
            perror("Error al ejecutar jugador_vota");  
        }
    }

    // esperar a que todos los procesos hijos terminen
    for (int i = 0; i < N; i++) {
        wait(NULL);
    }

    Clear();  //limpiar las pipes después de que todos los jugadores terminen

    return 0;
}
