#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//definitii structuri

#ifndef _CELULA_GENERICA_
#define _CELULA_GENERICA_

typedef struct celula
{
	void *info;
	struct celula *urm;
}TCelula, *TLista, **ALista;

#endif

#ifndef _STIVA_GENERICA_
#define _STIVA_GENERICA_

typedef struct
{
	size_t dime;
	TLista vs;	//varful stivei
}TStiva, *AStiva;

#endif

#ifndef _COADA_GENERICA_
#define _COADA_GENERICA_

typedef struct
{
	size_t dime;
	TLista ic;	//inceputul cozii
	TLista sc;	//sfarsitul cozii
}TCoada, *ACoada;

#endif

#ifndef _PARANTEZA_
#define _PARANTEZA_

typedef struct
{
	int id;
	char tip;
}TParanteza;

#endif

//macrodefinitii pentru acces campuri stucturi

#define VF(a) 	(((AStiva)(a))->vs)
#define DIME(a) (((AStiva)(a))->dime)
#define DIMDIF(s,d)	(DIME(s) != DIME(d))
#define VIDA(a) (VF(a) == NULL)

#define IC(a)  (((ACoada)(a))->ic)
#define SC(a)  (((ACoada)(a))->sc)
#define DIMEC(a)  (((ACoada)(a))->dime)

//anteturi functii

TLista AlocCelula(void *ae, size_t d);
TStiva *AlocStiva(size_t d);
TCoada *AlocCoada(size_t d);
void **InitS(size_t d, int n);
void **InitC(size_t d, int n);
int Push(void *s, void *ae);
void* Pop(void *s, void *ae);
int intrq(void *c, void *ae);
void* extrq(void *c, void *ae);
void AfiParanteza(void *ae, FILE *f);
void prints(void **s, int S, FILE *f);
void printq(void **c, int C, FILE *f);
int perechi(void *x1, void *x2);
void corrects(void *s, FILE *g);
void correctq(void* c, FILE *g);
int cmpP(void *x1, void *x2);
void sorts(void *s);
void sortq(void *c);
void DistrugeLG(ALista aL);
void Distruge(ALista aL);
void DistrugeS(void **s, int n);
void DistrugeC(void **c, int n);