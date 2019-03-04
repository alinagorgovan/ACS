//Gorgovan Alina-Valentina 314CB
#include "struct.h"


int main(int argc, char *argv[])
{
	FILE *f, *g;

	//deschidere fisiere pentru citire si scriere
	f = fopen(argv[1], "rt");
	g = fopen(argv[2], "wt");


	int N, S, C;	/*numar de operatii, numar de stive, numar de cozi*/
	char op_name[10], tip_p;	/*numele operatiei si tipul parantezei*/
	int id_stiva, id_coada, id_p;	/*numarul stivei folosite, numarul cozii folosite si id paranteza*/ 

	int i;

	fscanf(f,"%d %d %d\n", &N, &S, &C);
	fscanf(f, "%s", op_name);
	
	//alocare memorie pentru un element de tip TParanteza
	TParanteza *p1 = (TParanteza*)malloc(sizeof(TParanteza));
	//void *p1 = (void*)p;
	//initiere vector de structuri de cozi si stive
	void **s = InitS(sizeof(TParanteza), S);
	void **c = InitC(sizeof(TParanteza), C);

	//citire operatii si apelare functii implementate
	for (i = 0; i < N; i++)
	{
		if (!strcmp(op_name, "push"))
		{
			fscanf(f,"%d %d %c", &id_stiva, &id_p, &tip_p);
			p1->id = id_p;
			p1->tip = tip_p;
			Push(s[id_stiva], (void*)p1);
		}
		else if (!strcmp(op_name, "intrq"))
		{
			fscanf(f, "%d %d %c", &id_coada, &id_p, &tip_p);
			p1->id = id_p;
			p1->tip = tip_p;
			intrq(c[id_coada], (void*)p1);

		}
		else if (!strcmp(op_name, "pop"))
		{
			fscanf(f,"%d", &id_stiva);
			Pop(s[id_stiva], (void*)p1);
		}
		else if (!strcmp(op_name, "extrq"))
		{
			fscanf(f,"%d", &id_coada);
			extrq(c[id_coada], (void*)p1);
		}
		else if (!strcmp(op_name, "sorts"))
		{
			fscanf(f,"%d", &id_stiva);
			sorts(s[id_stiva]);
		}
		else if (!strcmp(op_name, "sortq"))
		{
			fscanf(f,"%d", &id_coada);
			sortq(c[id_coada]);
		}
		else if (!strcmp(op_name, "prints"))
		{
			prints(s, S, g);
		}
		else if (!strcmp(op_name, "printq"))
		{
			printq(c, C, g);
		}
		else if (!strcmp(op_name, "corrects"))
		{
			fscanf(f, "%d", &id_stiva);
			corrects(s[id_stiva], g);
		}
		else if (!strcmp(op_name, "correctq"))
		{
			fscanf(f, "%d", &id_coada);
			correctq(c[id_coada], g);
		}

		if (i != N - 1) fscanf(f, "%s", op_name);
		else break;
	}

	//eliberari de memorie
	free(p1);
	DistrugeS(s, S);
	DistrugeC(c, C);
	fclose(f);
	fclose(g);
	
	return 1;
}