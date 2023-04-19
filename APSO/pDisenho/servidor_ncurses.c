#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <errno.h>
#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <locale.h>

#include "comun.h"

/**** DECLARACION DE CONSTANTES ********************/
#define MAXCOLALLEGADA 45
#define MAXFINREV 45
#define MAXABURRIDOS 45
#define MAXCONTEORIA 2
#define MAXCONTODO 2

#define COLOR_COLALLEGADA 1
#define COLOR_FINREV 3
#define COLOR_ABURRIDOS 4
#define COLOR_CONTEORIA 5
#define COLOR_CONTODO 6
#define COLOR_MENS 7
#define COLOR_GRIS 8

// informacion que se almacena de cada cliente
struct alumno
{
  int elpid;
  int cualidad;
  wchar_t ncabeza;
  wchar_t ncuerpo;
};

/***** PROTOTIPOS DE FUNCONES *******************/
void pinta_escenario(WINDOW **vcolallegada, WINDOW **vfinrev, WINDOW **vaburridos, WINDOW **vconteoria, WINDOW **vcontodo, WINDOW **vmensajes);
void inserta(struct alumno *laventana, int maximo, struct tipo_elemento peticion);        //  int elpid, int cualidad);
void insertadelante(struct alumno *laventana, int maximo, struct tipo_elemento peticion); //  int elpid, int cualidad);
void quita(struct alumno *laventana, int maximo, int elpid);
void visualiza_peticion(WINDOW *vmensajes, struct tipo_elemento peticion);
void pinta_alumnos_inverso(WINDOW *ventana, struct alumno *datos_ventana, int maximo);
void pinta_alumnos(WINDOW *valumnos, struct alumno *datos_ventana1, int maximo);

void limpia_array(struct alumno *datos, int maximo);
void R12();
int crea_cola(key_t clave);

/***** DECLARACION DE VARIABLES GLOBALES ***********/

int fin = 0;
wchar_t cabeza[MAXCABEZAS];
wchar_t cuerpo[MAXCUERPOS];

/***************** MAIN ************************/

