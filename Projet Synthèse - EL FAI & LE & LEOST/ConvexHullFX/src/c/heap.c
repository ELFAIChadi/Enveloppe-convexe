#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "../include/tree.h"
#include "../include/heap.h"
#include "../include/util.h"

/**********************************************************************************
 * ARRAY HEAP
 **********************************************************************************/

int getAHMaxSize(const ArrayHeap* AH)
{
	if (AH == NULL) ShowMessage("getAHMaxSize : AH ne doit pas etre NULL", 1);
	return AH->MAX;
}

int getAHActualSize(const ArrayHeap* AH)
{
	if (AH == NULL) ShowMessage("getAHActualSize : AH ne doit pas etre NULL", 1);
	return AH->N;
}

void* getAHNodeAt(const ArrayHeap* AH, int pos)
{
	if (AH == NULL) ShowMessage("getAHNodeAt : AH ne doit pas etre NULL", 1);
	if (pos < 0 || pos >= getAHActualSize(AH)) ShowMessage("getAHNodeAt : valeur erronnee pour pos", 1);
	return AH->A[pos];
}

void decreaseAHActualSize(ArrayHeap* AH)
{
	if (AH == NULL) ShowMessage("decreaseAHActualSize : AH ne doit pas etre NULL", 1);
	if (getAHActualSize(AH) == 0) ShowMessage("decreaseAHActualSize : AH est deja vide", 1);
	AH->N = AH->N - 1;
}

void setAHNodeAt(ArrayHeap* AH, int position, void* newData)
{
	if (AH == NULL) ShowMessage("setAHNodeAt : AH ne doit pas etre NULL", 1);
	if (position < 0 || position >= AH->N) ShowMessage("setAHNodeAt : valeur erronnee pour pos", 1);
	AH->A[position] = newData;
}

/**
 * @brief Corrige la position de l'élément de la position \p pos
 * du tas \p AH en le comparant avec ses fils et en l'échangeant avec
 * le fils de la plus grande priorité si nécessaire.
 *
 * Procédure récursive.
 * 
 * @param[in] AH 
 * @param[in] pos L'indice de la valeur en mouvement vers le bas.
 */
static void updateArrayHeapDownwards(ArrayHeap* AH, int pos)
{
	if (AH == NULL) ShowMessage("updateArrayHeapDownwards : AH ne doit pas etre null", 1);
	if (pos < 0) ShowMessage("updateArrayHeapDownwards : pos ne doit pas etre inferieur a 0", 1);

	int indiceFG = 2 * pos + 1;
	int indiceFD = 2 * pos + 2;

	if (indiceFG < getAHActualSize(AH) && indiceFD < getAHActualSize(AH)) // Le noeud a 2 fils
	{
		if (AH->preceed(getAHNodeAt(AH, pos), getAHNodeAt(AH, indiceFG)) == 0 && AH->preceed(getAHNodeAt(AH, pos), getAHNodeAt(AH, indiceFD)) == 0) // Les 2 fils sont prioritaires
		{
			if (AH->preceed(getAHNodeAt(AH, indiceFG), getAHNodeAt(AH, indiceFD)) == 1) // Le le fg est prioritaire par rapport au fd, on échange avec le fg
			{
				void* temp = getAHNodeAt(AH, pos);
				setAHNodeAt(AH, pos, getAHNodeAt(AH, indiceFG));
				setAHNodeAt(AH, indiceFG, temp);
				updateArrayHeapDownwards(AH, indiceFG);
			}
			else // Sinon, avec le fd
			{
				void* temp = getAHNodeAt(AH, pos);
				setAHNodeAt(AH, pos, getAHNodeAt(AH, indiceFD));
				setAHNodeAt(AH, indiceFD, temp);
				updateArrayHeapDownwards(AH, indiceFD);
			}
		}
		else if (AH->preceed(AH->A[pos], AH->A[indiceFG]) == 0)
		{
			void* temp = getAHNodeAt(AH, pos);
			setAHNodeAt(AH, pos, getAHNodeAt(AH, indiceFG));
			setAHNodeAt(AH, indiceFG, temp);
			updateArrayHeapDownwards(AH, indiceFG);
		}
		else if (AH->preceed(AH->A[pos], AH->A[indiceFD]) == 0)
		{
			void* temp = getAHNodeAt(AH, pos);
			setAHNodeAt(AH, pos, getAHNodeAt(AH, indiceFD));
			setAHNodeAt(AH, indiceFD, temp);
			updateArrayHeapDownwards(AH, indiceFD);
		}
	}
	else if (indiceFG < getAHActualSize(AH))
	{
		if (AH->preceed(AH->A[pos], AH->A[indiceFG]) == 0) 
		{
			void* temp = getAHNodeAt(AH, pos);
			setAHNodeAt(AH, pos, getAHNodeAt(AH, indiceFG));
			setAHNodeAt(AH, indiceFG, temp);
			updateArrayHeapDownwards(AH, indiceFG);
			updateArrayHeapDownwards(AH, indiceFG);
		}
	}
}

