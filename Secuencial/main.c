#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#define cant_ciclos 5
#define cant_semanas 24
#define n 20


#define BLANCO=0, //Árbol podado.
#define AZUL=1, // Árbol enfermo con tratamiento antifúngico
#define ROJO=2, //Árbol enfermo con síntomas visibles
#define NARANJA=3, //Árbol enfectado con esporas (enfermo sin sintomas visibles)
#define VERDE=4, //Árbol sano

typedef struct {
    int estado;
    int edad; //joven - adulto - viejo
    int herida_abierta; // joven (23%) - adulto (8%) - viejos (37%)
    int tpo_contagio;
    int tpo_podado;
}Celda;

Celda **CrearMatriz();
float generadorUniforme_F(int,int,int); //devuelve valores reales hasta 1
int generadorUniforme_I(int,int,int); // devuelve valores enteros
void InicializarMatriz(Celda**);
void Proceso_Matriz(Celda**,Celda**);
Celda proceso_unaCelda(Celda,int);
void VisualizarMatriz(Celda** matriz);

int main() {
    clock_t start,finish;
    double duracion;
    double tpo_total=0;
    double promedio;
    double randomaux;
    srand(time(NULL));

    Celda **estadoActual=CrearMatriz();
    Celda **estadoSiguiente=CrearMatriz();
    Celda **Aux=CrearMatriz();

    for(int j = 0; j < cant_ciclos ; j++){
        randomaux=rand();
        srand(((rand()+randomaux)*13)*7);
        start=clock();
        InicializarMatriz(estadoActual);
        //VisualizarMatriz(estadoActual);
        //printf("\n");
        for(int i = 0;i<cant_semanas;i++){
            srand(((rand()+randomaux)*13)*7);
            Proceso_Matriz(estadoActual,estadoSiguiente);
            Aux=estadoSiguiente;
            estadoActual=Aux;
            estadoSiguiente=estadoActual;
            //VisualizarMatriz(estadoActual);
            //printf("\n");

        }
        finish=clock();
        duracion=(double)(finish-start)/ (clock_t)1000000;
        printf("\n---------------------------------------------\n");
        printf("Tiempos de la %d ejecucion\n",j+1);
        printf("Tiempo: %lf segundos.\n",duracion);
        printf("-----------------------------------------------\n");
        tpo_total+=duracion;
        duracion=0;
    }
    promedio=tpo_total/cant_ciclos;
    printf("\n---------------------------------------------\n");
    printf("Finalizado ciclo de %d ejecuciones\n",cant_ciclos);
    printf("Tiempo Total: %lf segundos.\n",tpo_total);
    printf("Tiempo promedio: %lf segundos.\n",promedio);
    printf("-----------------------------------------------\n");

    free((void*)estadoActual);
    free((void*)estadoSiguiente);

    return 0;
}

float generadorUniforme_F(int random, int a, int b){
    float result=((float)(random %(b-a+1) + a)/100.0f);
    return result;
}

int generadorUniforme_I(int random,int a, int b) {
    int result = a + (b - a) * generadorUniforme_F(random,0,100);
    return result;
}

void InicializarMatriz(Celda ** estado_Actual){
    Celda auxiliar;

    for(int i=0; i<n; i++){
        for(int e=0; e<n;e++){
            float d_prob= generadorUniforme_F(rand(),0,100);
            if(d_prob<=0.05){
                auxiliar.estado=2;
                auxiliar.tpo_contagio= (generadorUniforme_I(rand(),0,7));
            }else{
                if(d_prob<=0.15){
                    auxiliar.estado=3;
                    auxiliar.tpo_contagio=(generadorUniforme_I(rand(),0,7));
                }else{
                    if(d_prob<=0.35){
                        auxiliar.estado=1;
                        auxiliar.tpo_contagio=(generadorUniforme_I(rand(),4,7));
                    }else{
                        auxiliar.estado=4;
                        auxiliar.tpo_contagio=-1;
                    }
                }
            }

            d_prob= generadorUniforme_F(rand(),0,100);
            if(d_prob<0.30){
                auxiliar.edad=(generadorUniforme_I(rand(),1,156));
                if(generadorUniforme_F(rand(),0,100)<0.23){
                    auxiliar.herida_abierta=1;
                }else{
                    auxiliar.herida_abierta=0;
                }

            }else{
                if(d_prob<0.80){
                    auxiliar.edad=(generadorUniforme_I(rand(),157,1820));
                    if(generadorUniforme_F(rand(),0,100)<0.08){
                        auxiliar.herida_abierta=1;
                    }else{
                        auxiliar.herida_abierta=0;
                    }
                }else{
                    auxiliar.edad=(generadorUniforme_I(rand(),1821,2080));
                    if(generadorUniforme_F(rand(),0,100)<0.37){
                        auxiliar.herida_abierta=1;
                    }else{
                        auxiliar.herida_abierta=0;
                    }
                }
            }
            auxiliar.tpo_podado=-1;
            estado_Actual[i][e]=auxiliar;
        }
    }
}