int main()
{

  srand(getpid());

  struct alumno datos_vcolallegada[MAXCOLALLEGADA];
  struct alumno datos_vfinrev[MAXFINREV];
  struct alumno datos_vaburridos[MAXABURRIDOS];
  struct alumno datos_vconteoria[MAXCONTEORIA];
  struct alumno datos_vcontodo[MAXCONTODO];

  setlocale(LC_ALL, "");

  cabeza[0] = L'\x1f466'; // 👦
  cabeza[1] = L'\x1f467'; // 👧
  cabeza[2] = L'\x1f468'; // 👨
  cabeza[3] = L'\x1f469'; // 👩
  cabeza[4] = L'\x1f471'; // 👱
  cabeza[5] = L'\x1f627'; //😧

  cuerpo[0] = L'\x1f454'; // 👔
  cuerpo[1] = L'\x1f455'; // 👕
  cuerpo[2] = L'\x1f457'; // 👚
  cuerpo[3] = L'\x1f45A'; // 👗
  cuerpo[4] = L'\x1f3bd'; // 🎽

  struct tipo_elemento peticion;
  key_t Clave1;
  int Id_Cola_Mensajes;
  int i;
  WINDOW *vcolallegada, *vfinrev, *vaburridos, *vconteoria, *vcontodo, *vmensajes;

  // Nos preparamos para recibir la senyal 12
  signal(12, R12);

  // Limpiamos todas los arrays
  limpia_array(datos_vcolallegada, MAXCOLALLEGADA);
  limpia_array(datos_vfinrev, MAXFINREV);
  limpia_array(datos_vaburridos, MAXABURRIDOS);
  limpia_array(datos_vconteoria, MAXCONTEORIA);
  limpia_array(datos_vcontodo, MAXCONTODO);

  // Dibujamos el escenario
  pinta_escenario(&vcolallegada, &vfinrev, &vaburridos, &vconteoria, &vcontodo, &vmensajes);

  // Creamos y abrimos la cola de mensajes
  Clave1 = ftok("./fichcola.txt", 18);
  Id_Cola_Mensajes = crea_cola(Clave1);

  kill(getppid(), 10); // avisamos a principal de que todo va bien para continuar

  // espero que lleguen peticiones a la cola de mensajes
  msgrcv(Id_Cola_Mensajes, (struct tipo_elemento *)&peticion, sizeof(struct tipo_elemento) - sizeof(long), 0, 0);

  while (!fin) // Esto acaba cuando llegue la senyal 10
  {
    usleep(200000);

    // Visualizo en la zona de mensajes la informacon sobre la peticion
    visualiza_peticion(vmensajes, peticion);

    // decodifico y ejecuto la peticion sobre los arrays
    switch (peticion.donde)
    {
    case VCOLALLEGAR:
      if (peticion.que == PINTAR)
        inserta(datos_vcolallegada, MAXCOLALLEGADA, peticion); //.pid, peticion.cualidad);
      if (peticion.que == BORRAR)
        quita(datos_vcolallegada, MAXCOLALLEGADA, peticion.pid);
      pinta_alumnos(vcolallegada, datos_vcolallegada, 20);
      break;

    case VFINREV:
      if (peticion.que == PINTAR)
        insertadelante(datos_vfinrev, MAXFINREV, peticion); //.pid, peticion.cualidad);
      if (peticion.que == BORRAR)
        quita(datos_vfinrev, MAXFINREV, peticion.pid);
      pinta_alumnos(vfinrev, datos_vfinrev, 27);
      break;

    case VABURRIDO:
      if (peticion.que == PINTAR)
        insertadelante(datos_vaburridos, MAXABURRIDOS, peticion); //.pid, peticion.cualidad);
      if (peticion.que == BORRAR)
        quita(datos_vaburridos, MAXABURRIDOS, peticion.pid);
      pinta_alumnos(vaburridos, datos_vaburridos, 27);
      break;

    case VCONTODO:
      if (peticion.que == PINTAR)
        inserta(datos_vcontodo, MAXCONTODO, peticion); //.pid, peticion.cualidad);
      if (peticion.que == BORRAR)
        quita(datos_vcontodo, MAXCONTODO, peticion.pid);
      pinta_alumnos(vcontodo, datos_vcontodo, 2);
      break;

    case VCONTEORIA:
      if (peticion.que == PINTAR)
        inserta(datos_vconteoria, MAXCONTEORIA, peticion); //.pid, peticion.cualidad);
      if (peticion.que == BORRAR)
        quita(datos_vconteoria, MAXCONTEORIA, peticion.pid);
      pinta_alumnos(vconteoria, datos_vconteoria, 2);
      break;
    }
    // espero que lleguen peticiones a la cola de mensajes
    msgrcv(Id_Cola_Mensajes, (struct tipo_elemento *)&peticion, sizeof(struct tipo_elemento) - sizeof(long), 0, 0);
  }

  endwin();                              // Finaliza el uso de ncurses
  msgctl(Id_Cola_Mensajes, IPC_RMID, 0); // Borra la cola de mensajes
}

/*********** FUNCION: crea_cola ********************************************************/
/*********** Obtiene acceso a la cola de mensajes con el id que se pasa ****************/
int crea_cola(key_t clave)
{
  int identificador;
  if (clave == (key_t)-1)
  {
    printf("Error al obtener clave para cola mensajes\n");
    exit(-1);
  }

  identificador = msgget(clave, 0600 | IPC_CREAT);
  // LA BORRAMOS POR SI TIENE MENSAJES DE EJECUCIONES ANTERIORES
  msgctl(identificador, IPC_RMID, NULL);
  identificador = msgget(clave, 0600 | IPC_CREAT);

  if (identificador == -1)
  {
    printf("Error al obtener identificador para cola mensajes\n");
    perror("msgget");
    exit(-1);
  }

  return identificador;
}

/*********** FUNCION: inserta **********************************************************/
/*********** Inserta un pid en el array si hay sitio, sino mata la proceso *************/
void inserta(struct alumno *laventana, int maximo, struct tipo_elemento peticion) // int elpid, int cualidad)
{
  int i = 0;

  while (laventana[i].elpid != 0 && i < maximo)
    i++;

  if (i == maximo)
    // kill(elpid,12); //mato al proceso porque no se puede representar
    printf("No es posible incluir el proceso en el array ... desbordamiento\n");
  else
  {
    laventana[i].elpid = peticion.pid;
    laventana[i].cualidad = peticion.cualidad;
    laventana[i].ncabeza = peticion.cabeza;
    laventana[i].ncuerpo = peticion.cuerpo;
    kill(peticion.pid, 10); // le digo al proceso que puede continuar
  }
}

