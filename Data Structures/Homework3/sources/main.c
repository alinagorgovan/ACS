#include "struct2.h"
TTrie AlocTrieNode(char c)
{
	//alocari memorie campuri din structura trie
	TTrie t = (TTrie)malloc(sizeof(Trie));
	if (!t) return NULL;
	t->copii = (ATrie)malloc(sizeof(TTrie));
	t->parinte = NULL;
	t->n = 0;
	t->valoare = c;
	t->ok = 0;

	return t;

}
TTrie add(TTrie t, char *cuvant)
{
	if (!t) return 0;
	if (!cuvant) return 0;
	int i, j, m, k = 0;
	int len = strlen(cuvant);

	TTrie p = t;

	for (i = 0; i < len; i++)
	{
		k = 0;
		//daca nu exista copii
		if(p->n == 0)
		{
			p->copii[0] = AlocTrieNode(cuvant[i]);
			(p->n)++;
			p->copii[0]->parinte = p;
			p = p->copii[0];
		}
		else
		{
			for(j = 0; j < p->n; j++)
			{
				//daca litera curenta exista deja in trie la nivelul curent 
				if(p->copii[j]->valoare == cuvant[i]) 
				{
					p = p->copii[j];
					k = 1;
				}
			}
			//daca
			if (k == 0)
			{
				m = p->n + 1;
				p->copii = (ATrie)realloc(p->copii, m * sizeof(TTrie));
				p->copii[m-1] = AlocTrieNode(cuvant[i]);
				p->copii[m-1]->parinte = p;
				(p->n)++;
				int k,l;
				TTrie temp;
				//sortare
				for(k = 1; k < p->n; k++)
				{
					for(l = 0; l < p->n - k; l++)
					{
						if((int)(p->copii[l]->valoare) > (int)(p->copii[l+1]->valoare))
						{
							temp = p->copii[l];
							p->copii[l] = p->copii[l+1];
							p->copii[l+1] = temp;
						}
					}
				}
				for(k = 0; k < p->n; k++)
				{
					if (p->copii[k]->valoare == cuvant[i])	{p = p->copii[k]; break;}
				}
			}
		}
		//daca s-a ajuns la sfarsitul cuvantului atunci se marcheaza in trie
		if(i == len - 1) p->ok = 1;
	}
	return t;
}
int find(TTrie t, char *cuvant)
{
	if (!t) return 0;
	if (!cuvant) return 0;
	int i,j, k = 0;
	int len = strlen(cuvant);
	TTrie p = t;
	//se parcurge trie-ul pana la terminarea cuvantului si se returneaza 1 daca s-a gasit
	for(i = 0; i < len; i++)
	{
		for(j = 0; j < p->n; j++)
		{
			if(p->copii[j]->valoare == cuvant[i])
			{
				k++;
				p = p->copii[j];
				break;
			}
		}

	}
	if (k != len) return 0;
	return p->ok;
}
void FreeNode(TTrie t)
{
	//functia elibereaza memoria ocupata de un nod
	int i;
	TTrie p = t->parinte;
	for(i = 0; i < p->n; i++)
	{
		if(p->copii[i]->valoare == t->valoare)
		{
			//se muta toti copiii la stanga si se realoca spatiul
			while(i < p->n - 1)
			{
				p->copii[i] = p->copii[i + 1];
				i++;
			}
			break;
		}
	}
	//se decrementeaza numarul de copiii
	(p->n)--;
	p->copii = (ATrie)realloc(p->copii, (p->n)*sizeof(TTrie));
	free(t);

}
void removee(TTrie t, char cuvant[])
{
	if (!t) return;
	if (!cuvant) return;
	if (!find(t, cuvant)) return;
	int i, j;
	int len = strlen(cuvant);
	TTrie p = t;
	//se parcurge trie-ul pana se ajunge pe ultimul nod din cuvant
	for(i = 0; i < len; i++)
	{
		for(j = 0; j < p->n; j++)
		{
			if(p->copii[j]->valoare == cuvant[i])
			{
				p = p->copii[j];
				break;
			}
		}
	}
	//se deselecteaza ca fiind cuvant
	p->ok = 0;
	//daca are copii nu se elibereaza memorie
	if (p->n > 0) return;
	//daca nu are copii se elibereaza memoria ocupata de nod
	else if (p->n == 0)
	{
		while(p->n == 0 && p != t)
		{
			TTrie aux = p;
			p = p->parinte;
			FreeNode(aux);
		}
	}
	return;
}
int find_aux(TTrie t, char *cuvant, int *nr)
{
	//functie auxiliara pentru find_longest_prefix
	//returneaza 1 daca exista o secventa de caract in trie
	//si prin efect lateral returneaza numarul de copiii
	if (!t) return 0;
	if (!cuvant) return 0;
	int i,j, k = 0;
	int len = strlen(cuvant);

	TTrie p = t;
	for(i = 0; i < len; i++)
	{
		for(j = 0; j < p->n; j++)
		{
			if(p->copii[j]->valoare == cuvant[i]){
				p = p->copii[j];
				k++;
				break;
			}
		}
	}
	*nr = p->n;
	if (k == len) return 1; //exista secventa de caractere in trie
	else return 0;//nu exista secventa de cod in trie
}

char *find_longest_prefix(TTrie t, char* cuvant)
{
	if (!t) return NULL;

	while(1)
	{
		int a,b;
		b = find_aux(t, cuvant, &a);
		if (a >= 0 && b == 1) {break;}
		else { cuvant[strlen(cuvant) - 1] = '\0';}
	}

	return cuvant;
}
void printTrie(TTrie t, char *word, int n, char *returned_string)
{    //functie recursiva ce intoarce un string continand toate cuvintele din trie
    int i;
    if (t == NULL) return;

    if (t->ok)
    {  
		word[n] = 0;
		strcat(returned_string, " ");
		strcat(returned_string, word);
    }
    for (i = 0; i< t->n;i++)
    {    
        if (t->copii[i]!= NULL)
        {
	        word[n] = t->copii[i]->valoare;
            printTrie(t->copii[i], word, n + 1, returned_string);
        }
    }           
}

