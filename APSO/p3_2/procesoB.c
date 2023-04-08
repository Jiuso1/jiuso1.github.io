#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main(){
    int nAleatorio = 0,min = 1, max = 10,pidC = 0,pidA = getppid();//Si A crea B, el PID del padre de B es el PID de A.
    char num_str[10] = "";//En esta cadena guardaremos el nº aleatorio.
    printf("B: Hola, me da verguenza hablarte, irradias belleza.\n");
    printf("B: Soy el proceso B, saludos navegante.\n");
    srand(getpid());
    nAleatorio = (rand() % (max - min + 1)) + min;
    pidC = fork();
    if(pidC != -1){
        if(pidC == 0){
            snprintf(num_str, 10, "%d", nAleatorio);//Usamos snprintf para convertir nAleatorio a cadena.
            execl("procesoC","procesoC",num_str,NULL);
            perror("execl");
            exit(-1);
        }
    }else if(pidC == -1){
        perror("fork");
    }
    printf("B: Mi PID es %d, el PID de A es %d y el PID de C es %d.\n",getpid(),pidA,pidC);
    sleep(1);//Espero un segundo antes de terminar.
    return 0;
}