/********** FUNCION: insertadelante*****************************************************/
/********** Inserta un pid al principio del array un array y lo reordena  **************/
void insertadelante(struct alumno *laventana, int maximo, struct tipo_elemento peticion)
{
  int i = 0;
  int j;

  while (laventana[i].elpid != 0 && i < maximo)
    i++;

  if (i == maximo)
    i--;

  for (j = i; j > 0; j--)
  {
    laventana[j].elpid = laventana[j - 1].elpid;
    laventana[j].cualidad = laventana[j - 1].cualidad;
    laventana[j].ncabeza = laventana[j - 1].ncabeza;
    laventana[j].ncuerpo = laventana[j - 1].ncuerpo;
  }
  laventana[j].elpid = peticion.pid;
  laventana[j].cualidad = peticion.cualidad;
  laventana[j].ncabeza = peticion.cabeza;
  laventana[j].ncuerpo = peticion.cuerpo;
  kill(peticion.pid, 10); // le digo al proceso que puede continuar
}

/********** FUNCION: elimina ***********************************************************/
/********** Elimina un pid de un array y lo reordena  *********************************/
void quita(struct alumno *laventana, int maximo, int elpid)
{
  int i = 0;
  int j;

  while (laventana[i].elpid != elpid && i < maximo)
    i++;

  if (i == maximo)
    printf("Error, se intenta borrar un pid que no esta");
  else
  {
    for (j = i; j < maximo - 1; j++)
    {
      laventana[j].elpid = laventana[j + 1].elpid;
      laventana[j].cualidad = laventana[j + 1].cualidad;
    }
    laventana[j].elpid = 0;
    laventana[j].cualidad = 0;
  }
}

/*********** FUNCION: pinta_escenario ***************************************************/
/*********** Dibuja el escenario *******************************************************/
void pinta_escenario(WINDOW **vcolallegada, WINDOW **vfinrev, WINDOW **vaburridos, WINDOW **vconteoria, WINDOW **vcontodo, WINDOW **vmensajes)
{
  initscr();     // INICIALIZA NCURSES
  start_color(); // INICIALIZA COLORES
  WINDOW *vprofetodo, *vprofeteoria, *vconteoriaUP, *vcontodoUP;

  int ANCHO = 7;

  // ESTO COMPRUEBA SI EL TERMINAL SOPORTA EL USO DE COLORES, SINO ACABA
  if (!has_colors())
  {
    endwin();
    printf("Esta terminal no tiene colores\n");
    kill(getppid(), 12); // mata al principal que espera conformidad con la 10
    exit(-1);
  }
  // COMPROBAMOS QUE TENEMOS LAS LINEAS Y COLUMNAS QUE NECESITAMOS
  if (LINES < 30 || COLS < 140)
  {
    endwin();
    printf("Se necesitan, al menos 30 lineas y 140 columnas, y tienes %d lineas y %d columnas\n", LINES, COLS);
    kill(getppid(), 12); // mata al principal que espera conformidad con la 10
    exit(-1);
  }

  // VISUALIZAMOS LA RESOLUCI ACUTAL DEL TERMINAL
  attron(COLOR_MENS);
  erase();
  //printw("La pantalla tiene %d lineas y %d columnas)", LINES, COLS);

  // Colores fondo: 17-23 52-58 88-95 125-130 161-166
  //  definimos los pares de colores
  init_pair(COLOR_FINREV, COLOR_YELLOW, 22);
  init_pair(COLOR_COLALLEGADA, COLOR_YELLOW, 17); //-->este si
  init_pair(COLOR_ABURRIDOS, COLOR_YELLOW, 52); // 130);//COLOR_CYAN);
  init_pair(COLOR_CONTEORIA, COLOR_YELLOW, 236);
  init_pair(COLOR_CONTODO, COLOR_YELLOW, 236);
  init_pair(COLOR_MENS, COLOR_WHITE, COLOR_BLACK);

  refresh();

  //***********

  *vfinrev = newwin(5, 138, 1, 2);
  wbkgd(*vfinrev, COLOR_PAIR(COLOR_FINREV));
  // wprintw(*vfinrev, "color");
  wattron(*vfinrev, A_BOLD);
  wrefresh(*vfinrev);

  *vcolallegada = newwin(5, 105, 13, 35);
  wbkgd(*vcolallegada, COLOR_PAIR(COLOR_COLALLEGADA));
  wattron(*vcolallegada, A_BOLD);
  wrefresh(*vcolallegada);

  vconteoriaUP = newwin(2, 14, 7, 20);
  wbkgd(vconteoriaUP, COLOR_PAIR(COLOR_CONTEORIA));
  wattron(vconteoriaUP, A_BOLD);
  wrefresh(vconteoriaUP);

  *vconteoria = newwin(6, 14, 9, 20);
  wbkgd(*vconteoria, COLOR_PAIR(COLOR_CONTEORIA));
  wattron(*vconteoria, A_BOLD);
  wrefresh(*vconteoria);

  vprofeteoria = newwin(8, 18, 7, 2);
  wbkgd(vprofeteoria, COLOR_PAIR(COLOR_CONTEORIA));
  wattron(vprofeteoria, A_BOLD);
  wattron(vprofeteoria, A_REVERSE);
  wprintw(vprofeteoria, "TEORIA");
  wattroff(vprofeteoria, A_REVERSE);

  wprintw(vprofeteoria, "\n          🤔\n");
  wprintw(vprofeteoria, "\n🕞     👴     \n");
  wprintw(vprofeteoria, "       👔   \n");
  wprintw(vprofeteoria, " ╔═══════════╗\n");
  wprintw(vprofeteoria, " ║           ║\n");
  // box(vprofeteoria,'+','+');
  wrefresh(vprofeteoria);

  vprofetodo = newwin(8, 18, 16, 2);
  wbkgd(vprofetodo, COLOR_PAIR(COLOR_CONTODO));
  wattron(vprofetodo, A_BOLD);
  wprintw(vprofetodo, "TODO");  
  wprintw(vprofetodo, "\n          🤔\n");
  wprintw(vprofetodo, "🕞\n      👵\n");
  wprintw(vprofetodo, "  🖨   👚   💻\n");
  wprintw(vprofetodo, " ╔═══════════╗\n");
  wprintw(vprofetodo, " ║           ║\n");
  wrefresh(vprofetodo);

  vcontodoUP = newwin(2, 14, 16, 20);
  wbkgd(vcontodoUP, COLOR_PAIR(COLOR_CONTODO));
  wattron(vcontodoUP, A_BOLD);
  wrefresh(vcontodoUP);

  *vcontodo = newwin(6, 14, 18, 20);
  wbkgd(*vcontodo, COLOR_PAIR(COLOR_CONTODO));
  wattron(*vcontodo, A_BOLD);
  wrefresh(*vcontodo);

  *vmensajes = newwin(5, 55, 7, 45);
  wbkgd(*vmensajes, COLOR_PAIR(COLOR_MENS));
  wrefresh(*vmensajes);


  *vaburridos = newwin(5, 138, 25, 2);
  wbkgd(*vaburridos, COLOR_PAIR(COLOR_ABURRIDOS));
  wattron(*vaburridos, A_BOLD);
  wrefresh(*vaburridos);
}

