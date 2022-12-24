#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "algo.h"
#include "geometry.h"
#include "list.h"
#include "tree.h"
#include "util.h"
#include "heap.h"
#include "sort.h"

/**
 * @brief Réalise la lecture d'un ensemble des points à partir du fichier
 * \p filename.
 * Renvoie 2 paramètres : un tableau contenant les points du fichier
 * \p filename, le nombre \p N de ces points.
 *
 * @param[in] filename le nom du fichier qui contient les points d'entrée.
 * @param[out] N le nombre des points dans le fichier \p filename
 * (paramètre de sortie).
 * @return Point** le tableau avec les points du fichier \p filename.
 */
static Point** readInstance(const char* filename, int* N)
{
	if (filename == NULL) ShowMessage("readInstance : filename ne doit pas etre NULL", 1);
	if (N == NULL) ShowMessage("readInstance : N ne doit pas etre NULL", 1);
	
	FILE* fichier = NULL;
    fichier = fopen(filename, "r"); 
    if (fichier == NULL) ShowMessage("readInstance : echec de l'ouverture du fichier", 1);
	fscanf(fichier, "%d", N);
	Point** tableauPointsEntree = (Point**)malloc(*N * sizeof(Point*));
	if (tableauPointsEntree == NULL) ShowMessage("readInstance : echec de l'allocation du tableau de points\n", 1);
	int x, y;

	for (int i = 0 ; i < *N ; i++)
	{
		fscanf(fichier, "%d %d\n", &x, &y);
		tableauPointsEntree[i] = newPoint(x, y);
	}

	fclose(fichier);
	return tableauPointsEntree;
}

/**
 * @brief Réalise l'écriture de l'ensemble des points de la liste \p L
 * dans un fichier avec le nom \p filename.
 *
 * @param[in] filename le nom du fichier d'écriture.
 * @param[in] L la liste des points à écrire dans le fichier \p filename.
 */
static void writeSolution(const char* filename, List* L)
{
	if (filename == NULL) ShowMessage("writeSolution : filename ne doit pas etre NULL", 1);
	if (L == NULL) ShowMessage("writeSolution : L ne doit pas etre NULL", 1);
	
	FILE* fichier = NULL;
    fichier = fopen(filename, "w+");
    if (fichier == NULL) ShowMessage("writeSolution : echec de l'ouverture du fichier", 1);
	fprintf(fichier, "%d\n", getListSize(L)); // Ecriture du nombre de points au début du fichier

	while (listIsEmpty(L) == 0) // Ecriture de tous les points
	{
		fprintf(fichier, "%lld %lld\n", X(getLNodeData(Head(L))), Y(getLNodeData(Head(L))));
		listRemoveFirst(L);
	}

	fclose(fichier);
}

/**
 * @brief Transforme la liste des arcs \p dedges décrivant les arêtes
 * du polygone de l'enveloppe convexe à une liste des poins ordonnés
 * dans le sens horaire.
 * 
 * @param[in] edges la liste des arcs de l'enveloppe convexe
 * @return List* la liste des points de l'enveloppe convexe dans le sens
 * horaire
 */
static List* DedgesToClockwisePoints(List* dedges)
{
	if (dedges == NULL) ShowMessage("DedgesToClockwisePoints : dedges ne doit pas etre NULL", 1);
	if (listIsEmpty(dedges) == 1) ShowMessage("DedgesToClockwisePoints : dedges ne doit pas etre vide\n", 1);

	DEdge** tableauArcs = (DEdge**)malloc(getListSize(dedges) * sizeof(DEdge*)); 
	if (tableauArcs == NULL) ShowMessage("DedgesToClockwisePoints : echec de l'allocation du tableau", 1);
	int N = getListSize(dedges);
	int i = 0;

	while (listIsEmpty(dedges) == 0)
	{
		tableauArcs[i] = (DEdge*)(getLNodeData(Head(dedges)));
		listRemoveFirst(dedges);
		i++;
	}

	for (int i = 0 ; i < N - 2 ; i++) // On essaye de "lier" les arcs entre eux, on les places de façon à ce qu'ils se suivent
		for (int j = i + 1 ; j < N ; j++)
			if (X(getDestination(tableauArcs[i])) == X(getOrigin(tableauArcs[j])) && Y(getDestination(tableauArcs[i])) == Y(getOrigin(tableauArcs[j])))
			{
				void* temp = tableauArcs[i + 1];
				tableauArcs[i + 1] = tableauArcs[j];
				tableauArcs[j] = temp;
			}

	List* listePointsSensHorraire = newList(&viewPoint, &freePoint);

	for (int i = 0 ; i < N ; i++)
		listInsertLast(listePointsSensHorraire, getOrigin(tableauArcs[i]));

	for (int i = 0 ; i < N ; i++) {
		freeDEdge(tableauArcs[i]);
	}
	free(tableauArcs);
	
	return listePointsSensHorraire;
}