ArrayHeap* ArrayToArrayHeap(void** A, int N, int (*preceed)(const void*, const void*), void (*viewHeapData)(const void*), void (*freeHeapData)(void*))
{
	if (A == NULL) ShowMessage("ArrayToArrayHeap : A ne doit pas etre NULL", 1);
	if (N < 0) ShowMessage("ArrayToArrayHeap : N ne doit pas etre inferieur a 0", 1);
	if (preceed == NULL) ShowMessage("ArrayToArrayHeap : preceed ne doit pas etre NULL", 1);
	if (viewHeapData == NULL) ShowMessage("ArrayToArrayHeap : viewHeapData ne doit pas etre NULL", 1);
	if (freeHeapData == NULL) ShowMessage("EArrayToArrayHeap : freeHeapData ne doit pas être NULL", 1);

	/* On fait confiance sur le fait que N ne dépasse pas de A[] */
	ArrayHeap* nouveauTas = (ArrayHeap*)malloc(sizeof(ArrayHeap)); // Création d'un nouveau tas
	if (nouveauTas == NULL) ShowMessage("ArrayToArrayHeap : echec de l'allocation du tas", 1);
	nouveauTas->A = A;
	nouveauTas->MAX = N;
	nouveauTas->N = N;
	nouveauTas->preceed = preceed;
	nouveauTas->viewHeapData = viewHeapData;
	nouveauTas->freeHeapData = freeHeapData;

	for (int i = N / 2 ; i >= 0 ; i--)
		updateArrayHeapDownwards(nouveauTas, i);

	return nouveauTas;
}

void viewArrayHeap(const ArrayHeap* AH)
{
	if (AH == NULL) ShowMessage("viewArrayHeap(const ArrayHeap* AH) : AH ne doit pas etre NULL", 1);

	printf("[ ");

	for (int i = 0 ; i < AH->N ; i++) 
	{
		AH->viewHeapData(AH->A[i]);
		printf(" ");
	}

	printf("]\n");
}

void freeArrayHeap(ArrayHeap* AH, int deletenode)
{
	if (AH == NULL) ShowMessage("freeArrayHeap : AH ne doit pas etre NULL", 1);
	if (deletenode != 0 && deletenode != 1) ShowMessage("freeArrayHeap : deletenode ne doit valoir que 0 ou 1", 1);

	if (deletenode == 1) // Si deletenode vaut 1, alors on supprime également les données du tas
	{
		for (int i = 0 ; i < getAHMaxSize(AH) ; i++)
			AH->freeHeapData(getAHNodeAt(AH, i));

		free(AH->A); // Libération du tableau du tas
	}

	free(AH); // Libération du tas
}

void* ArrayHeapExtractMin(ArrayHeap* AH)
{
	assert(getAHActualSize(AH) > 0);

	void* min = getAHNodeAt(AH, 0);
	setAHNodeAt(AH, 0, getAHNodeAt(AH, getAHActualSize(AH) - 1));
	setAHNodeAt(AH, getAHActualSize(AH) - 1, min);

	decreaseAHActualSize(AH);
	updateArrayHeapDownwards(AH, 0);

	return min;
}

/**********************************************************************************
 * COMPLETE BINARY TREE HEAP
 **********************************************************************************/

CBTHeap* newCBTHeap(int (*preceed)(const void*, const void*), void (*viewHeapData)(const void*), void (*freeHeapData)(void*))
{
	if (preceed == NULL) ShowMessage("newCBTHeap : preceed ne doit pas etre NULL", 1);
	if (viewHeapData == NULL) ShowMessage("newCBTHeap : viewHeapData ne doit pas etre NULL", 1);
	if (freeHeapData == NULL) ShowMessage("ERREUR newCBTHeap : freeHeapData ne doit pas etre NULL", 1);

	CBTHeap* nouveauTas = (CBTHeap*)malloc(sizeof(CBTHeap)); // Création d'un nouveau tas
	if (nouveauTas == NULL) ShowMessage("newCBTHeap : echec de l'allocation du tas", 1);
	nouveauTas->T = newCBTree(viewHeapData, freeHeapData);
	nouveauTas->preceed = preceed;
	nouveauTas->viewHeapData = viewHeapData;
	nouveauTas->freeHeapData = freeHeapData;
	return nouveauTas;
}

