#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "comun.h"

bool llega10 = false;

void R10(){
    llega10 = true;
}
void R12(){
    printf("No es posible arrancar el servidor grafico.\n");    
    exit(-1);//Directamente aquí abort.
}

void visualiza(int cola,int donde,int que,int tipo,int cabeza,int cuerpo);

int main(){
    //Lancemos el servidor gráfico.
    int pservidorgraf = fork();
    signal(10,R10);
    signal(12,R12);

    //Lanzamos el servidor gráfico.
    if(pservidorgraf == 0){//Estoy en la copia.
        execl("servidor_ncurses","servidor_ncurses",NULL);
        perror("execl");
        exit(-1);
    }else if(pservidorgraf == -1){
        perror("fork");
        exit(-1);//Vamos a abortar también, de normal no lo hacíamos pero en esto nos interesa.
        //Si no creo el servidor gráfico, no me sirve de nada seguir.
    }
    //Esperamos que me avise por la 10 de que hay resolución.
    while(!llega10){//Mientras no llegue la 10 pause.
        pause();
    }
    llega10 = false;//Reseteo la variable para la vez siguiente que espere.

    //Si estoy aquí el servidor gráfico funciona. Abramos la cola.
    //Creamos y accedemos a la cola de mensajes.
    key_t clave = ftok("./fichcola.txt", 18);
    if(clave == -1){
        printf("Error al obtener la cola de mensajes\n");
        exit(-1);//Aborto si no he conseguido tener acceso a la cola.
    }
    int colagrafica = msgget(clave,0666|IPC_CREAT);//clave, sin bits especiales (0) y 0666 de permisos. Por si acaso, IPC_CREAT (si no existe créala).
    if(colagrafica == -1){
        printf("Error al obtener la clave\n");
        exit(-1);//Aborto.
    }
    //Aquí ya tengo acceso a la cola de mensajes. Ya puedo dibujar.

    int cabeza = 2;//Podríamos ponerlo aleatorio también con rand() % MAXCABEZAS.
    int cuerpo = 2;
    visualiza(colagrafica,VCOLALLEGAR,PINTAR,TIPOTODO,cabeza,cuerpo);//Lo pinto.
    sleep(2);
    visualiza(colagrafica,VCOLALLEGAR,BORRAR,TIPOTODO,cabeza,cuerpo);//Lo borro.
    visualiza(colagrafica,VCONTEORIA,PINTAR,TIPOTODO,cabeza,cuerpo);
    sleep(2);

    visualiza(colagrafica,VABURRIDO,PINTAR,TIPOTODO,cabeza,cuerpo);//Lo pinto.
    visualiza(colagrafica,VCONTODO,PINTAR,TIPOTODO,cabeza,cuerpo);//Lo pinto.
    visualiza(colagrafica,VFINREV,PINTAR,TIPOTODO,cabeza,cuerpo);//Lo pinto.

    sleep(10);
    //Apagaré el servidor gráfico (mandándole la 12).
    kill(pservidorgraf,12);


    return 0;
}
/*
Función visualiza.
Rellena la estructura con los parámetros y los envía a la cola gráfica.
*/
void visualiza(int cola,int donde,int que,int tipo,int cabeza,int cuerpo){//Creamos una función.
    struct tipo_elemento peticion;
    peticion.tipo = 1;
    peticion.pid = getpid();//Voy a visualizar mi PID.
    peticion.donde = donde;
    peticion.que = que;
    peticion.cualidad = tipo;//tipoteoria o tipotodo.
    peticion.cabeza = cabeza;
    peticion.cuerpo = cuerpo;
    
    msgsnd(cola,&peticion,sizeof(peticion)-sizeof(long),0);//Parámetros: cola, información, tamaño de la información, 0 si no quiero esperar.
    //Solo llega la 10 cuando pintas, no cuando borras.
    if(que == PINTAR){
        while(!llega10){//Mientras no llegue la 10 pause.
            pause();
        }
        llega10 = false;
    }
}