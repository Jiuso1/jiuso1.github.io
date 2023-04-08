#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>

bool autorizadoAvisar = false;//Cuando valga true, podremos avisar a C.
void funcionAvisado(){//Esta función se ejecutará cuando B nos avise de que debemos avisar a C.
    autorizadoAvisar = true;
}

int main(){
    signal(10,funcionAvisado);
    int pidB = 0,pidC = 0;//Inicializamos a 0 por costumbre, realmente da igual porque luego le asignaremos a cada uno fork().
    pidB = fork();
    if(pidB != -1){//Si no ha dado error...
        if(pidB == 0){//Si soy la copia...
            execl("procesoB","procesoB",NULL);//Convertimos la copia a procesoB.c.
            exit(-1);//Esta línea no se debería ejecutar si execl ha ido guay. Ponemos -1 señalizando error.
        }
    }else if(pidB == -1){//Si el fork ha dado error...
        perror("fork");
    }
    pidC = fork();
    if(pidC != -1){
        if(pidC == 0){
            execl("procesoC","procesoC",NULL);
            exit(-1);
        }
    }else if(pidC == -1){
        perror("fork");
    }
    sleep(1);
    printf("Primer Mensaje\n");
    kill(pidB,10);//A le debe mandar a B que ya puede escribir "Segundo Mensaje".
    pause();//Paramos. Cuando volvamos a ejecución deberemos mandar la señal a C.
    if(autorizadoAvisar){
        kill(pidC,10);//Avisamos a C de que ya puede escribir "Tercer Mensaje".
        autorizadoAvisar = false;
    }
    pause();//Paramos hasta que C nos avise de que avisemos a B.
    if(autorizadoAvisar){
        kill(pidB,10);//Avisamos a B de que ya puede escribir "Quinto Mensaje".
        autorizadoAvisar = false;
    }
    pause();//Paramos hasta que B nos avise de que podemos escribir "Ultimo Mensaje".
    if(autorizadoAvisar){
        printf("Ultimo Mensaje\n");
        autorizadoAvisar = false;
    }
    return 0;
}