CBTree* getCBTree(const CBTHeap* H) 
{
	if (H == NULL) ShowMessage("getCBTree : H ne doit pas etre NULL", 1);
	return H->T;
}

/**
 * @brief Corrige la position du nœud à la position \p pos
 * de l'arbre raciné à \p node en le comparant avec son père
 * et en l'échangeant avec lui si nécessaire.
 * Le pointeur de fonction \p preceed est utilisé pour la comparaison.
 *
 * Procédure récursive. En descendant, on cherche le premier nœud
 * à corriger qui se trouve dans la position \p pos (de la même façon
 * que dans insertAfterLastTNode). En remontant, on corrige en échangeant
 * avec le père, si besoin.
 * 
 * @param[in] node 
 * @param[in] position 
 * @param[in] preceed 
 */
static void updateCBTHeapUpwards(TNode* node, int pos, int (*preceed)(const void*, const void*))
{
	if (node == NULL) ShowMessage("updateCBTHeapUpwards : node ne doit pas etre NULL\n", 1);
	if (pos <= 0) ShowMessage("updateCBTHeapUpwards : pos ne doit pas être inferieur ou egal a 0", 1);

	if (pos == 1) // 1re exception : c'est le fg
	{
		if (Left(node) != NULL) // Vérification de l'existence d'un fg
			if (preceed(getTNodeData(node), getTNodeData(Left(node))) == 0) // Si le père n'est pas à sa place par rapport au fg, alors on les échange
				CBTreeSwapData(node, Left(node));
	}
	else if (pos == 2) // 2e exception : c'est le fd
	{
		if (Right(node) != NULL) // Vérification de l'existence d'un fd
			if (preceed(getTNodeData(node), getTNodeData(Right(node))) == 0) // pareil pour le fd

				CBTreeSwapData(node, Right(node));
	}
	else // Cas général
	{
		int chemin = recherchePosition(&pos); // Appel de l'algorithme de recherche de position

		if (chemin == 1) // Si chemin = 1, alors l'appel récursif se fait dans le FG
			updateCBTHeapUpwards(Left(node), pos, preceed);
		else // Sinon, dans le FD
			updateCBTHeapUpwards(Right(node), pos, preceed);
	}
}

void CBTHeapInsert(CBTHeap* H, void* data)
{
	if (H == NULL) ShowMessage("CBTHeapInsert : H ne doit pas etre NULL", 1);
	if (data == NULL) ShowMessage("CBTHeapInsert : data ne doit pas etre NULL", 1);
	CBTreeInsert(getCBTree(H), data);

	for (int i = getCBTreeSize(getCBTree(H)) - 1 ; i > 0 ; i = recherchePositionPere(i))
		updateCBTHeapUpwards(Root(getCBTree(H)), i, H->preceed);
}

/**
 * @brief Corrige la position du nœud \p node en le comparant avec ses fils
 * et en l'échangeant avec le fils de la plus grande priorité si nécessaire.
 * Le pointeur de fonction \p preceed est utilisé pour la comparaison.
 *
 * Procédure récursive.
 *
 * NB: Le sous-arbre avec racine \p node ne peut pas être vide.
 * 
 * @param[in] node 
 * @param[in] preceed 
 */
