
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

int JugadorMax(int *array,int length, int pos){
   int max = -1;
   for (int ix = 0; ix < length ; ix++){
            if (pos == ix){
                continue;
            }
            if (array[ix] > max)
            {
                max = ix;
            }
    }
    return  max;
}