Práctica 6 de APSO
dup(2);//Coge la posición 2 y la pone en la primera posición libre que haya.
//Supongamos que la posición libre que encuentra es la 6. dup te dice dónde lo ha puesto, retornando un int:
int escritura = dup(2);
//escritura valdrá 6.
close(2);
open("dev/tty",O_WRONLY);//Recupero la escritura en la pantalla al asignarle a la 2 dev/tty.
/*
Recuerda que en Linux los dispositivos de E/S son como ficheros.
*/
write(esc,...);
perror(...);

Hay otro mecanismo de comunicación, más potente que las FIFOs y las PIPEs, las colas de mensaje.
Las colas de mensaje no están en disco duro, están en la memoria principal.
Ventajas:
-Son más rápidas.
-Permiten diferenciar cada mensaje con un tipo.

Se guardan con estructuras.
struct mensaje{
	long tipo;
	//El resto de elementos son los que yo quiera escribir, el único campo obligatorio es tipo (o como quieras llamarlo).
};

C.M: Cola de mensajes
A->C.M->B
Puedo tener varias estructuras o puedo tener una única estructura, como yo quiera. 

La información se mete por el final, están encolados.
Se lee la más antigua del tipo que quieras. No se lee el mensaje final, se lee el mensaje final del tipo que quieras. 

No es buena idea usar una misma FIFO o PIPE para varios procesos. Es mejor no usarlo. Más fácil tener dos FIFO o PIPEs que una única para dos procesos.
Con las C.M es más fácil, porque tendré una única C.M. para dos procesos. ¿Cómo sé que los mensajes que retiro son del otro proceso y no son mis propios mensajes? Por el tipo.
¿Y si meto un tercer proceso? Creo un tipo para ese tipo, es fácil tener una única C.M.

Hagamos un ejemplito...
A.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
int main(){
	printf("Soy A\n");
	
	//Creamos y abrimos la cola de mensajes.
	//Le tienes que dar una ruta a un fichero y un número, y si le das el mismo número generará el mismo identificador.
	//Lo ideal es usar un fichero propio, no compartir nunca el fichero con otros usuarios. Pongámosle por ejemplo el Makefile,
	//él con eso generará los identificadores.
	key_t identificador = ftok("./Makefile",2023);//Haz man ftok si quieres.
	if(identificador==-1){
		perror("ftok");
		exit(-1);
	}
	int cola=msgget(identificador,0600 | IPC_CREAT);
	if(cola == -1){
		perror("msgget");
		exit(-1);
	}
	/*
	man msgget. Elegimos los permisos que tenga la cola. 0 para indicar que no quiero bits especiales.
	*/
      	
	
	
	
	int vpidB = fork();
	if(vpidB == 0){
		execl("B","B",NULL);
		perror("execl");
		exit(-1)
	}else if(vpidB == -1){
		perror("fork");
	}
	
	int vpidC = fork();
	if(vpidC == 0){
		execl("B","B",NULL);
		perror("execl");
		exit(-1)
	}else if(vpidC == -1){
		perror("fork");
	}
	
	wait(NULL);//Espero que termine uno.
	wait(NULL);//Espero que termine otro.
	
	msgctl(cola,IPC_RMID,NULL);//Sirve para borrar la cola, si le pones IPC_RMID
	
	printf("Fin A\n");
		
	return 0;
}
Makefile
all: A B C
A: A.c
	cc A.c -o A
B: B.c
	cc B.c -o B
C: C.c
	cc C.c -o C

B.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct mensint{
	long tipo;
	int dato;
};

int main(){
	struct mensint mensaje, mensaje2;
	key_t identificador = ftok("./Makefile",2023);//Haz man ftok si quieres.
	if(identificador==-1){
		perror("ftok");
		exit(-1);
	}
	int cola=msgget(identificador,0600 | IPC_CREAT);
	if(cola == -1){
		perror("msgget");
		exit(-1);
	}
	printf("Soy B\n");
	mensaje.tipo = 1;//Pongámosle el 1 por ejemplo.
	mensaje.dato = 58;//Le ponemos 58 porque queremos.
	
	msgsnd(cola,&mensaje,sizeof(mensint)-sizeof(long),0);
	//msgsnd necesita la cola, la variable a escribir, el tamaño de la información de tu 
	//struct exceptuando el long que ya lo conoce y la condición o cosa que queramos. Para más info man msgsnd.
	
	//B manda un mensaje a C y esperará otro mensaje de A.
	msgsnd(cola,&mensaje,sizeof(mensaje)-sizeof(long),0);
	msgrcv(cola,&mensaje2,sizeof(mensaje2)-sizeof(long),1,0);//Espera un mensaje de tipo 1.		
	printf("Fin B\n");
	return 0;
}
C.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct mensint{
	long tipo;
	int dato;
};

int main(){
	struct mensint mensaje;
	
	key_t identificador = ftok("./Makefile",2023);//Haz man ftok si quieres.
	if(identificador==-1){
		perror("ftok");
		exit(-1);
	}
	int cola=msgget(identificador,0600 | IPC_CREAT);
	if(cola == -1){
		perror("msgget");
		exit(-1);
	}
	printf("Soy C\n");
	
	mensaje.tipo = 1;//Pongámosle el 1 por ejemplo.
	mensaje.dato = -58;//Le ponemos 58 porque queremos.
	
	printf("C: Esperando mensaje ....\n");
	msgsnd(cola,&mensaje,sizeof(mensaje)-sizeof(long),0);
	msgrcv(cola,&mensaje,sizeof(mensaje)-sizeof(long),2,0);//Para evitar problemas, ponemos tipos diferentes. Uno espera tipo 1 y otro tipo 
								//2.
								
	msgrcv(cola,&mensaje,sizeof(mensaje)-sizeof(long),2,0);//Para evitar problemas, ponemos tipos diferentes. Uno espera tipo 1 y otro tipo 
	printf("C: Recibido %d\n",mensaje.dato);
	
	printf("Fin C\n");
	return 0;
}
//Recordar en el examen del autosave.	
La cola de mensaje la crea cualquiera. Supongamos que la cree el A pero la podría crear cualquiera que la fuera a usar.
Cualquier proceso que tenga acceso a la cola puede leer y escribir (las dos cosas).

A, B y C deben tener acceso a la misma cola de mensajes.

¿Cómo se lee y como se escribe en una cola de mensajes?
msgrcv //msg receive para leer.
msgsnd //msg send para escribir.

Definamos los structs.

Si tú en msgrcv pones un tipo, no llega ese tipo y tienes 0 en el último parámetro, te quedas esperando.

De una ejecución a otra los mensajes se quedan en la cola. 

El comando
ipcs -q
te da el estado de las colas.

¿Cómo borro la cola de mensajes?
ipcs -q
Elijo la que borrar y pongo
ipcrm -q idDelacola
el id de la cola te lo dice msqid

La cola si no la borras está. Por eso debes preocuparte por borrar la cola.

Lo que no puedes hacer es usar dos tipos iguales con distintas estructuras, pero el resto yeah.

Cuando recibes desencolas (desaparece de la cola el mensaje).

Si no quieres esperar, pones msgrcv(...,IPC_NOWAIT);
Si quieres saber si te ha llegado o no el mensaje y no es basura...
if(msgrcv(...,IPC_NOWAIT) == -1){
	printf("Dato no disponible en este momento\n");
}else{
	printf("Mensaje recibido\n");
}

