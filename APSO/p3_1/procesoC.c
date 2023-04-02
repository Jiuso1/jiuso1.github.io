#include <stdio.h>
#include <unistd.h>

int main(){
    int pidC,pidA;
    pidC = getpid();
    pidA = getppid();
    printf("Proceso C: Saludos terricola. Soy muchas cosas, pero principalmente soy el proceso C.\n");
    printf("Proceso C: Mi PID es %d y el PID de A es %d.\n",pidC,pidA);
    return 0;
}

/*
#include "proceso.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void procesoA(){
    printf("Proceso A: Hola persona bonita, me presento, soy el proceso A.\n");
    int pid = getpid();//Con getpid obtendremos el PID y lo guardaremos en la variable pid.
    printf("Proceso A: Mi PID es %d.\n",pid);
}
void procesoB(){
    printf("Proceso B: Hola forastero o forastera, soy el proceso B, algunos me temen.\n");
    int pidB = getpid();
    int pidA;
    printf("Proceso B: Mi PID es %d y el PID de A es %d.\n",pidB,pidA);
}
void procesoC(){
    printf("Proceso C: Saludos terricola, yo soy... Que soy? Soy muchas cosas, pero principalmente soy el proceso C.\n");
    int pidC = getpid();
    int pidA;
    printf("Proceso C: Mi PID es %d y el PID de A es %d.\n",pidC,pidA);
}
*/