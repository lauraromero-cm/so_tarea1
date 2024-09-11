#include "pipes.h"
int aleatorio(int num){

    return  (rand() % num) ;

}

int generar_numero_aleatorio(int *array,int maxJugadores) {
    int voto;
    do {
        voto =aleatorio(maxJugadores) ;
    } while(array[voto]!=0);


    return  voto;
}

void ZeroArray(int *array, int n)
{
    for (int ix = 0; ix < n; ix++)
    {
        array[ix] = 0;
    }
}

int CantidadJugadoresVivo(int *array, int n)
{

    int cantidad = n;
    for (int ix = 0; ix < n; ix++)
    {
        cantidad -= array[ix];
    }
    return cantidad;
}

int JugadorGanador(int *array, int n)
{
    for (int ix = 0; ix < n; ix++)
    {
        if (array[ix]==0)
        return ix;
    }
    return n;
}

int JugadorMax(int *array, int length, int pos){
    int max_votes = -1;   
    int jugador_max = -1; 

    for (int ix = 0; ix < length; ix++) {
        if (pos == ix) {
            continue;  
        }

        if (array[ix] > max_votes) {  
            max_votes = array[ix];    
            jugador_max = ix;         
        }
    }

    return jugador_max;  
}


void dance(int ciclos_musica,int *fd, int *jugadores_array, int length)
{


    printf("🎵 Empieza la música tiempo %d...\n",ciclos_musica);
    for (int i = 0; i < ciclos_musica; i++)
    {
        sendMassive(fd,-4,jugadores_array,length);

        // Simulación de movimientos de baile
        printf("\t\t\t\t\t\t\t\t\t  \r");
        printf("(>° °)>\t\t-\t🎵  Bailanding ...\r");
        fflush(stdout);
        sleep(1);

        printf("\t\t\t\t\t\t\t\t\t  \r");
        printf("<(° °<)\t\t-\t🎵  Bailanding ...\r");
        fflush(stdout);
        sleep(1);

        printf("\t\t\t\t\t\t\t\t\t  \r");
        printf("(>° °)>\t\t-\t🎵  Bailanding ...\r");
        fflush(stdout);
        sleep(1);


        printf("\t\t\t\t\t\t\t\t\t  \r");
        printf("<(° °<)\t\t-\t🎵  Bailanding ...\r");
        fflush(stdout);
        sleep(1);

        printf("\t\t\t\t\t\t\t\t\t  \r");
        printf("\\(° °)\t\t-\t🎵  Bailanding ...\r");
        fflush(stdout);
        sleep(1);
        printf("\t\t\t\t\t\t\t\t\t  \r");
    }
    printf("\\(^_^)/\t\t \n");

    printf("🎵 La música ha terminado.\n");
    sendMassive(fd,-3,jugadores_array,length);
    return ;
}