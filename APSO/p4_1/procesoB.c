#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>

bool autorizado = false;
/*Esta variable será true cuando el procesoA haya escrito lo que deba escribir antes.*/

void funcion(){
   autorizado = true;
}

int main(){
   int pidA = getppid();//El PID de A será el PID de nuestro padre, ya que A crea B.
   signal(10,funcion);
   pause();//Pausamos hasta que nos llegue una señal.
   if(autorizado){
      printf("Segundo Mensaje\n");
      autorizado = false;//Lo ponemos a false para que se deba activar nuevamente.
      /*
      B le debería mandar a C que ya puede escribir "Tercer Mensaje". El problema es que
      en B no podemos saber el pid de C, ni C tampoco puede saber el pid de B. Solo el
      padre conoce los PIDs de sus hijos. Por tanto, le mandaremos la señal a A y ya A
      se lo mandará a C.
      */
      kill(pidA,10);
   }
   pause();//Paramos otra vez. Cuando A nos avise, podremos escribir "Quinto Mensaje".
   if(autorizado){
      printf("Quinto Mensaje\n");
      autorizado = false;
      kill(pidA,10);//Avisamos a A de que ya puede escribir "Ultimo Mensaje". 
   }
   return 0;
}