void find_all_with_prefix(TTrie t, char *pref, char* returned_string)
{
	if (!t) return;
	if (t->n == 0) returned_string = "None"; 
	TTrie p = t;
	char *w = (char*)malloc(100*sizeof(char));
	int i,j, k = 0;
	if (pref)
	{
		for(i = 0; i < strlen(pref); i++)
		{
			for(j = 0; j < p->n; j++)
			{
				if(p->copii[j]->valoare == pref[i])
				{
					p = p->copii[j];
					k++;
					break;
				}
			}
		}
		if (k != strlen(pref)) returned_string = "None";
		else printTrie(p, w, 0, returned_string);
	}
	if (w) free(w);

}
float mean_length(char *pref, char *str)
{
	int len_sum = 0, nr = 0;
	char *token;
	char *aux = (char*)malloc(2000*sizeof(char));
	memcpy(aux, pref, strlen(pref));
	token = strtok(str, " ");
	if (token)
	{
		nr++;
		strcat(aux, token);
		len_sum += strlen(aux);
		strcpy(aux, pref);
	}
	while(token != NULL)
	{
		token = strtok(NULL, " ");
		if (token)
		{
			nr++;
			strcat(aux, token);
			len_sum += strlen(aux);
			strcpy(aux, pref);
		}
	}

	//free(aux);

	return ((float)len_sum)/nr;	
}
char *inputString(FILE* fp, size_t size)
{
//functie care sa citeasca un string de lungime necunoscuta
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size*=2));
            if(!str)return str;
        }
    }
    str[len++]='\0';

    return realloc(str, sizeof(char)*len);
}

int main(int argc, char *argv[])
{
	FILE *f, *g;

	//deschide fisiere pentru citire si scriere
	f = fopen(argv[1], "rt");
	g = fopen(argv[2], "wt");

	//numar de operatii
	int N, i;
	//numele operatiei, cuvantul, prefixul
	char *op_name, *w, *pref, *str;
	op_name = (char*)malloc(30 * sizeof(char));
	pref = (char*)malloc(100 * sizeof(char));
	str = (char*)malloc(500 * sizeof(char));
	TTrie tr = AlocTrieNode('$');

	fscanf(f, "%d", &N);

	for(i = 0; i < N; i++)
	{
		fscanf(f, "%s ", op_name);
		if (!strcmp(op_name, "add"))
		{
			w = inputString(f,20);
			tr = add(tr, w);
		}
		else if (!strcmp(op_name, "remove"))
		{
			w = inputString(f,20);
			removee(tr, w);
		}
		else if (!strcmp(op_name, "find"))
		{
			w = inputString(f,20);
			int rez = find(tr, w);
			if (rez) fprintf(g, "True\n");
			else fprintf(g, "False\n");
		}
		else if (!strcmp(op_name, "find_longest_prefix"))
		{
			w = inputString(f,20);
			char *aux =  (char*)malloc(50*sizeof(char));
			aux = find_longest_prefix(tr, w);
			fprintf(g, "%s\n", aux);
			free(aux);
		}
		else if (!strcmp(op_name, "find_all_with_prefix"))
		{
			int len = ftell(f);
			fscanf(f, "%s", pref);
			if(strcmp(pref, "add") && strcmp(pref, "remove") && strcmp(pref, "find") &&
			strcmp(pref, "find_longest_prefix") && strcmp(pref, "find_all_with_prefix") &&
			strcmp(pref, "mean_length"))
			{
				find_all_with_prefix(tr, pref, str);
				if (!strcmp(str, "None") || !strcmp(str, "")) fprintf(g, "None");
				else
				{
					memmove(str, str + 1, strlen(str));
					char *token;
					token = strtok(str, " ");
					if (find(tr, pref)) fprintf(g, "%s ", pref);
					if (token) fprintf(g, "%s%s ", pref, token);
					while(token != NULL)
					{
						token = strtok(NULL, " ");
						if (token) fprintf(g, "%s%s ", pref, token);
					}
				}
				fprintf(g, "\n");

			}
			else 
			{
				fseek(f, len, SEEK_SET);
				printTrie(tr, w, 0, str);
				memmove(str, str + 1, strlen(str));
				fprintf(g, "%s\n", str);
			}
			strcpy(pref, "");
			strcpy(str, "");
		}
		else if (!strcmp(op_name, "mean_length"))
		{
			int len = ftell(f);
			fscanf(f, "%s", pref);
			int k;
			if(strcmp(pref, "add") && strcmp(pref, "remove") && strcmp(pref, "find") &&
			strcmp(pref, "find_longest_prefix") && strcmp(pref, "find_all_with_prefix") &&
			strcmp(pref, "mean_length"))
			{
				find_all_with_prefix(tr, pref, str);
				k = 1;
			}
			else
			{
				k = 0;
				fseek(f, len, SEEK_SET);
				printTrie(tr, w, 0, str);
			}

			memmove(str, str + 1, strlen(str));
			if (!strcmp(str, "None") || !strcmp(str, "")) fprintf(g, "0\n");
			else
			{
				float rez;
				if (k == 1) rez = mean_length(pref, str);
				else rez = mean_length("", str);
				fprintf(g, "%.3f\n", rez);
			}
			strcpy(pref, "");
			strcpy(str, "");
		}
	}

	/*free(op_name);
	free(w);
	free(pref);
	free(str);*/
	fclose(f);
	fclose(g);

	return 0;
}