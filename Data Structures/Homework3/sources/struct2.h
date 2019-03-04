#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct nod
{
	char valoare;
	struct nod **copii;
	struct nod *parinte;
	int n;
	int ok;
}Trie, *TTrie, **ATrie;