/************** FUNCION: visualiza_peticion  ******************************************************/
/************** Visualiza las peticiones recibidas en la ventana de mensajes *********************/
void visualiza_peticion(WINDOW *vmensajes, struct tipo_elemento peticion)
{
  // wchar_t *chw=0x1f458;//"😦";
  /*wchar_t cabeza[4];
  cabeza[0] = L'\x1f466';//👦
  cabeza[1] = L'\x1f467';//👧
  cabeza[2] = L'\x1f468';//👨
  cabeza[3] = L'\x1f469';//👩
 */

 char* sitios[]={"VCOLALLEGAR","VFINREV","VABURRIDO","VCONTEORIA","VCONTODO"};
 char* como[]={"NADA","TIPOTEORIA","TIPOTODO"};
 char* accion[]={"BORRAR","PINTAR"};


  werase(vmensajes);
  // wprintw(vmensajes,"Recibido  😦  mensaje:\n");
  // waddwstr(vmensajes, L"%lc\n",128550);
  wprintw(vmensajes, "Recibido mensaje\n"); // %lc:\n",cabeza[0]);
  wprintw(vmensajes, "\t pid: %d\n", peticion.pid);
  wprintw(vmensajes, "\t ventana: %s\n", sitios[peticion.donde]);
  wprintw(vmensajes, "\t operacion: %s\n", accion[peticion.que]);
  wprintw(vmensajes, "\t cualidad: %s\n", como[peticion.cualidad]);
  wrefresh(vmensajes);
}

