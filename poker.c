#include <stdio.h>
#include <time.h>
#include "getnum.h"
#include "random.h"

typedef struct tipoCarta
{
    char * palo;
    int num;
} tCarta;
typedef tCarta ** tMazo;

int main (void)
{
    int puntosUs = 0;
    int puntosPc = 0;
    int hizoUs = 0;
    int hizoPc = 0;
    int numCartaUs = int numCartaPc;
    tMazo cartasUs;
    tMazo cartasPc;
    int dimMazo = 52;
    int j = 0;
    int h;
    char ** palos={"picas", "corazones", "trebol", "diamantes"};
    char ** jugada = {"", "par", "pierna", "poker"};
    cartasUs = malloc(5*sizeof(*cartasUs));
    cartasPc = malloc(5*sizeof(*cartasPc));
    for (h=0; h<5; h++)
    {
        (*cartasJug[h]).palo = malloc(10);
        (*cartasPc[h]).palo = malloc(10);
    }
    tMazo mazo = malloc(52*sizeof(*mazo));
    for (i=0; i<4; i++)
    {
        for ( ; j<(i+1)*13; j++)
        {
            mazo[j] = malloc(sizeof(tCarta));
            (*mazo[j]).palo = malloc(10);
            strcpy((*mazo[j]).palo, palos[i]);
            (*mazo[j]).num = j+1;
        }
    }
    mezclarCartas(mazo);
    for (k=0; k<5 && getint("Pulse 0 para cortar, 1 para seguir"); k++)
    {
        cartasUs = tomaCartas(mazo, &dimMazo);
        numCartUs = controlaCartas(cartasUs, &hizoUs);
        cartasPc = tomaCartas(mazo, &dimMazo);
        numCartaPc = controlaCartas(cartasPc, &hizoPc);
        if (hizoUs)
        {
            printf("Usuario hizo %s de %d\n", jugada[hizoUs], hizoUs);
        }
        else printf("Usuario no hizo nada\n");
        if (hizoPc)
        {
            printf("Pc hizo %s de %d\n", jugada[hizoPc], hizoPc);
        }
        else printf("Pc no hizo nada\n");
        if (hizoPc > hizoUs)
        {
            puntosPc++;
            printf("Punto para PC\n");
        }
        if else (hizoPc < hizoUs)
        {
            puntosUs++;
            printf("Punto para Usuario\n");
        }
        else
        {
            if (numCartaPc > numCartUs)
            {
                puntosPc++;
                printf("Punto para PC\n");
            }
            if else (numCartaPc < numCartUs)
            {
                puntosUs++;
                printf("Punto para Usuario\n");
            }
            else
            {
                printf("Empate!!!, punto para los dos\n");
                puntosUs++;
                puntosPc++;
            }
        }
        printf("Puntos:\nUsuario: %d\nPc:%d\n", puntosUs, puntosPc);
    }
    if (puntosPc > puntosUs)
    {
        printf("GANO PC\n");
    }
    else if (puntosPc < puntosUs)
    {
        printf("GANO USUARIO\n");
    }
    else printf("EMPATEEEEE!!!!\n");
    printf("GAME OVER\n");
    return 0;
}

void mezclarCartas(tMazo mazo)
{
    char palo[10];
    int num;
    for (i=0; i<26; i++)
    {
        aux = randInt(0, 51);
        strcpy(palo, (*mazo[i]).palo);
        num = (*mazo[i]).num;
        strcpy((*mazo[i]).palo, (*mazo[aux]).palo);
        (*mazo[i]).num = (*mazo[aux]).num;
        strcpy((*mazo[aux]).palo, palo);
        (*mazo[aux]).num = num;
    }
    return;
}

tMazo tomaCartas(tMazo mazo, int * dimMazo)
{
    for (i=0; i<5; i++)
    {
        strcpy((*cartas[i]).palo, (*mazo[i]).palo);
        (*cartas[i]).num = (*mazo[i]).num;
    }
    mazo = mazo + 5;
    (*dimMazo)--;

    return cartas;
}

int controlaCartas(tMazo cartas, int * hizo)
{
    int poker = hayPoker(cartas);
    if (!poker)
    {
        int pierna = hayPierna(cartas);
        if (!pierna)
        {
            int par = hayPar(cartas);
            if (!par)
            {
                printf("lachonchatuyaydetuvieja\n");
                return 0;
            }
            else
            {
                (*hizo)=1;
                return par;
            }
        }
        else
        {
            (*hizo)=2;
            return pierna;
        }
    }
    else 
    {
        (*hizo)=3;
        return poker;
    }
}

int hayPoker (tMazo cartas){

    int numPoker;
    int distintas = 0;
    int poker = 0;
    for(i=0;i<2 && poker!=3;i++){
        for(j=i+1,distintas=poker=0;j<5 && distintas<2;j++){
            if ((*cartas[i]).num==(*cartas[j]).num)
                poker++;
            else{
                distintas++;
            }
        }
        numPoker=(*cartas[i]).num;
    }
    if (poker==3){
        return numpoker;
    }
    else{
        return 0;
    }
}

int hayPierna (tMazo cartas)
{
    int pierna = 0;
    int distintas = 0;
    int numPierna;
    for(i=0; i<3 && pierna!=2; i++)
    {
        for(j=i+1, distintas=pierna=0; j<5 && distintas<3; j++)
        {
            if ((*cartas[i].num)==(*cartas[j]).num)
            {
                pierna++;
            }
            else
            {
                distintas++;
            }
        }
        numPierna = (*cartas[i]).num;
    }
    if (pierna==2)
    {
        return numPierna;
    }
    return 0;
}

int hayPar (tMazo cartas)
{
    int huboPar = 0;
    int par = 0;
    for (i=0; i<4 && par<2; i++)
    {
        for (j=i+1; j<5; j++)
        {
            if ((*cartas[i]).num == (*cartas[j]).num)
            {
                if (huboPar < (*cartas[i]).num)
                {
                    huboPar = (*cartas[i]).num;
                }
                par++;
            }
        }
    }
    return huboPar;
}