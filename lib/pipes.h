#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>


void CreatePipe(char *PIPE_PATH_USER,int type ){
    mkfifo(PIPE_PATH_USER, type);  //crear la pipe principal para el gestor
    return ;
}


void ConnectPipe(int *fd,char *PIPE_PATH_USER,int type ) {

    *fd = open(PIPE_PATH_USER, type);
    if (*fd == -1) {
        perror("Error al abrir la tubería para escribir");
       //exit(EXIT_FAILURE);
    }

    return ;
}

int ClosePipe(){
    //close();
    return 0 ;
}

void send(int *fd, int msg){
    if (write(*fd, &msg, sizeof(msg)) < 0) {
        perror("Error al enviar el mensaje");
        exit(EXIT_FAILURE);
    }
}

void sendMassive(int *fd, int msg, int *jugadores_array, int length){
    for (int ix = 0; ix < length; ix++){
        if (jugadores_array[ix] == 1){
            continue;
        }
        send(&fd[ix], msg);
    }
}

void ConnectMassivePipes(char *path,int *fd, int length,int type){
    char PIPE_PATH_USER[100];
    for (int ix = 0; ix < length; ix++)
        {
            sprintf(PIPE_PATH_USER, "%s_%d", path, ix);            
            ConnectPipe(&fd[ix],PIPE_PATH_USER, type);
        }
}