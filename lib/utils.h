
int generar_numero_aleatorio(int *array,int maxJugadores) {
    int voto;
    do {
        voto = (rand() % maxJugadores) ;
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

// int JugadorMax(int *array,int length, int pos){
//    int max = -1;
//    for (int ix = 0; ix < length ; ix++){
//             if (pos == ix){
//                 continue;
//             }
//             if (array[ix] > max)
//             {
//                 max = ix;
//             }
//     }
//     return  max;
// }

int JugadorMax(int *array, int length, int pos){
    int max_votes = -1;   // Variable para almacenar el número máximo de votos
    int jugador_max = -1; // Variable para almacenar el índice del jugador con más votos

    for (int ix = 0; ix < length; ix++) {
        if (pos == ix) {
            continue;  // Saltar el jugador que ya se ha eliminado
        }

        if (array[ix] > max_votes) {  // Comparar con el número de votos más alto encontrado
            max_votes = array[ix];    // Actualizar el número de votos más alto
            jugador_max = ix;         // Actualizar el índice del jugador con más votos
        }
    }

    return jugador_max;  // Devolver el índice del jugador con más votos
}
