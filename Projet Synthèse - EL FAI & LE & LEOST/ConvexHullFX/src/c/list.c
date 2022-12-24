#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/list.h"
#include "../include/util.h"

/********************************************************************
 * LNode
 ********************************************************************/

LNode* newLNode(void* data) 
{	
	if (data == NULL) ShowMessage("newLNode : data ne doit pas etre NULL", 1);
	
	LNode* nouveauNoeud = (LNode*)malloc(sizeof(LNode));
	if (nouveauNoeud == NULL) ShowMessage("newLNode : echec de l'allocation du noeud", 1);
	nouveauNoeud->data = data;
	nouveauNoeud->succ = NULL;
	nouveauNoeud->pred = NULL;
	return nouveauNoeud;
}

void* getLNodeData(const LNode* node)
{
	if (node == NULL) ShowMessage("getLNodeData : node ne doit pas etre NULL", 1);
	return node->data;
}

LNode* Successor(const LNode* node)
{
	if (node == NULL) ShowMessage("Successor : node ne doit pas etre NULL", 1);
	return node->succ;
}

LNode* Predecessor(const LNode* node)
{
	if (node == NULL) ShowMessage("Predecessor : node ne doit pas etre NULL", 1);
	return node->pred;
}

void setLNodeData(LNode* node, void* newData)
{
	if (node == NULL) ShowMessage("setLNodeData(LNode* node, void* newData) : node ne doit pas etre NULL", 1);
	if (newData == NULL) ShowMessage("setLNodeData(LNode* node, void* newData) : newData ne doit pas etre NULL", 1);
	node->data = newData;
}

void setSuccessor(LNode* node, LNode* newSucc)
{
	if (node == NULL) ShowMessage("setSuccessor : node ne doit pas etre NULL", 1);
	// newSucc peut etre NULL
	node->succ = newSucc;
}

void setPredecessor(LNode* node, LNode* newPred)
{
	if (node == NULL) ShowMessage("setPredecessor : node ne doit pas etre NULL", 1);
	// newPred peut etre NULL
	node->pred = newPred;
}

/********************************************************************
 * List
 ********************************************************************/

List* newList(void (*viewData)(const void*), void (*freeData)(void*))
{
	if (viewData == NULL) ShowMessage("newList : viewData ne doit pas etre NULL", 1);
	if (freeData == NULL) ShowMessage("newList : freeData ne doit pas etre NULL", 1);

	List* nouvelleListe = (List*)malloc(sizeof(List));
	if (nouvelleListe == NULL) ShowMessage("newList : echec de l'allocation de la liste", 1);
	nouvelleListe->viewData = viewData;
	nouvelleListe->freeData = freeData;
	nouvelleListe->head = NULL;
	nouvelleListe->tail = NULL;
	nouvelleListe->numelm = 0;
	return nouvelleListe;
}

int listIsEmpty(List* L)
{
	if (L == NULL) ShowMessage("listIsEmpty : L ne doit pas etre NULL", 1);

	if (L->numelm == 0) return 1;
	else return 0;
}

int getListSize(const List* L)
{
	if (L == NULL) ShowMessage("getListSize : L ne doit pas etre NULL", 1);
	return L->numelm;
}

LNode* Head(const List* L)
{
	if (L == NULL) ShowMessage("Head : L ne doit pas etre NULL", 1);
	return L->head;
}

LNode* Tail(const List* L)
{
	if (L == NULL) ShowMessage("Tail : L ne doit pas etre NULL", 1);
	return L->tail;
}

void increaseListSize(List* L)
{
	if (L == NULL) ShowMessage("increaseListSize : L ne doit pas etre NULL", 1);
	L->numelm = L->numelm + 1;
}

void decreaseListSize(List* L)
{
	if (L == NULL) ShowMessage("decreaseListSize : L ne doit pas etre NULL", 1);
	if (listIsEmpty(L) == 1) ShowMessage("decreaseListSize : L est deja vide", 1);
	L->numelm = L->numelm - 1;
}

