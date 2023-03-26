#include <stdio.h>//Para el printf y el scanf.
#include <stdlib.h>//Para el system.
#include <math.h>//Para la función sqrt.
#include<unistd.h>//Para el sleep.

void pedirCoeficientes(int *_A,int *_B,int *_C){
    //A los scanf debemos pasarle punteros o direcciones de memoria, como nuestros parámetros ya son punteros no debemos poner &.
    system("clear");
    printf("Introduzca x²:\n");
    scanf("%d",_A);
    printf("Introduzca x:\n");
    scanf("%d",_B);
    printf("Introduzca el termino independiente:\n");
    scanf("%d",_C);
    printf("Coeficientes pedidos correctamente\n");
    sleep(2);
}
void escribirEcuacion(int _A,int _B,int _C){
    //A los printf les pasamos las variables sin más. Como son enteras empleamos %d.
    printf("La ultima ecuacion escrita es %dx² + %dx + %d\n",_A,_B,_C);
    sleep(5);
}
void escribirRaices(int _A,int _B,int _C){
    //(-b +- sqrt(b² - 4ac))/2a
    double raiz1 = 0,raiz2 = 0;
    if((pow(_B,2) - (4*_A*_C)) < 0){
        printf("Error, no se pueden hallar raices\n");
    }else{
        raiz1 = (-_B + sqrt(pow(_B,2) - (4*_A*_C))) / ((double)2*_A);
        raiz2 = (-_B - sqrt(pow(_B,2) - (4*_A*_C))) / ((double)2*_A);
        printf("Las raices son %f y %f\n",raiz1,raiz2);
    }
    sleep(5);
}
int menu(){
    int opcion = 0;
    do{
        system("clear");
        printf("Ecuaciones de segundo grado\n");
        printf("1. Leer Coeficientes\n");
        printf("2. Visualizar Ecuación\n");
        printf("3. Calcular Raices\n");
        printf("4. Salir\n");
        scanf("%d",&opcion);
    }while(opcion < 1 || opcion > 4);
}
void selector(int _A,int _B,int _C){
    int opcion;
    do{
        opcion = menu();
        switch(opcion){
        case 1:{
            pedirCoeficientes(&_A,&_B,&_C);
            /*
            Le pasamos las direcciones de memoria.             
            Esto es así porque tenemos los parámetros de pedirCoeficientes como punteros,
            para que alteren el valor de las variables. Estamos haciendo como cuando en
            C++ pasamos "por referencia", pero con punteros.
            */
            break;
        }
        case 2:{
            escribirEcuacion(_A,_B,_C);
            break;
        }
        case 3:{
            escribirRaices(_A,_B,_C);
            break;
        }
        case 4:{
            printf("Cerrando la app\n");
            break;
        }
    }
    }while(opcion != 4);    
}