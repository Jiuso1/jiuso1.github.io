#include "funciones.h"
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int main(){
    int contadorMain = 10;//Inicializamos por costumbre, pero realmente da igual ya que lo pediremos por scanf a posteriori.
    pthread_t h1, h2;
    pthread_create(&h1,NULL,pedirContador,&contadorMain);//Lanzamos los hilos, estos se ejecutarán concurrentemente.
    pthread_create(&h2,NULL,escribirContador,&contadorMain);//A los dos les pasamos contadorMain, ambos trabajarán sobre el mismo contador. Todo esto es gracias a los parámetros de tipo puntero que hemos empleado, que trabajarán sobre la dirección de memoria de contadorMain (por eso &contadorMain).
    pthread_join(h1,NULL);//Muy importante poner los join al final, nunca se deben olvidar o generará problemas.
    pthread_join(h2,NULL);
    return 0;
}