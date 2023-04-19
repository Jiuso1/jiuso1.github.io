#include <sys/types.h>
#include <sys/ipc.h>


// Definiciones de ventanas
#define VCOLALLEGAR 0
#define VFINREV 1
#define VABURRIDO 2
#define VCONTEORIA 3
#define VCONTODO 4

// Definiciones de aspecto
#define TIPOTEORIA 1
#define TIPOTODO 2

// Definiciones de operaciones
#define PINTAR 1
#define BORRAR 0

// Máximo de iconos para representar de cada tipo
#define MAXCABEZAS 6
#define MAXCUERPOS 5



// Información que se envia al servidor gráfico
struct tipo_elemento{
 long tipo; //obligatorio para la cola de mensajes
 int pid;   //del proceso que se dibuja
 int donde; //ventana en la que se pinta, según definiciones previas
 int que;   //Operación: PINTAR o BORRAR
 int cualidad; //Aspecto, según definiciones previas
 int cabeza; //Aspecto de la cabeza, número entre 0 y MAXCABEZAS-1
 int cuerpo; //Aspecto del cuerpo, número entre 0 y MAXCUERPOS-1
};
 