void setListSize(List* L, int newSize)
{
	if (L == NULL) ShowMessage("setListSize : L ne doit pas etre NULL", 1);
	if (newSize < 0) ShowMessage("setListSize : le nombre d'elements ne doit pas etre inferieur à 0", 1);
	L->numelm = newSize;
}

void resetListSize(List* L)
{
	if (L == NULL) ShowMessage("resetListSize : L ne doit pas etre NULL", 1);
	L->numelm = 0;
}

void setHead(List* L, LNode* newHead)
{
	if (L == NULL) ShowMessage("setHead : L ne doit pas etre NULL", 1);
	// newHead peut etre NULL
	L->head = newHead;
}
void setTail(List* L, LNode* newTail)
{
	if (L == NULL) ShowMessage("setTail : L ne doit pas etre NULL", 1);
	// newTail peut etre NULL
	L->tail = newTail;
}

void freeList(List* L, int deleteData)
{
	assert(deleteData == 0 || deleteData == 1);
	if (L == NULL) ShowMessage("freeList : L ne doit pas etre NULL", 1);

	LNode* i = Head(L);

	while (i != NULL)
	{
		LNode* aSupprimer = i;
		i = Successor(i);

		if(deleteData == 1)
		{
			(L->freeData)(getLNodeData(aSupprimer));
		}
		free(aSupprimer);
	}

	free(L);
}

void viewList(const List* L)
{
	if (L == NULL) ShowMessage("viewList : L ne doit pas etre NULL", 1);

	LNode* i = Head(L);
	printf("( ");
	
	while (i != NULL)
	{
		L->viewData(getLNodeData(i));
		printf(" ");
		i = Successor(i);
	}

	printf(")\n");
}

void listInsertFirst(List* L, void* data)
{
	if (L == NULL) ShowMessage("listInsertFirst : L ne doit pas etre NULL", 1);
	if (data == NULL) ShowMessage("listInsertFirst : data ne doit pas etre NULL", 1);

	LNode* nouveauNoeud = newLNode(data);

	if (listIsEmpty(L) == 1) // Si la liste est vide, alors le nouveau noeud devient sa tête et sa queue
	{
		setHead(L, nouveauNoeud);
		setTail(L, nouveauNoeud);
	}
	else // Sinon, il faut minutieusement l'insérer au début
	{
		setSuccessor(nouveauNoeud, Head(L));
		setPredecessor(Head(L), nouveauNoeud);
		setHead(L, nouveauNoeud);
	}

	increaseListSize(L); 
}

void listInsertLast(List* L, void* data)
{
	if (L == NULL) ShowMessage("listInsertLast : L ne doit pas etre NULL", 1);
	if (data == NULL) ShowMessage("listInsertLast : data ne doit pas etre NULL", 1);

	LNode* nouveauNoeud = newLNode(data);

	if (listIsEmpty(L) == 1) // Si la liste est vide, alors le nouveau noeud devient sa tête et sa queue
	{
		setHead(L, nouveauNoeud);
		setTail(L, nouveauNoeud);
	}
	else // Sinon, il faut minutieusement l'insérer à la fin
	{
		setPredecessor(nouveauNoeud, Tail(L));
		setSuccessor(Tail(L), nouveauNoeud);
		setTail(L, nouveauNoeud);
	}

	increaseListSize(L); 
}

void listInsertAfter(List* L, void* data, LNode* ptrelm)
{
	if (L == NULL) ShowMessage("listInsertAfter : L ne doit pas etre NULL", 1);
	if (data == NULL) ShowMessage("listInsertAfter : data ne doit pas etre NULL", 1);
	if (ptrelm == NULL) ShowMessage("listInsertAfter : ptrelm ne doit pas etre NULL", 1);

	LNode* nouveauNoeud = newLNode(data);
	setPredecessor(nouveauNoeud, ptrelm);
	setSuccessor(nouveauNoeud, Successor(ptrelm));
	setSuccessor(ptrelm, nouveauNoeud);
	increaseListSize(L);

	if (Successor(nouveauNoeud) == NULL) // Si l'élément est inséré à la fin de la liste, alors il faut également modifier la queue de la liste
		setTail(L, nouveauNoeud);
}