void SlowConvexHull(const char* infilename, const char* outfilename)
{
	if (infilename == NULL) ShowMessage("SlowConvexHull : infilename ne doit pas etre NULL", 1);
	if (outfilename == NULL) ShowMessage("SlowConvexHull : outfilename ne doit pas etre NULL", 1);

	int N;
	Point** tableauPointsEntree = readInstance(infilename, &N);
	List* E = newList(&viewDEdge, &freeDEdge);

	if (N < 3) ShowMessage("SlowConvexHull : Impossible de calculer l'enveloppe convexe car il y a moins de 3 points", 1);

	for (int i = 0 ; i < N ; i++)
		for (int j = 0 ; j < N ; j++)
			if (i != j)
			{
				Point* A = tableauPointsEntree[i];
				Point* B = tableauPointsEntree[j];
				int valide = 1;

				for (int k = 0 ; k < N ; k++)
				{
					Point* P = tableauPointsEntree[k];

					if (P != A && P != B)
						if (onLeft(A, B, P) == 1 || isIncluded(A, B, P) == 1)
							valide = 0;
				}

				if (valide == 1)
					listInsertLast(E, newDEdge(A, B));
			}

	List* listePointsSensHorraire = DedgesToClockwisePoints(E);
	writeSolution(outfilename, listePointsSensHorraire);

	freeList(E, 0); 
	freeList(listePointsSensHorraire, 0);

	for (int i = 0 ; i < N ; i++) {
		freePoint(tableauPointsEntree[i]);
	}

	free(tableauPointsEntree);
}

/**
 * @brief Compare le points \p a et \p b.
 *
 * @param[in] a
 * @param[in] b
 * @return int si l'abscisse de \p a est plus petite que l'abscisse de \p b
 * renvoie 1, sinon renvoie 0. Dans le cas d'égalité, si l'ordonnée de \p a
 * est plus petite que l'ordonnée de \p b renvoie 1, sinon renvoie 0.
 */
static int smallerPoint(const void* a, const void* b) {
	if (a == NULL) ShowMessage("biggerPoint : a ne doit pas etre NULL", 1);
	if (b == NULL) ShowMessage("biggerPoint : b ne doit pas etre NULL", 1);

	a = (Point*)a;
	b = (Point*)b;

	if (X(a) < X(b))
		return 1;
	else if (X(a) == X(b) && Y(a) < Y(b))
		return 1;
	else
		return 0;
}

/**
 * @brief Compare le points \p a et \p b.
 * 
 * @param[in] a 
 * @param[in] b 
 * @return int si l'abscisse de \p a est plus grande que l'abscisse de \p b
 * renvoie 1, sinon renvoie 0. Dans le cas d'égalité, si l'ordonnée de \p a
 * est plus grande que l'ordonnée de \p b renvoie 1, sinon renvoie 0.
 */
static int biggerPoint(const void* a, const void* b)
{
	if (a == NULL) ShowMessage("biggerPoint : a ne doit pas etre NULL", 1);
	if (b == NULL) ShowMessage("biggerPoint : b ne doit pas etre NULL", 1);

	a = (Point*)a;
	b = (Point*)b;

	if (X(a) > X(b))
		return 1;
	else if (X(a) == X(b) && Y(a) > Y(b))
		return 1;
	else
		return 0;
}

