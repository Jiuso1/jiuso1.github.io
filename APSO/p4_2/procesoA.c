#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

bool senhal_de_B = false;
void rutina(){
    senhal_de_B = true;
}

int main(){
    int pidB = 0,nAleatorio = 0, min = 2, max = 5, pidFin = 0,resultado = 0;//Inicializamos a cero por costumbre.
    signal(10,rutina);
    printf("Bienvenue! Soy el proceso A.\n");
    pidB = fork();
    if(pidB != -1){//Si no ha dado fallo el fork...
        if(pidB == 0){//Si soy la copia...
            execl("procesoB","procesoB",NULL);//Me convierto en procesoB.
            exit(-1);//Esta línea no debería ejecutarse, si se ejecuta mandamos -1 como aviso de error.
        }
    }else if(pidB == -1){//Si ha dado fallo el fork...
        perror("fork");
    }
    printf("Esperare a que B me mande senhal.\n");
    pause();
    if(senhal_de_B == true){
        printf("Ya me ha mandado la senhal.\n");
        srand(getpid());
        /*Le metemos una semilla, el PID cambia continuamente, entonces nos sirve como semilla 
        para hacer números aleatorios.*/
        nAleatorio = (rand() % (max - min + 1)) + min;
        //printf("%d\n",nAleatorio);
        sleep(nAleatorio);
        kill(pidB,10);//Le mandamos la señal 10 a B.
        pidFin = wait(&resultado);
        wait(NULL);//También esperaremos a C.
        /*Esperamos a que termine B y recogemos el resultado. wait espera a que muera un hijo,
        cualquier hijo. Si queremos especificar el hijo, debemos usar waitpid.
        Si queremos esperar a la muerte de varios hijos, debemos poner varios wait o varios waitpid.*/
        if(WIFEXITED(resultado)){//Si ha finalizado bien...
            switch(WEXITSTATUS(resultado)){//Mirar los mierdiapuntes para acordarse de las macros.
                case 0:{
                    printf("Juego abortado\n");
                    break;
                }
                case 1:{
                    printf("Gano Yo\n");
                    break;
                }
                case 2:{
                    printf("Gana C\n");
                    break;
                }
            }
        }
    }
    return 0;
}