Celda ** CrearMatriz(){
    Celda **Matriz, *M;
    Matriz= (Celda **)malloc (n*sizeof(Celda *));
    M = (Celda *) malloc(n*n*sizeof(Celda));
    for (int i=0; i<n; i++) {
        Matriz[i] = M + i*n;
    }
    return Matriz;
}

float fun_Contagio(float vecinos_con_sintomas,float susceptibilidad){
    float resultado= ((vecinos_con_sintomas + susceptibilidad)* 0.60) + 0.07;
    return resultado;
}

float fun_Susceptibilidad(int edad, int herida){
    float res=0;
    if(edad<= 156){
        res=0.35;
    }else{
        if(edad<= 1820){
            res=0.17;
        }else{
            res=0.63;
        }
    }
    if(herida){
        res+=0.15;
    }

    return res;
}

Celda proceso_unaCelda(Celda una_celda, int cant_enfermos){
    Celda celda_Aux;
    float random;
    celda_Aux.estado=una_celda.estado;
    celda_Aux.edad=una_celda.edad;
    celda_Aux.herida_abierta=una_celda.herida_abierta;
    celda_Aux.tpo_contagio=una_celda.tpo_contagio;
    celda_Aux.tpo_podado=una_celda.tpo_podado;

    if(una_celda.estado==2){ //ROJO
        random= generadorUniforme_F(rand(),0,100);
        if(una_celda.tpo_contagio>4 && random<0.85){
            celda_Aux.estado=1;
        }
        celda_Aux.tpo_contagio++;
    }else{
        if(una_celda.estado==1){ //AZUL
            if(una_celda.tpo_contagio>7){
                random= generadorUniforme_F(rand(),0,100);
                if(una_celda.edad<156){
                    if(random<=0.03){
                        celda_Aux.estado=0;
                        celda_Aux.tpo_contagio=-1;
                        celda_Aux.tpo_podado=0;
                    }else{
                        celda_Aux.estado=4;
                        celda_Aux.tpo_contagio=-1;
                    }
                }else{
                    if(celda_Aux.edad<1820){
                        if(random<=0.15){
                            celda_Aux.estado=0;
                            celda_Aux.tpo_contagio=-1;
                            celda_Aux.tpo_podado=0;
                        }else{
                            celda_Aux.estado=4;
                            celda_Aux.tpo_contagio=-1;
                        }
                    }else{
                        if(random<=0.53){
                            celda_Aux.estado=4;
                            celda_Aux.edad=52;
                            celda_Aux.herida_abierta=0;
                            celda_Aux.tpo_contagio=-1;
                            celda_Aux.tpo_podado=-1;
                        } else{
                            celda_Aux.estado=4;
                            celda_Aux.tpo_contagio=-1;
                        }
                    }
                }
            }else{
                celda_Aux.tpo_contagio++;
            }
        } else{
            if(celda_Aux.estado==0){ //BLANCO
                if(una_celda.tpo_podado==12){
                    celda_Aux.estado=4;
                    celda_Aux.tpo_podado=-1;
                }else{
                    celda_Aux.tpo_podado++;
                }
            } else{
                if(celda_Aux.estado==3){ //NARANJA
                    if(una_celda.tpo_contagio>3){
                        celda_Aux.estado=2;
                    }
                    celda_Aux.tpo_contagio++;
                }else{ //=VERDE
                    random= fun_Contagio(((float)cant_enfermos/12),fun_Susceptibilidad(una_celda.edad,una_celda.herida_abierta));
                    if(generadorUniforme_F(rand(),0,100)<random){
                        celda_Aux.estado=3;
                        celda_Aux.tpo_contagio=0;
                    }
                }
            }
        }
    }
    if(una_celda.edad<156){
        if(generadorUniforme_F(rand(),0,100)<0.23){
            celda_Aux.herida_abierta=1;
        }else{
            celda_Aux.herida_abierta=0;
        }
    } else{
        if(una_celda.edad<1820){
            if(generadorUniforme_F(rand(),0,100)<0.08){
                celda_Aux.herida_abierta=1;
            }else{
                celda_Aux.herida_abierta=0;
            }
        } else{
            if(generadorUniforme_F(rand(),0,100)<0.37){
                celda_Aux.herida_abierta=1;
            } else{
                celda_Aux.herida_abierta=0;
            }
        }
    }
    celda_Aux.edad++;
    return celda_Aux;
}


