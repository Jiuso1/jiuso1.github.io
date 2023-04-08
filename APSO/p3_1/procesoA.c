/*
Para saber qué debes incluir, haz man de lo que quieras usar en tu código. Por ejemplo:
man 2 open
man wait
man printf
En general, usaremos los capítulos 2 y 3 de man, así que busca por ahí.
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    int pidA_B,pidB_B,pidA,pidB,pidC,pidC_C,pidA_C;
    pidB = fork();//Creamos la copia que se convertirá en el proceso B.
    if(pidB != -1){
        if(pidB == 0){//Si somos la copia (el proceso B)...
            printf("Proceso B: Hola forastero o forastera, soy el proceso B. Algunos procesos me temen...\n");
            pidB_B = getpid();//Inicializamos una variable llamada pidB_B (PID de B mirándolo desde B).
            pidA_B = getppid();//Igual que arriba pero con pidA_B (PID de A mirándolo desde B).
            printf("Proceso B: Mi PID es %d y el PID de A es %d.\n",pidB_B,pidA_B);
            execl("procesoB","procesoB",NULL);//Borramos el código de la copia y lo sustituimos por el de procesoB.
            exit(-1);
            /*
            Importantísimo el exit o ejecutaremos sin querer la sección de código que queda fuera del if.
            Si execl falla, ejecutaremos lo de debajo, con lo cual exit(-1). Si no quisiésemos indicar fallo a la hora
            de hacer exit pondríamos exit(0).
            
            He querido poner los prinft de B antes del execl siguiendo los ejemplos de la Moodle, que ponen antes
            los printf que el execl. En el caso del proceso C sí he puesto los printf justo en el proceso C, pero como
            el proceso B hace muchas cosas prefiero dejarlo más limpito. De todas formas desconozco cómo lo pedirá Pedro,
            lo suyo es poner los printf donde se exija.
            */
        }
    }else if(pidB == -1){//Si da fallo el fork nos retorna -1.
        perror("fork");
    }   
    pidC = fork();//Creamos la copia que se convertirá en el proceso C.
    if(pidC != -1){
        if(pidC == 0){//Si somos la copia (el proceso C)...
            /*
            Recuerda la tabla de canales. Cada proceso tiene su propia tabla de canales.
            Si cerramos un canal, podremos sustituir el valor del mismo por el de un fichero por ejemplo.
            En este caso, nos piden sacar la salida estándar a un fichero llamado infoc, con lo cual
            debemos hacer close(1) y luego open.
            0: Entrada estándar.
            1: Salida estándar.
            2: Salida de errores.
            */
            close(1);//Cerramos la salida estándar para abrir y hacer que esta vaya al fichero que abramos con open.
            open("infoc.txt",O_WRONLY|O_CREAT,0666);//Usemos el ejemplo de manejo_ficheros de la Moodle.
            execl("procesoC","procesoC",NULL);
            exit(-1);//Esta línea no debería ejecutarse si execl ha funcionado bien.
        }
    }else if(pidC == -1){
        perror("fork");
    }
    printf("Proceso A: Hola persona bonita, me presento, soy el proceso A.\n");
    pidA = getpid();//Con getpid obtendremos el PID y lo guardaremos en la variable pidA.
    printf("Proceso A: Mi PID es %d.\n",pidA);
    printf("Proceso A: El PID de B es %d y el PID de C es %d.\n",pidB,pidC);
    //Esperaremos a B y a C para morir, no lo pone en la práctica pero creo que es conveniente.
    waitpid(pidB,NULL,0);
    waitpid(pidC,NULL,0);
    printf("Proceso A: Dame 4 segundos y me muero.\n");
    sleep(4);
    printf("Proceso A: Me mori. F.\n");
    return 0;
}