void* listRemoveFirst(List* L)
{
	assert(Head(L));
	if (listIsEmpty(L) == 1) ShowMessage("listRemoveFirst : L ne doit pas etre vide", 1);
	
	LNode* aSupprimer = Head(L);
	void* donnee = getLNodeData(aSupprimer);

	if (getListSize(L) == 1) // Cas où la liste ne contient qu'un seul élément
	{
		setHead(L, NULL);
		setTail(L, NULL); 
		decreaseListSize(L);
	}
	else // Cas où la liste contient plusieurs elements
	{
		setHead(L, Successor(aSupprimer));
		setPredecessor(Successor(aSupprimer), NULL);
		decreaseListSize(L);
	}

	free(aSupprimer);

	return donnee;
}

void* listRemoveLast(List* L)
{
	assert(Head(L));
	if (listIsEmpty(L) == 1) ShowMessage("listRemoveLast : L ne doit pas etre vide", 1);
	
	LNode* aSupprimer = Tail(L);
	void* donnee = getLNodeData(aSupprimer);

	if (getListSize(L) == 1) // Cas où la liste ne contient qu'un seul élément
	{
		setHead(L, NULL);
		setTail(L, NULL); 
		decreaseListSize(L);
	}
	else // Cas où la liste contient plusieurs elements
	{
		setTail(L, Predecessor(aSupprimer));
		setSuccessor(Predecessor(aSupprimer), NULL);
		decreaseListSize(L);
	}

	free(aSupprimer);

	return donnee;
}

void* listRemoveNode(List* L, LNode* node)
{
	assert(Head(L) && Tail(L));
	if (listIsEmpty(L) == 1) ShowMessage("listRemoveNode : L ne doit pas etre vide", 1);
	if (node == NULL) ShowMessage("listRemoveNode : node ne doit pas etre NULL", 1);

	/* On suppose que node fait bien partie de L */

	void* donnee;

	if (getListSize(L) == 1) // Cas où la liste ne contient que node
		donnee = listRemoveFirst(L);
	else if (Predecessor(node) == NULL) // Cas où node est la tête de la liste
		donnee = listRemoveFirst(L);
	else if (Successor(node) == NULL) // Cas où node est la queue de la liste
		donnee = listRemoveLast(L);
	else // Cas où la liste contient au moins 3 elements et que node n'est pas la tête ni la queue
	{
		setPredecessor(Successor(node), Predecessor(node)); // Remplacement du prédecesseur du successeur de node
		setSuccessor(Predecessor(node), Successor(node)); // Remplacement du successeur du prédecesseur de node
		decreaseListSize(L);
		donnee = getLNodeData(node);
		free(node); // Libération de node
	}

	return donnee;	
}

List* listConcatenate(List* L1, List* L2)
{
	if (L1 == NULL) ShowMessage("listConcatenate : L1 ne doit pas etre NULL", 1);
	if (L2 == NULL) ShowMessage("listConcatenate : L2 ne doit pas etre NULL", 1);

	List* nouvelleListe;

	if (listIsEmpty(L1) == 1) // Cas où L1 est vide (pas NULL)
	{
		nouvelleListe = L2; 
		freeList(L1, 0);
	}
	else
	{
		setSuccessor(Tail(L1), Head(L2)); // 1) le successeur de la queue de L1 devient la tête de L2
		setTail(L1, Tail(L2)); // 2) la queue de L2 devient celle de L2
		setListSize(L1, getListSize(L1) + getListSize(L2)); // 3) on additionne au nombre d'elements de L1 celui de L2

		setHead(L2, NULL);
		setTail(L2, NULL);
		setListSize(L2, 0);

		freeList(L2, 0); // 4) on libère L2 (uniquement la liste, pas les noeuds qu'elle contient)
		nouvelleListe = L1;
	}

	return nouvelleListe;
}


