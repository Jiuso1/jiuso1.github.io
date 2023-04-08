#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>

bool alarmado = false; // Variable global que registra si ha sonado la alarma.
bool avisado = false;  // Valdrá true cuando A nos avise de que podemos escribir "Tercer Mensaje".

void rutina()
{
   alarmado = true; // Asignamos true a alarmado.
}
void funcionAvisado()
{
   avisado = true;
}

int main()
{
   int pidA = getppid();//El PID de A será el PID de nuestro padre.
   /*
   Declaramos dos variables para detectar cuándo ha pasado un segundo.
   Estas guardarán el tiempo en segundos desde las 00:00 del 01/01/1970.
   */
   time_t tiempoActual = 0, tiempoGuardado = 0;
   signal(SIGALRM, rutina); // Conectamos la señal de alarma con la función rutina.
   signal(10, funcionAvisado);
   alarm(4); // Ponemos la alarma en 4 segundos. Antes estaba con 3 pero con los pause necesitamos 4.
   pause();  // Paramos hasta que nos llegue la señal.
   if (avisado)
   {
      printf("Tercer Mensaje\n");
      while (!alarmado)
      { // Mientras no se haya alarmado...
         /*
         time es una función proporcionada por <time.h>. A esta le pasamos un
         puntero a un tipo time_t, y simplemenete esta función le asignará
         a la variable que le pasemos el tiempo transcurrido en segundos
         desde el 01/01/1970 a las 00:00.
         */
         time(&tiempoActual);
         tiempoGuardado = tiempoActual; // Guardaremos en tiempoGuardado tiempoActual.
         while (tiempoGuardado == tiempoActual)
         {                       // Mientras el tiempoActual siga siendo el mismo que el tiempo guardado...
            time(&tiempoActual); // Actualiza el tiempoActual.
            // Con este while conseguimos permanecer hasta que transcurra 1 segundo.
         }
         if (!alarmado)
         {                                   // Si no se ha alarmado...
            printf("Esperando alarma...\n"); // Escribe por pantalla.
         }
      }
      printf("Cuarto Mensaje\n");
      /*
      Deberíamos poder avisar a B de que ya puede escribir "Quinto Mensaje". El problema es que
      aquí tampoco conocemos a B, así que avisaremos a nuestro padre para que nuestro padre avise
      a C.
      */
      kill(pidA,10);//Avisamos a A.
   }
}