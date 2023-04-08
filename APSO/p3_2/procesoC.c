#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc,char *argv[]){
    int num = atoi(argv[1]); // Convertir el argumento a entero
    int pidB = getppid();
    close(1);
    printf("C: Aloha. Mi PID es %d, el pid de B es %d y el valor de num es: %d.\n",getpid(),pidB,num);
    return 0;
}