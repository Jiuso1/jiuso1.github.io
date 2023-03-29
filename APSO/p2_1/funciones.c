#include "funciones.h"

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>

void *pedirContador(int *cont)
{
    int contador = 1;//Le asignaremos 1 porque si lo inicializamos con 0 no entraría en el bucle. Es una solución chapucera pero funcional.
    while(contador != 0){
        printf("Introduzca el contador:\n");
        scanf("%d",&contador);   
        *cont = contador;//Al contenido del contador al que apuntemos le asignaremos el valor pedido por teclado. En este caso, al contenido de contadorMain (por haber llamado a esta función con &contadorMain en p2_1.c) le asignaremos lo pedido por teclado.
        sleep(1);//Esperamos 1 segundo por ejemplo.
    }
}
void *escribirContador(int *cont)
{
    int contador = 1;//Aquí también inicializamos con 1, por la misma razón.
    int operando = 1;//Operando que le sumaremos al contador del main. Al principio será 1.
    while(contador != 0){
        contador = *cont;//Al contador local de esta función le asignaremos el contenido del contador al que apuntemos.
        sleep(1);
        printf("El contador vale %d\n",contador);
        if(*cont == 0){
            exit(0);   
        }
        if(*cont != contador){//Si el contador del main cambia a lo que teníamos guardado antes del sleep.
            operando = *cont;//Cambiamos el operando.
            *cont = contador;//Volvemos a ponerle el contador que teníamos guardado.
        }
        *cont = *cont + operando;//Incrementamos en uno el valor del contador al que apuntemos (el del main).
    }
}
void *prueba(int *var){
    printf("Hola, esto es una prueba y se ha pasado como parametro %d\n",*var);//Como es un puntero, debemos escribir su contenido.
}