#include <stdio.h>
#include <time.h>
#include "getnum.h"
#include "random.h"
#define FILS 3
#define COLS 5
#define BORRA_BUFFER while(getchar()!='\n')

typedef int tLinea[COLS];
typedef tLinea tCarton[FILS];
typedef struct tipoJugador 
{
	int lineas;
	int bingo;
	tCarton carton;
} tJugador;
typedef tJugador ** tJugadores;

void generaCarton ( tJugador * jugador );
int sacarBolilla ( int * bolillero, int * cantBol );
int buscarBolilla ( tJugador * jugador, int bolilla);
int controlarLineas ( tLinea linea);
void imprimirCarton ( tCarton carton );
int controlarCarton ( tJugador * jugador, int bolilla);
int jugar (int * bolillero, tJugadores jugadores, int cantJug, int primLineas[cantJug]);
int repite (tJugador * jugador, int num, int a, int b);

int repite (tJugador * jugador, int num, int a, int b)
{
  int  i, j;
  for (i=0; i<=a; i++)
  {
    for (j=0; (i<a && j<COLS) || (i==a && j<b); j++)
    {
      if (num==(*jugador).carton[i][j])
        return 1;
    }
  }
  return 0;
}

void generaCarton ( tJugador * jugador )
{
	int i, j, aux, rep;
	for (i=0; i<FILS; i++)
	{
		for (j=0; j<COLS; )
		{
				aux = randInt(1, 90);
				rep = repite(jugador, aux, i, j);
				if (!rep)
				{
				  (*jugador).carton[i][j++]=aux;
				}
		}
	}
	return;		
}

int sacarBolilla ( int * bolillero, int * cantBol )
{
	int i, aux, num;
	aux = randInt(0, *cantBol-1);
	num = bolillero[aux];
	for (i=aux+1; i<*cantBol; i++)
	{
		bolillero[i-1]=bolillero[i];
	}
	(*cantBol)-=1;
	printf("bolilla: %d\n", num);
	return num;
}

int buscarBolilla ( tJugador * jugador, int bolilla)
{
	int i, j, encontro=0;
	for (i=0; i<FILS && !encontro; i++)
	{
		for (j=0; j<COLS && !encontro; j++)
		{
			if ((*jugador).carton[i][j]==bolilla)
			{
				(*jugador).carton[i][j]=0;
				encontro=1;
			}
		}
	}
	return encontro;
}

int controlarLineas ( tLinea linea)
{
	int i, marcado=1;
	for (i=0; i<COLS && marcado; i++)
	{
		if (linea[i]!=0)
		{
			marcado=0;
		}
	}
	return marcado;
}

void imprimirCarton ( tCarton carton )
{
	int i, j;
	for (i=0; i<FILS; i++)
	{
		for (j=0; j<COLS; j++)
		{
			if (carton[i][j]!=0)
			{
				printf("%4d ", carton[i][j]);
			}	
		}
		printf("\n");
	}
	return;
}

int controlarCarton ( tJugador * jugador, int bolilla)
{
	int i, lineasComp=0;
	if (buscarBolilla( jugador, bolilla))
	{
		for (i=0; i<FILS; i++)
		{
			lineasComp += controlarLineas((*jugador).carton[i]);
		}			
		(*jugador).bingo = lineasComp==FILS;
	}
	return lineasComp;
}

int jugar (int * bolillero, tJugadores jugadores, int cantJug, int primLineas[cantJug])
{
	int huboLinea = 0;
	int bingo = 0;
	int bolilla, c, i, j, k; 
	int cantBol=90;
	int noAutomatico;

    noAutomatico=getint("\nPulse 0 para juego automatico, numero distinto de 0 para no\n");
	do 
	{
		if (noAutomatico)
		{
	  	    do
	  	    {
	  		    printf("\n\nPulse espacio para pedir bolilla, '0' para juego automatico o 'q' para terminar juego\n");		
		        c = getchar();
		        BORRA_BUFFER;

		        noAutomatico = c - '0';
		        if ( c=='q')
		            {
			            return 0;
	  	            }
		    } while ( c != ' ' && c != '0');
        }
		bolilla = sacarBolilla(bolillero, &cantBol);
		for (i=0; i<cantJug; i++)
		{
			(*jugadores[i]).lineas = controlarCarton(jugadores[i], bolilla);
			printf("~~~~~~~~~~~~~~\nCarton del jugador %d:\n", i+1);
			imprimirCarton((*jugadores[i]).carton);
		}
		if (!huboLinea)
		{
			for (j=0; j<cantJug; j++)
		    {
			    if ((*jugadores[j]).lineas)
			    {
			        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
			    	printf("Jugador %d hizo primera linea!!!\n", j+1);
			        printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
			        primLineas[j]=1;
			    	huboLinea=1;
			    }
			    else primLineas[j]=0;
		    }
		}
		
		for (k=0; k<cantJug; k++)
		{
			bingo = bingo*10 + (*jugadores[k]).bingo;
		}
	} while (!bingo);
	return bingo;
}

int main (void)
{
	srand(time(0));
	int bolillero[90];
	int ganadores, i, j, k, h, cantJug;
	tJugadores jugadores;
	for (i=0; i<90; i++)
	{
		bolillero[i]=i+1;
	}
	printf("\n\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("B B B BIIIIIINGOOOO O O O OOOO OO O\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	do
	{
		cantJug=getint("\n\nIngrese cantidad de jugadores (1~10)\n");
	}
	while (cantJug<1 || cantJug>10);
	printf("\n\n\n");
	int primLineas[cantJug];
	jugadores = malloc(cantJug*sizeof(jugadores[0]));
	for (j=0; j<cantJug; j++)
	{
		jugadores[j]=malloc(sizeof(tJugador));
		generaCarton(jugadores[j]);
		printf("~~~~~~~~~~~~~~~~~~~~~~\nCarton del jugador %d:\n", j+1);
		imprimirCarton((*jugadores[j]).carton);
	}
	ganadores=jugar(bolillero, jugadores, cantJug, primLineas);
	if (ganadores)
	{
	    printf("\n\n\n\n\n\nGanadores de primera linea:\n Jugadores ");
      for (h=0; h<cantJug; h++)
      {
          if (primLineas[h])
      		printf("%d  ", h+1);
      }
      printf("\n");
	}
	for (k=cantJug; ganadores>0; k--)
	{
		if (ganadores%2)
		{
			printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
			printf("EL JUGADOR %d HIZO BINGO!!!\n", k);
			printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
		}
		ganadores=ganadores/10;
	}
	printf("\n\n\n~ G A M E O V E R ~\n\n\n\n\n\n");
	return 0;
}