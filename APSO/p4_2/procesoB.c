#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

char senhal = '\0';//A esta variable le asignaremos una letra en función de la que llegue antes.

void rutinaA(){
    if(senhal == '\0'){//Si la señal está inicializada a nada todavía (si nadie ha llegado antes)...
        senhal = 'A';
    }
}
void rutinaC(){
    if(senhal == '\0'){
        senhal = 'C';
    }
}
void rutinaAlarma(){
    if(senhal == '\0'){//Si la señal está inicializada a nada todavía (si nadie ha llegado antes)...
        senhal = 'L';//L de ALRM.
    }
}

int main(){
    int pidA = getppid(), pidC = 0, nAleatorio = 0, min = 3,max = 6, ret = 0;//El PID de A será igual al PID padre de B.
    signal(10,rutinaA);
    signal(12,rutinaC);//Usamos la 12 porque la 12 es SIGUSR2, la 10 es SIGUSR1. Creo que son señales pensadas para nosotros, los usuarios.
    signal(SIGALRM,rutinaAlarma);//La señal de alarma es 14, distinta a la señal 10 que empleamos en comunicación de procesos.
    printf("Ola bb, soi el proseso B.\n");
    pidC = fork();
    if(pidC != -1){
        if(pidC == 0){
            execl("procesoC","procesoC",NULL);
            exit(-1);
        }
    }else if(pidC == -1){
        perror("fork");
    }
    sleep(1);//Esperamos un segundo.
    kill(pidA,10);//Le mandaremos la señal 10 por ejemplo.
    kill(pidC,10);//Avisamos a C.
    srand(getpid());
    nAleatorio = (rand() % (max - min + 1)) + min;
    printf("El numero aleatorio vale %d\n",nAleatorio);
    alarm(nAleatorio);
    pause();//Esperamos hasta que nos llegue una señal.
    printf("Hola. senhal vale %c\n",senhal);
    switch(senhal){
        case 'L':{
            break;
        }
        case 'A':{
            ret = 1;
            break;
        }
        case 'C':{
            ret = 2;
            break;
        }
        default:{
            printf("Esto no tiene sentido...\n");
        }
    }
    printf("B ret %d.\n",ret);
    return ret;     
}