static void updateCBTHeapDownwards(TNode* node, int (*preceed)(const void*, const void*))
{
	assert(node);
	assert(preceed);

	if (Left(node) != NULL && Right(node) != NULL)
	{
		// Le noeud n'est pas à sa place par rapport à ses 2 fils
		if (preceed(getTNodeData(node), getTNodeData(Left(node))) == 0 && preceed(getTNodeData(node), getTNodeData(Right(node))) == 0) 
		{ 
			// Si le fg est prioritaire au fg, alors on échange le père avec et on lance l'appel récursif
			if (preceed(getTNodeData(Left(node)), getTNodeData(Right(node))) == 1)
			{
				CBTreeSwapData(node, Left(node));
				updateCBTHeapDownwards(Left(node), preceed);
			}
			else
			// Sinon, ce sera avec le fg
			{
				CBTreeSwapData(node, Right(node));
				updateCBTHeapDownwards(Right(node), preceed);
			}
		}
		else if (preceed(getTNodeData(node), getTNodeData(Left(node))) == 0)
		{
			CBTreeSwapData(node, Left(node));
			updateCBTHeapDownwards(Left(node), preceed);
		}
		else if (preceed(getTNodeData(node), getTNodeData(Right(node))) == 0)
		{
			CBTreeSwapData(node, Right(node));
			updateCBTHeapDownwards(Right(node), preceed);
		}
	}
	else if (Left(node) != NULL)
	{
		if (preceed(getTNodeData(node), getTNodeData(Left(node))) == 0)
		{
			CBTreeSwapData(node, Left(node));
			updateCBTHeapDownwards(Left(node), preceed);
		}
	}
	// Impossible que le fg soit NULL et pas le fd car c'est un arbre binaire complet
}

void* CBTHeapExtractMin(CBTHeap* H)
{
	assert(Root(getCBTree(H)));

	// Sauvegarde du min
	void* min = getTNodeData(Root(getCBTree(H)));
	CBTreeSwapData(Root(getCBTree(H)), CBTreeGetLast(getCBTree(H)));
	CBTreeRemove(getCBTree(H));

	if (getCBTreeSize(getCBTree(H)) > 0) 
		updateCBTHeapDownwards(Root(getCBTree(H)), H->preceed);

	return min;
}

void viewCBTHeap(const CBTHeap* H) // pas fini
{
	viewCBTree(getCBTree(H), 0);
}

void freeCBTHeap(CBTHeap* H, int deletenode)
{
	if (H == NULL) ShowMessage("freeCBTHeap : H ne doit pas etre NULL", 1);
	if (deletenode != 0 && deletenode != 1) ShowMessage("freeCBTHeap : deletenode ne doit valoir que 0 ou 1", 1);

	freeCBTree(getCBTree(H), deletenode);
	free(H);
}

/*
static void updateArrayHeapDownwards(ArrayHeap* AH, int pos)
{
	if (AH == NULL) ShowMessage("updateArrayHeapDownwards : AH ne doit pas etre null", 1);
	if (pos < 0) ShowMessage("updateArrayHeapDownwards : pos ne doit pas etre inferieur a 0", 1);

	int indiceFG = 2 * pos + 1;
	int indiceFD = 2 * pos + 2;

	if (indiceFG < getAHActualSize(AH) && indiceFD < getAHActualSize(AH)) // Le noeud a 2 fils
	{
		if (AH->preceed(getAHNodeAt(pos), getAHNodeAt(indiceFG)) == 0 && AH->preceed(AH->A[pos], AH->A[indiceFD]) == 0) // Les 2 fils sont prioritaires
		{
			if (AH->preceed(AH->A[indiceFG], AH->A[indiceFD]) == 1) // Le le fg est prioritaire par rapport au fd, on échange avec le fg
			{
				void* temp = AH->A[pos];
				AH->A[pos] = AH->A[indiceFG];
				AH->A[indiceFG] = temp;
				updateArrayHeapDownwards(AH, indiceFG);
			}
			else // Sinon, avec le fd
			{
				void* temp = AH->A[pos];
				AH->A[pos] = AH->A[indiceFD];
				AH->A[indiceFD] = temp;
				updateArrayHeapDownwards(AH, indiceFD);
			}
		}
		else if (AH->preceed(AH->A[pos], AH->A[indiceFG]) == 0)
		{
			void* temp = AH->A[pos];
			AH->A[pos] = AH->A[indiceFG];
			AH->A[indiceFG] = temp;
			updateArrayHeapDownwards(AH, indiceFG);
		}
		else if (AH->preceed(AH->A[pos], AH->A[indiceFD]) == 0)
		{
			void* temp = AH->A[pos];
			AH->A[pos] = AH->A[indiceFD];
			AH->A[indiceFD] = temp;
			updateArrayHeapDownwards(AH, indiceFD);
		}
	}
	else if (indiceFG < getAHActualSize(AH))
	{
		if (AH->preceed(AH->A[pos], AH->A[indiceFG]) == 0) 
		{
			void* temp = AH->A[pos];
			AH->A[pos] = AH->A[indiceFG];
			AH->A[indiceFG] = temp;
			updateArrayHeapDownwards(AH, indiceFG);
		}
	}
}
*/