void Proceso_Matriz(Celda ** estadoActual,Celda ** estadoSiguiente) {
    int cant_vecinosEnfermos=0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if(estadoActual[i][j].estado==4){
                if (i + 1 < n) {
                    cant_vecinosEnfermos += estadoActual[i + 1][j].estado == 2;
                    if (i + 2 < n) {
                        cant_vecinosEnfermos += estadoActual[i + 2][j].estado == 2;
                    }
                }
                if (j + 1 < n) {
                    cant_vecinosEnfermos += estadoActual[i][j + 1].estado == 2;
                    if (j + 2 < n) {
                        cant_vecinosEnfermos += estadoActual[i][j + 2].estado == 2;
                    }
                }
                if (i - 1 > -1) {
                    cant_vecinosEnfermos += estadoActual[i - 1][j].estado == 2;
                    if (i - 2 > -1) {
                        cant_vecinosEnfermos += estadoActual[i - 2][j].estado == 2;
                    }
                }
                if (j - 1 > -1) {
                    cant_vecinosEnfermos += estadoActual[i][j - 1].estado == 2;
                    if (j - 2 > -1) {
                        cant_vecinosEnfermos += estadoActual[i][j - 2].estado == 2;
                    }
                }
                if (i - 1 > -1 && j + 1 < n) {
                    cant_vecinosEnfermos += estadoActual[i - 1][j + 1].estado == 2;
                }
                if (i - 1 > 1 && j - 1 > -1) {
                    cant_vecinosEnfermos += estadoActual[i - 1][j - 1].estado == 2;
                }
                if (i + 1 < n && j + 1 < n) {
                    cant_vecinosEnfermos += estadoActual[i + 1][j + 1].estado == 2;
                }
                if (i + 1 < n &&  j - 1 > -1) {
                    cant_vecinosEnfermos += estadoActual[i + 1][j - 1].estado == 2;
                }
                estadoSiguiente[i][j]= proceso_unaCelda(estadoActual[i][j],cant_vecinosEnfermos);
            }else{
                estadoSiguiente[i][j]= proceso_unaCelda(estadoActual[i][j],0);
            }
        }
    }
}


void VisualizarMatriz(Celda** matriz) { ///EL COLOR SOLO FUNCIONA EN LINUX, EN WINDOWS HAY QUE COMENTAR LOS PRINTS CON CODIGO DE ESCAPE  EJ: \033
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            char estado='-';
            switch (matriz[i][j].estado) {
                case ROJO:{
                    estado='R';
                    //printf("\033[0;31m");
                    break;
                }
                case AZUL:{
                    estado='A';
                    //printf("\033[0;34m");
                    break;

                }
                case BLANCO:{
                    estado='B';
                    // printf("\033[0;37m");
                    break;

                }
                case NARANJA:{
                    estado='N';
                    //printf("\033[0;33m");
                    break;

                }
                case VERDE:{
                    estado='V';
                    //printf("\033[0;32m");
                    break;
                }
            }
            if(matriz[i][j].herida_abierta){
                printf("|{%c}|",estado);
            }else {
                printf("|[%c]|", estado);
            }
            if(j==n-1){
                printf("\n");
            }
        }
    }
}



