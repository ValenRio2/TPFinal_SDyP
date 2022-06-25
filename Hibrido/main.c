#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <mpi.h>
#include <math.h>
#include <omp.h>


#define cant_ciclos 5
#define cant_semanas 24
#define n 20

enum Estado {
    BLANCO=0, //Árbol podado.
    AZUL=1, // Árbol enfermo con tratamiento antifúngico
    ROJO=2, //Árbol enfermo con síntomas visibles
    NARANJA=3, //Árbol enfectado con esporas (enfermo sin sintomas visibles)
    VERDE=4, //Árbol sano
};

typedef struct {
    enum Estado estado;
    int edad; //joven - adulto - viejo
    int herida_abierta; // joven (23%) - adulto (8%) - viejos (37%)
    int tpo_contagio;
    int tpo_podado;
}Celda;

Celda **CrearMatriz(int,int);
float generadorUniforme_F(int,int,int); //devuelve valores reales hasta 1
int generadorUniforme_I(int,int,int); // devuelve valores enteros
void InicializarMatriz(Celda**,int,int);
void set_seed_random(int);
void Proceso_Matriz(Celda**,Celda**,int, int,int);
Celda proceso_unaCelda(Celda,int);
void VisualizarMatriz(Celda** matriz,int,int);

int main(int argc, char **argv) {
    int id_proceso, nro_proceso;
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&id_proceso);
    MPI_Comm_size(MPI_COMM_WORLD,&nro_proceso);
    MPI_Datatype nueva_celda = nuevo_Tipo();
    int div= floor(n/nro_proceso);
    clock_t start,finish;
    double duracion;
    double tpo_total=0;
    double promedio;
    double randomaux;
    //srand(time(NULL));

    Celda ** MatrizActual;
    Celda ** MatrizSiguiente;
    Celda ** Matriz_auxiliar;

    if(id_proceso==0){
        MatrizActual=CrearMatriz(div+2,n);
        MatrizSiguiente=CrearMatriz(div+2,n);
    }else{
        if(id_proceso==nro_proceso-1){
            MatrizActual=CrearMatriz(div+2,n);
            MatrizSiguiente=CrearMatriz(div+2,n);
        } else{
            MatrizActual=CrearMatriz(div+4,n);
            MatrizSiguiente=CrearMatriz(div+4,n);
        }
    }

    for(int j = 0; j < cant_ciclos ; j++){
        set_seed_random(id_proceso);
        if(id_proceso==0){
            start=clock();
            InicializarMatriz(MatrizActual,0,div);
        }
        else{
            InicializarMatriz(MatrizActual,2,div+2);
        }

        for(int i = 0;i<cant_semanas;i++) {
            if (id_proceso % 2 == 1) {
                if (id_proceso == nro_proceso - 1) {
                    MPI_Recv(&(MatrizActual[0][0]), n, , id_proceso - 1, 0, MPI_COMM_WORLD, &status);
                    MPI_Recv(&(MatrizActual[1][0]), n, nueva_celda, id_proceso - 1, 0, MPI_COMM_WORLD, &status);
                    MPI_Send(&(MatrizActual[2][0]), n, nueva_celda, id_proceso - 1, 0, MPI_COMM_WORLD);
                    MPI_Send(&(MatrizActual[3][0]), n, nueva_celda, id_proceso - 1, 0, MPI_COMM_WORLD);
                    Proceso_Matriz(MatrizActual, MatrizSiguiente, 2, div + 2, div + 2);

                } else {
                    MPI_Recv(&(MatrizActual[0][0]), n, nueva_celda, id_proceso - 1, 0, MPI_COMM_WORLD, &status);
                    MPI_Recv(&(MatrizActual[1][0]), n, nueva_celda, id_proceso - 1, 0, MPI_COMM_WORLD, &status);
                    MPI_Recv(&(MatrizActual[div + 2][0]), n, nueva_celda, id_proceso + 1, 0, MPI_COMM_WORLD, &status);
                    MPI_Recv(&(MatrizActual[div + 3][0]), n, nueva_celda, id_proceso + 1, 0, MPI_COMM_WORLD, &status);
                    MPI_Send(&(MatrizActual[2][0]), n, nueva_celda, id_proceso - 1, 0, MPI_COMM_WORLD);
                    MPI_Send(&(MatrizActual[3][0]), n, nueva_celda, id_proceso - 1, 0, MPI_COMM_WORLD);
                    MPI_Send(&(MatrizActual[div][0]), n, nueva_celda, id_proceso + 1, 0, MPI_COMM_WORLD);
                    MPI_Send(&(MatrizActual[div + 1][0]), n, nueva_celda, id_proceso + 1, 0, MPI_COMM_WORLD);
                    Proceso_Matriz(MatrizActual, MatrizSiguiente, 2, div + 2, div + 4);
                }
            } else {
                if (id_proceso == 0) {
                    MPI_Send(&(MatrizActual[div - 2][0]), n, nueva_celda, id_proceso + 1, 0, MPI_COMM_WORLD);
                    MPI_Send(&(MatrizActual[div - 1][0]), n, nueva_celda, id_proceso + 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(&(MatrizActual[div][0]), n, nueva_celda, id_proceso + 1, 0, MPI_COMM_WORLD, &status);
                    MPI_Recv(&(MatrizActual[div + 1][0]), n, nueva_celda, id_proceso + 1, 0, MPI_COMM_WORLD, &status);
                    Proceso_Matriz(MatrizActual, MatrizSiguiente, 0, div, div + 2);
                } else {
                    if (id_proceso == nro_proceso - 1) {
                        MPI_Send(&(MatrizActual[2][0]), n, nueva_celda, id_proceso - 1, 0, MPI_COMM_WORLD);
                        MPI_Send(&(MatrizActual[3][0]), n, nueva_celda, id_proceso - 1, 0, MPI_COMM_WORLD);
                        MPI_Recv(&(MatrizActual[0][0]), n, nueva_celda, id_proceso - 1, 0, MPI_COMM_WORLD, &status);
                        MPI_Recv(&(MatrizActual[1][0]), n, nueva_celda, id_proceso - 1, 0, MPI_COMM_WORLD, &status);
                        Proceso_Matriz(MatrizActual, MatrizSiguiente, 2, div + 2, div + 2);
                    } else {
                        MPI_Send(&(MatrizActual[2][0]), n, nueva_celda, id_proceso - 1, 0, MPI_COMM_WORLD);
                        MPI_Send(&(MatrizActual[3][0]), n, nueva_celda, id_proceso - 1, 0, MPI_COMM_WORLD);
                        MPI_Send(&(MatrizActual[div][0]), n, nueva_celda, id_proceso + 1, 0, MPI_COMM_WORLD);
                        MPI_Send(&(MatrizActual[div + 1][0]), n, nueva_celda, id_proceso + 1, 0, MPI_COMM_WORLD);
                        MPI_Recv(&(MatrizActual[0][0]), n, nueva_celda, id_proceso - 1, 0, MPI_COMM_WORLD, &status);
                        MPI_Recv(&(MatrizActual[1][0]), n, nueva_celda, id_proceso - 1, 0, MPI_COMM_WORLD, &status);
                        MPI_Recv(&(MatrizActual[div + 2][0]), n, nueva_celda, id_proceso + 1, 0, MPI_COMM_WORLD,&status);
                        MPI_Recv(&(MatrizActual[div + 3][0]), n, nueva_celda, id_proceso + 1, 0, MPI_COMM_WORLD,&status);
                        Proceso_Matriz(MatrizActual, MatrizSiguiente, 2, div + 2, div + 4);
                    }
                }
                Matriz_auxiliar = MatrizSiguiente;
                MatrizSiguiente = MatrizActual;
                MatrizActual = Matriz_auxiliar;
            }
        }
        if(id_proceso==0){
            finish=clock();
            duracion=(double)(finish-start)/ (clock_t)1000000;
            printf("\n---------------------------------------------\n");
            printf("Tiempos de la %d ejecucion\n",j+1);
            printf("Tiempo: %lf segundos.\n",duracion);
            printf("-----------------------------------------------\n");
            tpo_total+=duracion;
            duracion=0;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    if(id_proceso==0){
        promedio=tpo_total/cant_ciclos;
        printf("-----------------------------------------------\n");
        printf("------Finalizado ciclo de %d ejecuciones--------\n",cant_ciclos);
        printf("Tiempo Total: %lf segundos.\n",tpo_total);
        printf("Tiempo promedio: %lf segundos.\n",promedio);
        printf("-----------------------------------------------\n");
    }
    MPI_Barrier(MPI_COMM_WORLD);
    free((void*)MatrizActual);
    free((void*)MatrizSiguiente);
    MPI_Finalize();
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

void set_seed_random(int id_process){
    srand(time(NULL));
    srand(rand()+(id_process+7)*13);

}

void InicializarMatriz(Celda ** estado_Actual, int inicio, int fin){
    Celda auxiliar;
    #pragma omp parallel for shared(estado_Actual) private(i,e,auxiliar) num_threads(3)
    for(int i=inicio; i<fin; i++){
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

MPI_Datatype nuevo_Tipo() {
    MPI_Datatype nueva_celda;
    int longitud[5] = {1,1,1,1,1};
    MPI_Aint desp[5];
    Celda celda_modelo;
    MPI_Aint direccion_base;
    MPI_Get_address(&celda_modelo, &direccion_base);
    MPI_Get_address(&celda_modelo.estado, &desp[0]);
    MPI_Get_address(&celda_modelo.edad, &desp[1]);
    MPI_Get_address(&celda_modelo.herida_abierta, &desp[2]);
    MPI_Get_address(&celda_modelo.tpo_contagio, &desp[3]);
    MPI_Get_address(&celda_modelo.tpo_podado, &desp[4]);

    desp[0] = MPI_Aint_diff(desp[0], direccion_base);
    desp[1] = MPI_Aint_diff(desp[1], direccion_base);
    desp[2] = MPI_Aint_diff(desp[2], direccion_base);
    desp[3] = MPI_Aint_diff(desp[3], direccion_base);
    desp[4] = MPI_Aint_diff(desp[4], direccion_base);

    MPI_Datatype tipos[5] = {MPI_INT,MPI_INT, MPI_INT, MPI_INT, MPI_INT};
    MPI_Type_create_struct(5, longitud, desp, tipos, &nueva_celda);
    MPI_Type_commit(&nueva_celda);

    return nueva_celda;
}

Celda ** CrearMatriz(int f, int m){
    Celda **Matriz, *M;
    Matriz= (Celda **)malloc (f*sizeof(Celda *));
    M = (Celda *) malloc(f*n*sizeof(Celda));
    for (int i=0; i<f; i++) {
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


void Proceso_Matriz(Celda ** estadoActual,Celda ** estadoSiguiente, int inicio, int fin, int lim) {
    int cant_vecinosEnfermos=0;
    #pragma omp parallel for shared(estadoActual, estadoSiguiente) private(i,j,cant_vecinosEnfermos) num_threads(3)
    for (int i = inicio; i < fin; i++) {
        for (int j = 0; j < n; j++) {
            if(estadoActual[i][j].estado==4){
                if (i + 1 < lim) {
                    cant_vecinosEnfermos += estadoActual[i + 1][j].estado == 2;
                    if (i + 2 < lim) {
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
                if (i + 1 < lim && j + 1 < n) {
                    cant_vecinosEnfermos += estadoActual[i + 1][j + 1].estado == 2;
                }
                if (i + 1 < lim &&  j - 1 > -1) {
                    cant_vecinosEnfermos += estadoActual[i + 1][j - 1].estado == 2;
                }
                estadoSiguiente[i][j]= proceso_unaCelda(estadoActual[i][j],cant_vecinosEnfermos);
            }else{
                estadoSiguiente[i][j]= proceso_unaCelda(estadoActual[i][j],0);
            }
        }
    }
}


void VisualizarMatriz(Celda** matriz, int inicio, int final) {
    for (int i = inicio; i < final; i++) {
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