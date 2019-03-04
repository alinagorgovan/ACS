#include "struct.h"


TLista AlocCelula(void *ae, size_t d)
{//aloca celula
	TLista aux = malloc(sizeof(TCelula)); //aloc celula auxiliara pentru inserare
	if (!aux) return 0;
	aux->info = malloc(d);  //se aloca spatiu pentru informatie
	if(!aux->info)
	{
		free(aux);
		return NULL;
	}
	memcpy(aux->info, ae, d);
	aux->urm = NULL;
	return aux;
}
TStiva *AlocStiva(size_t d)
{
	//functia aloca o stiva si initiaza elementele din structura
	TStiva *s = (TStiva*)malloc(sizeof(TStiva));
	if (!s) return NULL;
	DIME(s) = sizeof(d);
	VF(s) = NULL;

	return s;

}
TCoada *AlocCoada(size_t d)
{
	//functia aloca o coada si initiaza  elementele din structura
	TCoada* c = (TCoada*)malloc(sizeof(TCoada));
	if (!c) return NULL;
	IC(c) = NULL;
	SC(c) = NULL;
	DIMEC(c) = sizeof(d);

	return c;
}
void **InitS(size_t d, int n)
{
	//returneaza vectorul de structuri de tip stiva
	//d-dimensiunea unui element
	//n-numarul de stive din vector
	int i;
	//alocare vector de adrese de tip TStiva*
	AStiva *v = (AStiva*)malloc(n*sizeof(AStiva));
	if (!v) return NULL;
	for(i = 0; i < n; i++)
	{
		//aloca fiecare adresa din vectorul v cu dimensiunea structurii de tip stiva
		v[i] = AlocStiva(d);
	}

	return (void**)v;
}
void **InitC(size_t d, int n)
{	
	//returneaza vectorul de cozi
	//d-dim unui element din coada
	//n-numarul de cozi din vector
	int i;
	//alocare vector de adrese de tip TCoada*
	ACoada *v = (ACoada*)malloc(n*sizeof(ACoada));
	if (!v) return NULL;
	for (i = 0; i < n; i++)
	{
		v[i] = AlocCoada(d);
	}

	return (void**)v;
}
int Push(void *s, void *ae)
{
	//aloca celula cu informatia ae
	TLista aux = AlocCelula(ae, DIME(s));
	if(!aux) return 0;
	//se leaga noua celula de lista
	aux->urm = VF(s);
	//varful stivei devine noua celula
	VF(s) = aux;

	return 1;
}
void* Pop(void *s, void *ae)
{
	//daca lista este vida
	if (!VF(s)) return NULL;
	if (!ae) return NULL;
	TLista aux = VF(s);
	//se copiaza informatia din lista la adresa primita de parametru
	memcpy(ae, aux->info, DIME(s));
	//se muta varful stivei
	VF(s) = aux->urm;
	//se elibereza memoria ocupata de auxiliar
	free(aux->info);
	free(aux);

	return ae;
}
int intrq(void *c, void *ae)
{
	//se aloca o celula cu informatia ae
	TLista aux = AlocCelula(ae, DIMEC(c));
	if (!aux) return 0;
	//daca coada initiala este vida
	if (IC(c) == NULL)
	{
		IC(c) = aux;
		SC(c) = aux;
	}
	//daca coada este nevida
	else
	{
		SC(c)->urm = aux;
		SC(c) = aux;
	}
	return 1;
}
void* extrq(void *c, void *ae)
{
	if (!IC(c)) return NULL;
	TLista aux = IC(c);
	if (!aux) return NULL;
	if (!ae) return NULL;
	//se copiaza informatia de la inceputul stivei la adresa ae primita ca parametru
	memcpy(ae, aux->info, DIMEC(c));
	//se deplaseaza varful stivei
	IC(c) = aux->urm;
	//se elibereaza spatiul ocupat de auxiliar
	free(aux->info);
	free(aux);

	return ae;
}
void AfiParanteza(void *ae, FILE *f)
{
	//afiseaza paranteza
	TParanteza *p = (TParanteza*)ae;
	fprintf(f, "%c",p->tip);
}
void prints(void **s, int S, FILE *f)
{
	//printeaza vectorul de structuri de tip stiva
	int i;
	//aloca memorie pentru extragerea elementelor din stiva
	TParanteza* p1 = (TParanteza*)malloc(sizeof(TParanteza));
	void *p = (void*)p1;
	//alocare stiva auxiliara
	TStiva *d = AlocStiva(sizeof(TParanteza));

	for (i = 0; i < S; i++)
	{
		fprintf(f, "\"");
		//rastoarna stiva pentru a o afisa de la baza la varf
		while(VF(s[i]) != NULL)
		{
			p = Pop(s[i], p);
			Push(d, p);
		}
		//afisare fiecare element si reintroducere in stiva
		while(VF(d) != NULL)
		{
			p = Pop((void*)d, p);
			AfiParanteza(p, f);
			Push(s[i], p);
		}
		
		fprintf(f, "\"\n");
	}
	//eliberari memorie alocata
	free(d);
	free(p);

}
void printq(void **c, int C, FILE *f)
{
	//aloca memorie pentru extragerea elementelor din coada
	TParanteza* p1 = (TParanteza*)malloc(sizeof(TParanteza));
	void *p = (void*)p1;
	int i;
	//aloca coada auxiliara
	TCoada* d = AlocCoada(sizeof(TParanteza));

	for (i = 0; i < C; i++)
	{
		fprintf(f, "\"");
		//afisare fiecare element si salvarea acestora in coada auxiliara
		while(IC(c[i]) != NULL)
		{
			p = extrq(c[i], p);
			intrq(d,p);
			AfiParanteza(p, f);
		}
		fprintf(f,"\"\n");
		//reintroducerea elementelor in coada initiala
		while(d->ic != NULL)
		{
			p = extrq(d,p);
			intrq(c[i], p);
		}
	}
	//eliberari memorie alocata
	free(d);
	free(p);
}
int perechi(void *x1, void *x2)
{
	//
	TParanteza *p1 = (TParanteza*)x1;
	TParanteza *p2 = (TParanteza*)x2;

	if (p1->tip == ')' && p2->tip == '(')
		return 1;
	else if (p1->tip == ']' && p2->tip == '[')
		return 1;
	else if (p1->tip == '}' && p2->tip == '{')
		return 1;
	else
		return 0;
}
void corrects(void *s, FILE *g)
{
	int dif, max = 0;
	/*l-lungimea unui subsir de paranteze inchise corect
	k-1 daca s-a citit o paranteza inchisa 0 pentru deschisa
	m-devine 0 daca s-a citit o paranteza deschisa care nu se inchide
	max-lungimea maxima returnata
	j-1 daca sirul citit este corect si 0 cand apare ceva gresit*/
	//char p_inchise[3] = {')', ']', '}'};

	//alocari coada si stive auxiliare
	//TCoada *c = AlocCoada(DIME(s));

	TStiva *d = AlocStiva(DIME(s));

	TStiva *f = AlocStiva(DIME(s));

	TParanteza* p1 = (TParanteza*)malloc(sizeof(TParanteza));
	//TParanteza* q1 = (TParanteza*)malloc(sizeof(TParanteza));

	void *p = (void*)p1;
	//void *q = (void*)q1;

	while(VF(s) != NULL)
	{
		p = Pop(s, p);
		Push((void*)f, p);
	}
	while(VF(f) != NULL)
	{
		p = Pop((void*)f, p);
		Push(s, p);
		Push((void*)d, p);
	}
	//s ramane la fel, f goala, d este s
	int id = 0;
	while(VF(d) != NULL)
	{
		id = id + 1;
		p = Pop((void*)d, p);
		((TParanteza*)p)->id = id;
		Push((void*)f, p);
	}
	//d goala, f rasturnata cu id-uri modificate descrescatoare

	TStiva* h = AlocStiva(sizeof(int));
	int x = -1;
	Push((void*)h, (void*)(&x));

	while(VF(f) != NULL)
	{
		//printf("123\n");
		p = Pop((void*)f, p);
		//daca se extrage paranteza inchisa
		if (((TParanteza*)p)->tip == ')' || ((TParanteza*)p)->tip == ']' || ((TParanteza*)p)->tip == '}')
		{
			//printf("123\n");
			x = ((TParanteza*)p)->id;
			Push((void*)h, (void*)(&x));
			Push((void*)d, p);
		}
		//daca se extrage paranteza deschisa
		else
		{
			if (perechi(p, VF(d)->info))
			{
				//printf("123\n");
				Pop((void*)h, (void*)(&x));
				Pop((void*)d, p);
				if(VF(d) != NULL)
				{
					dif = ((TParanteza*)p)->id - ((TParanteza*)(VF(d)->info))->id;
					if (dif > max) max = dif;
				}
				else
				{
					dif = ((TParanteza*)p)->id;
					if (dif > max) max = dif;
				}
			}
			else
			{
				while(VF(d) != NULL)
				{
					Pop((void*)d, p);
					Pop((void*)h, (void*)(&x));
				}
			}
		}
	} 
	fprintf(g, "%d\n", max);
	free(d);
	free(f);
	free(p);
	//free(c);
	//free(q);

}
void correctq(void* c, FILE *g)
{//functia  primeste ca parametru o coada si returneaza lungimea celei mai lungi secvente
 //scoatem din coada cate un element si il introduc intr-o stiva
	//l-lungimea unui sir de parantexe corect
	//k-devine 1 daca s-a citit o paranteza deschis si 0 pt una inchisa
	//m-este 1 daca sirul continua sa fie corect si 0 daca este intrerupt
	//max-retine lungimea maxima a unui sir corect
	int l = 0, i, k = 0, m = 0, j = 0;
	int max = 0;

	char p_deschise[3] = {'(', '[', '{'};

	TStiva *d = AlocStiva(DIME(c));

	TCoada* f = AlocCoada(DIME(c));

	TParanteza* p1 = (TParanteza*)malloc(sizeof(TParanteza));
	TParanteza* q1 = (TParanteza*)malloc(sizeof(TParanteza));

	void *p = (void*)p1;
	void *q = (void*)q1;

	while(IC(c) != NULL)
	{
		p = extrq(c, p);
		for(i = 0; i < 3; i++)
		{
			if (((TParanteza*)p)->tip == p_deschise[i])
			{
				Push((void*)d,p);
				m = 1;
				k = 1;
				j = 1;
			}
		}
		if (k == 0 && d->vs != NULL)
		{
			if (perechi(p, d->vs->info)) 
			{
				l = l + 2;
				m = 1;
				Pop((void*)d, q);
				j = 1;
			}
			else 
			{
				m = 0;
				while(VF(d) != NULL)
				{
					Pop((void*)d, q);
				}
			}

		}
		if (k == 0 && j == 0 && VF(d) == NULL) m = 0;

		if(l > max) max = l;
		k = 0;
		if(!m) l = 0;
		j = 0;

		intrq((void*)f, p);
	}
	while(f->ic != NULL)
	{
		p = extrq((void*)f, p);
		intrq(c, p);
	}
	while(VF(d) != NULL)
	{
		Pop((void*)d, p);
	}
	free(d);
	free(f);
	free(p);
	free(q);
	fprintf(g,"%d\n", max);

}
int cmpP(void *x1, void *x2)
{
	//functie de comparare a doua elemente de tip TParanteza dupa id
	TParanteza *p1 = (TParanteza*)x1;
	TParanteza *p2 = (TParanteza*)x2;

	if (p1->id < p2->id) return 2;
	if (p1->id > p2->id) return 1;
	else return 0;
}
void sorts(void *s)
{
	//aloc pointeri pentru a salva ce se scoate din coada
	TParanteza* p1 = (TParanteza*)malloc(sizeof(TParanteza));
	TParanteza* q1 = (TParanteza*)malloc(sizeof(TParanteza));
	void *p = (void*)p1;
	void *q = (void*)q1;
	//aloc stiva auxiliara
	TStiva *d = AlocStiva(DIME(s));

	//cat timp stiva este nevida
	while(VF(s) != NULL)
	{
		//scoate un element din stiv
		p = Pop(s, p);
		//cat timp stiva d este nevida si elementul din varf este mai mica decat p
		//ordonare descrescatoare
		while (VF(d) != NULL && cmpP(VF(d)->info, p) == 2)
		{
			//se scoate elementul din d si se reintroduce in stiva
			q = Pop(d, q);
			Push(s, q);
		}
		//il reintroduce in stiva d
		Push(d, p);
	}
	while(VF(d) != NULL)
	{
		//se rastoarna stiva d in s care este acum ordonata crescator
		p = Pop(d, p);
		Push(s, p);
	}

	free(d);
	free(p);
	free(q);

}
void sortq(void *c)
{
	TParanteza* p1 = (TParanteza*)malloc(sizeof(TParanteza));
	TParanteza* q1 = (TParanteza*)malloc(sizeof(TParanteza));

	void *p = (void*)p1;
	void *q = (void*)q1;

	TStiva *d = AlocStiva(DIMEC(c));

	while(IC(c) != NULL)
	{
		p = extrq(c, p);
		while (VF(d) != NULL && cmpP(VF(d)->info, p) == 2)
		{
			q  = Pop(d, q);
			intrq(c, q);
		}
		Push(d, p);
	}
	while(VF(d) != NULL)
	{
		p = Pop(d, p);
		intrq(c, p);
	}
	free(d);
	free(p);
	free(q);
}
void DistrugeLG(ALista aL)
{ 
	//functia elibereaza memoria dintr-o celua
  	//adresa celulei eliminate
  	TLista aux = *aL;
  	if (!aux) return; /* lista vida */
  	//elib.spatiul ocupat de element
  	free(aux->info);
  	//deconecteaza celula din lista
  	*aL = aux->urm;
  	//elibereaza spatiul ocupat de celula
  	free(aux);
}

void Distruge(ALista aL)
{
	//distruge o lista simplu inlantuita generica
  	while(*aL != NULL)
    	DistrugeLG(aL);
}

void DistrugeS(void **s, int n)
{
	//distruge un vector de structuri de tip stiva
	int i;
	for (i = 0; i < n; i++)
	{
		//pentru fiecare element din vector elibereaza spatiul ocupat de elemente
		Distruge(&(VF(s[i])));
		//elibereaza adresa pointer-ului
		free(s[i]);
	}
	free(s);
}
void DistrugeC(void **c, int n)
{
	//distruge un vector de structuri de tip coada
	int i;
	for (i = 0; i < n; i++)
	{
		//pentru fiecare element din vector elibereaza spatiul ocupat de elemente
		Distruge(&(IC(c[i])));
		free(c[i]);
	}
	free(c);
}