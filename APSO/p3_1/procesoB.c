#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int main(){
    int pidBcopia,pidB,pidPadreB;
    pidBcopia = fork();
    if(pidBcopia != -1){//Si no da fallo el fork...
        if(pidBcopia == 0){//Si soy la copia...
        /*
        Guardaremos los PID en variables ya que en la segunda iteración getppid ya no retorna el pid del padre correcto.
        Creo que esto se debe a que transcurrido un tiempo los procesos cambian de padre, tenemos que coger el padre
        justo al principio o no tendremos el padre original.
        Recordemos que el padre de B copia es B.
        */
            pidBcopia = getpid();
            pidPadreB = getppid();
            for(int i = 0;i < 3;i++){
                printf("Soy el proceso B Copia, mi pid es %d y el de mi padre es %d\n",pidBcopia,pidPadreB);
                sleep(1);
            }
            exit(0);
        }
    }else if(pidBcopia == -1){
        perror("fork");
    }
    pidB = getpid();
    for(int i = 0;i < 3;i++){
        printf("Soy el proceso B, mi pid es %d y el de mi copia es %d\n",pidB,pidBcopia);
        sleep(1);
    }
}   