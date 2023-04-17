int quedaPendiente = alarm(5);//Si la alarma se cancela, queda guardado en quedaPendiente el nº de segundos restantes.
alarm(0);//alarm cancela todas las alarmas pendientes.


int llega12 = 0;
//Las rutinas deben de ser muy pequeñas, solo deben cambiar el valor de variables.
void R12(){
	llega12=1;
}

int main(){
	if(!llega12){
		pause();
	}else{
		//Haz lo que tengas que hacer.
		llega12=0;//Reinicias la variable
	}
}

Un programa se puede parar por:
-pause().
-leer de una FIFO que no tenga datos. read(fifo,&_,_)
-leer de una cola con la opción 0 como último argumento (en vez de IPC_NOWAIT). msgrcv(cola,,,,,0)

Si llega una señal, en la FIFO no te saca de la espera pero en la cola sí te saca de la espera. Si he salido con -1 debería leer otra vez (porque no he leído bien).

Práctica
AnotaDescarga es la 12.

descargacarretilla vale true cuando la carretilla vale true.
while(descargacarretilla) usleep(1);//Mientras descargues la carretilla, no hagas nada. A esto se le llama pulling. Le metemos sleep para que no lo revise constantemente, o también podemos usar usleep (microsegundos. 1 microsegundo son 10⁻6 segundos.).

