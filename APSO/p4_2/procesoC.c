#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>

bool senhalB = false;
void rutina(){
    printf("C: Llego la senhal de B.\n");
    senhalB = true;
}

int main(){
    int nAleatorio = 0, min = 2, max = 5,pidB=getppid();//Como B es padre de C, el PID del padre de C será el PID de B. Parece un trabalenguas esto.
    signal(10,rutina);
    pause();//No comenzamos hasta que B nos mande la señal.
    if(senhalB == true){//Si le llega la señal procedente de B.
        srand(getpid());
        nAleatorio = (rand() % (max - min + 1)) + min;
        sleep(nAleatorio);//Generamos el número aleatorio pedido y hacemos sleep de dicho tiempo.
        printf("C: Mandamos 12 a B.\n");
        kill(pidB,12);//Mandamos la señal 12 a B.   
    }
    return 0;
}