void ConvexHull(const char* infilename, const char* outfilename, int sortby)
{
	if (infilename == NULL) ShowMessage("SlowConvexHull : infilename ne doit pas etre NULL", 1);
	if (outfilename == NULL) ShowMessage("SlowConvexHull : outfilename ne doit pas etre NULL", 1);
	if (sortby != 1 && sortby != 2 && sortby != 3) ShowMessage("SlowConvexHull : sortby ne doit valoir que 1, 2 ou 3", 1);

	int N;
	Point** tableauPointsEntree = readInstance(infilename, &N);
	if (N < 3) ShowMessage("SlowConvexHull : Impossible de calculer l'enveloppe convexe car il y a moins de 3 points", 1);

	switch (sortby)
	{
		case 1 :
			CBTHeapSort((void**)tableauPointsEntree, N, &smallerPoint, &viewPoint, &freePoint);
			break;
		case 2 :
			ArrayHeapSort((void**)tableauPointsEntree, N, &smallerPoint, &viewPoint, &freePoint);
			break;
		case 3 : 
			SelectionSort((void**)tableauPointsEntree, N, &smallerPoint);
			break;
	}
	
	List* Hsup = newList(&viewPoint, &freePoint);
	listInsertLast(Hsup, tableauPointsEntree[0]);
	listInsertLast(Hsup, tableauPointsEntree[1]);

	for (int i = 2 ; i < N ; i++)
	{
		listInsertLast(Hsup, tableauPointsEntree[i]);
		int validite = 0;

		while (getListSize(Hsup) > 2 && validite == 0)
		{
			Point* A = getLNodeData(Predecessor(Predecessor(Tail(Hsup))));
			Point* B = getLNodeData(Predecessor(Tail(Hsup)));
			Point* P = getLNodeData(Tail(Hsup));

			if (onLeft(A, B, P) == 1)
			{
				void* tmp = listRemoveLast(Hsup);
				listRemoveLast(Hsup);
				listInsertLast(Hsup, tmp);
			}
			else
			{
				validite = 1;
			}
		}
	}

	List* Hinf = newList(&viewPoint, &freePoint);
	listInsertLast(Hinf, tableauPointsEntree[N - 1]);
	listInsertLast(Hinf, tableauPointsEntree[N - 2]);

	for (int i = N - 3 ; i >= 0 ; i--)
	{
		listInsertLast(Hinf, tableauPointsEntree[i]);
		int validite = 0;

		while (getListSize(Hinf) > 2 && validite == 0)
		{
			Point* A = getLNodeData(Predecessor(Predecessor(Tail(Hinf))));
			Point* B = getLNodeData(Predecessor(Tail(Hinf)));
			Point* P = getLNodeData(Tail(Hinf));

			if (onLeft(A, B, P) == 1)
			{
				void* tmp = listRemoveLast(Hinf);
				listRemoveLast(Hinf);
				listInsertLast(Hinf, tmp);
			}
			else
			{
				validite = 1;
			}
		}
	}

	listRemoveLast(Hsup);
	listRemoveLast(Hinf);
	listConcatenate(Hsup, Hinf);
	writeSolution(outfilename, Hsup);
	freeList(Hsup, 0);

	for (int i = 0 ; i < N ; i++) {
		freePoint(tableauPointsEntree[i]);
	}

	free(tableauPointsEntree);
}

void RapidConvexHull(const char* infilename, const char* outfilename)
{
	if (infilename == NULL) ShowMessage("RapidConvexHull : infilename ne doit pas etre NULL", 1);
	if (outfilename == NULL) ShowMessage("RapidConvexHull : outfilename ne doit pas etre NULL", 1);

	int N;
	Point** tableauPointsEntree = readInstance(infilename, &N);
	if (N < 3) ShowMessage("SlowConvexHull : Impossible de calculer l'enveloppe convexe car il y a moins de 3 points", 1);
	
	int indicePointAbscisseMin = 0;

	for (int i = 1 ; i < N ; i++)
		if (X(tableauPointsEntree[indicePointAbscisseMin]) > X(tableauPointsEntree[i]))
			indicePointAbscisseMin = i;

	Point* temp = tableauPointsEntree[indicePointAbscisseMin];
	tableauPointsEntree[indicePointAbscisseMin] = tableauPointsEntree[0];
	tableauPointsEntree[0] = temp;

	List* H = newList(&viewPoint, &freePoint);
	listInsertFirst(H, tableauPointsEntree[0]);

	int i = 1;
	
	do {
		listInsertLast(H, tableauPointsEntree[i]);
		for (int j = 0; j < N ; j++)
		{
			Point* A = (Point*)(getLNodeData(Predecessor(Tail(H))));
			Point* B = (Point*)(getLNodeData(Tail(H)));
			Point* P = tableauPointsEntree[j];

			if (P != A && P != B) {
				if (onLeft(A, B, P) || isIncluded(A, B, P)) 
				{
					listRemoveLast(H);
					listInsertLast(H, P);
				}
			}
		}
		i++;
	} while (getLNodeData(Head(H)) != getLNodeData(Tail(H)));

	listRemoveLast(H);

	LNode* noeud = Head(H);
	int valide = 0;

	while (Successor(noeud) != NULL && valide == 0)
	{
		if (X(((Point*)(getLNodeData(noeud)))) == X(((Point*)(getLNodeData(Successor(noeud))))) && Y(((Point*)(getLNodeData(noeud)))) == Y(((Point*)(getLNodeData(Successor(noeud))))))
		{
			listRemoveNode(H, noeud);
			valide = 1;
		}
		noeud = Successor(noeud);
	}
	
	writeSolution(outfilename, H);
	
	freeList(H, 0);

	for (int i = 0 ; i < N ; i++) {
		freePoint(tableauPointsEntree[i]);
	}
	free(tableauPointsEntree);
	
}