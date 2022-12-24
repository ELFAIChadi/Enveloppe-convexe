#include <stdio.h>
#include <stdlib.h>
#include "util.h"

#include "../include/list.h"
#include "../include/tree.h"

void ShowMessage(char * msg, int interrupt) { 
	fprintf(stderr, "%s\n", msg);
	if (interrupt) {
		fprintf(stderr, "An error has occured. The execution of the program will be interrupted!\n");
		exit(1);
	}
}

void viewInt(const void *i) {
	printf("%d", *((int*)i));
}

void freeInt(void * i) {
	free((int*) i);
}

int intSmallerThan(const void* a, const void* b) {
	if (*((int*) a) < *((int*) b))
		return 1;
	else
		return 0;
}

int intGreaterThan(const void* a, const void* b) {
	if (*((int*) a) > *((int*) b))
		return 1;
	else
		return 0;
}

/* Calcul de la position d'une noeud dans un arbre (s'il est dans le FG ou le FD) */
int recherchePosition(int *pos)
{
	int puissance = 2;
	int copiePos = *pos;

	while (puissance < copiePos)
	{
		copiePos -= puissance;
		puissance *= 2;
	}

	if (copiePos <= puissance / 2)
	{
		*pos -= puissance / 2;
		return 1;
	}
	else if (copiePos > puissance / 2)
	{
		*pos -= puissance;
		return 2;
	}
	else
		ShowMessage("ERREUR recherchePosition(int *pos) : résultat de fonction différent de 1 ou 2\n", 1);
}

int recherchePositionPere(int pos)
{
	if (pos % 2 == 1) return pos / 2;
	else return pos / 2 - 1;
}

/* Cette fonction ne sert à rien, juste pour faire des tests */
void liberer(void* data)
{
	printf("*liberation de %d*\n", *(int*)data);
}


	