/************** FUNCION: pinta_alumnos **********************************************************/
/************** Pinta los alumnos en la ventana que indiquemos *********************************/
void pinta_alumnos(WINDOW *ventana, struct alumno *datos_ventana, int maximo)
{

  int j;
  char tipo;

  werase(ventana);

  wprintw(ventana, "\n");
  for (j = 0; j < maximo; j++)
  {
    if (datos_ventana[j].elpid != 0)
    {
      if (datos_ventana[j].cualidad == TIPOTEORIA)
      {
        wprintw(ventana, "  ");

        wattron(ventana, A_REVERSE);
        tipo = 'T';
        // wprintw(ventana, " %c%02d ", tipo, datos_ventana[j].elpid % 100);
        wprintw(ventana, "%02d", datos_ventana[j].elpid % 100);
        wattroff(ventana, A_REVERSE);
        wprintw(ventana, " ");
      }
      else
      {
        tipo = 'N';
        // wprintw(ventana, " %c%02d ", tipo, datos_ventana[j].elpid % 100);
        wprintw(ventana, "  %02d ", datos_ventana[j].elpid % 100);
      }
    }
    else
    {
      wprintw(ventana, "     ");
    }
  }

  wprintw(ventana, "\n");
  for (j = 0; j < maximo; j++)
  {
    if (datos_ventana[j].elpid != 0)
    {
      wprintw(ventana, "  %lc ", cabeza[datos_ventana[j].ncabeza]);
    }

    else
    {
      wprintw(ventana, "     ");
    }
  }
  wprintw(ventana, "\n");
  for (j = 0; j < maximo; j++)
  {
    if (datos_ventana[j].elpid != 0)
    {
      wprintw(ventana, "  %lc ", cuerpo[datos_ventana[j].ncuerpo]);
    }

    else
    {
      wprintw(ventana, "     ");
    }
  }
  wprintw(ventana, "\n");
  for (j = 0; j < maximo; j++)
  {
    if (datos_ventana[j].elpid != 0)
    {
      wprintw(ventana, "  👖 ");
    }

    else
    {
      wprintw(ventana, "     ");
    }
  }

  wrefresh(ventana);
}

/************** FUNCION: pinta_alumnos inverso****************************************************/
/************** Pinta los alumnos en la ventana que indiquemos del último al primero*************/
void pinta_alumnos_inverso(WINDOW *ventana, struct alumno *datos_ventana, int maximo)
{
  int j;
  char tipo;

  werase(ventana);

  wprintw(ventana, "\n");
  for (j = maximo - 1; j >= 0; j--)
  {
    if (datos_ventana[j].elpid != 0)
    {
      if (datos_ventana[j].cualidad == TIPOTEORIA)
      {
        wprintw(ventana, "  ");

        wattron(ventana, A_REVERSE);
        tipo = 'T';
        // wprintw(ventana, " %c%02d ", tipo, datos_ventana[j].elpid % 100);
        wprintw(ventana, "%02d", datos_ventana[j].elpid % 100);
        wattroff(ventana, A_REVERSE);
        wprintw(ventana, " ");
      }
      else
      {
        tipo = 'N';
        // wprintw(ventana, " %c%02d ", tipo, datos_ventana[j].elpid % 100);
        wprintw(ventana, "  %02d ", datos_ventana[j].elpid % 100);
      }
    }
    else
    {
      wprintw(ventana, "     ");
    }
  }

  wprintw(ventana, "\n");
  for (j = maximo - 1; j >= 0; j--)
  {
    if (datos_ventana[j].elpid != 0)
    {
      wprintw(ventana, "  %lc ", cabeza[datos_ventana[j].ncabeza]);
    }

    else
    {
      wprintw(ventana, "     ");
    }
  }
  wprintw(ventana, "\n");
  for (j = maximo - 1; j >= 0; j--)
  {
    if (datos_ventana[j].elpid != 0)
    {
      wprintw(ventana, "  %lc ", cuerpo[datos_ventana[j].ncuerpo]);
    }

    else
    {
      wprintw(ventana, "     ");
    }
  }
  wprintw(ventana, "\n");
  for (j = maximo - 1; j >= 0; j--)
  {
    if (datos_ventana[j].elpid != 0)
    {
      wprintw(ventana, "  👖 ");
    }

    else
    {
      wprintw(ventana, "     ");
    }
  }

  wrefresh(ventana);
}

/************** FUNCION: limpia_array **********************************************************/
/************** Limpia los campos del array que indiquemos ************************************/
void limpia_array(struct alumno *datos, int maximo)
{
  int i;
  for (i = 0; i < maximo; i++)
  {
    datos[i].elpid = 0;
    datos[i].cualidad = 0;
  }
}

/************** FUNCION: R10 *******************************************************************/
/************** Atiende a la senyal 10 cuando llega *******************************************/
void R12()
{
  fin = 1; // cuando llega la senyal 10 finalizo el servidor.
}
