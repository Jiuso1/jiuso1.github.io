#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(){
    int pidB = 0;
    printf("A: Hola Mundo. Soy el procesito A.\n");
    pidB = fork();
    if(pidB != -1){
        if(pidB == 0){//Si soy la copia.
            close(1);//Cerramos la salida estándar, es la que queremos.
            open("InfoB.txt",O_WRONLY|O_CREAT,0666);
            execl("procesoB","procesoB",NULL);
            perror("execl");
            exit(-1);//Estas dos últimas líneas no debería ejecutarse si la copia se ha convertido en B.
        }
    }else if(pidB == -1){
        perror("fork");
    }
    printf("A: Holi. Mi PID es %d y el PID de B es %d.\n",getpid(),pidB);
    sleep(2);//Esperamos dos segunditos.
    return 0;
}