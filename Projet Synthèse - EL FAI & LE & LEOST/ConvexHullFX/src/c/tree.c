#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../include/tree.h"
#include "../include/util.h"

#include <math.h>

/********************************************************************
 * TNode
 ********************************************************************/

TNode* newTNode(void* data)
{
	if (data == NULL) ShowMessage("newTNode : data ne doit pas etre NULL", 1);

	TNode* nouveauNoeud = (TNode*)malloc(sizeof(TNode));
	if (nouveauNoeud == NULL) ShowMessage("newTNode : echec de l'allocation du noeud", 1);
	nouveauNoeud->data = data;
	nouveauNoeud->left = NULL;
	nouveauNoeud->right = NULL;
	return nouveauNoeud;
}

void* getTNodeData(const TNode* node)
{
	if (node == NULL) ShowMessage("getTNodeData : node ne doit pas etre NULL", 1);
	return node->data;
}

TNode* Left(const TNode* node)
{
	if (node == NULL) ShowMessage("Left : node ne doit pas etre NULL", 1);
	return node->left;
}

TNode* Right(const TNode* node)
{
	if (node == NULL) ShowMessage("Right : node ne doit pas etre NULL", 1);
	return node->right;
}

void setTNodeData(TNode* node, void* newData)
{
	if (node == NULL) ShowMessage("setTNodeData : node ne doit pas etre NULL", 1);
	if (newData == NULL) ShowMessage("setTNodeData : newData ne doit pas etre NULL", 1);

	node->data = newData;
}

void setLeft(TNode* node, TNode* newLeft)
{
	if (node == NULL) ShowMessage("setLeft : node ne doit pas etre NULL", 1);
	// newLeft peut être NULL

	node->left = newLeft;
}

void setRight(TNode* node, TNode* newRight)
{
	if (node == NULL) ShowMessage("setRight : node ne doit pas etre NULL", 1);
	// newRight peut être NULL

	node->right = newRight;
}

/********************************************************************
 * Complete Binary Tree
 ********************************************************************/

CBTree* newCBTree(void (*viewData)(const void*), void (*freeData)(void*))
{
	if (viewData == NULL) ShowMessage("newCBTree : viewData ne doit pas etre NULL", 1);
	if (freeData == NULL) ShowMessage("newCBTree : freeData ne doit pas etre NULL", 1);

	CBTree* nouvelArbre = (CBTree*)malloc(sizeof(CBTree));	
	if (nouvelArbre == NULL) ShowMessage("newCBTree : echec de l'allocation de l'arbre", 1);
	nouvelArbre->root = NULL;
	nouvelArbre->numelm = 0;
	nouvelArbre->viewData = viewData;
	nouvelArbre->freeData = freeData;
	return nouvelArbre;
}

int treeIsEmpty(CBTree* T)
{
	if (T == NULL) ShowMessage("treeIsEmpty : T ne doit pas etre NULL", 1);

	if (T->numelm == 0) return 1;
	else return 0;
}

int getCBTreeSize(const CBTree* T)
{
	if (T == NULL) ShowMessage("getCBTreeSize : T ne doit pas etre NULL", 1);
	return T->numelm;
}

TNode* Root(const CBTree* T)
{
	if (T == NULL) ShowMessage("Root : T ne doit pas etre NULL", 1);
	return T->root;
}

void increaseCBTreeSize(CBTree* T)
{
	if (T == NULL) ShowMessage("increaseCBTreeSize : T ne doit pas etre NULL", 1);
	T->numelm = T->numelm + 1;
}

void decreaseCBTreeSize(CBTree* T)
{
	if (T == NULL) ShowMessage("decreaseCBTreeSize : T ne doit pas etre NULL", 1);
	else if (T->numelm == 0) ShowMessage("decreaseCBTreeSize : T est deja vide", 1);
	T->numelm = T->numelm - 1;
}

void resetCBTreeSize(CBTree* T)
{
	if (T == NULL) ShowMessage("resetCBTreeSize : T ne doit pas etre NULL", 1);
	T->numelm = 0;
}

void setRoot(CBTree* T, TNode* newRoot)
{
	if (T == NULL) ShowMessage("setRoot : T ne doit pas etre NULL", 1);
	// newRoot peut être NULL
	T->root = newRoot;
}

/**
 * @brief Libère récursivement le sous-arbre raciné au nœud \p node.
 * Dans le cas où le pointeur de fonction \p freeData n'est pas NULL,
 * la mémoire de la donnée du nœud actuel est aussi libérée.
 * NB : procédure récursive.
 * 
 * @param[in] node 
 * @param[in] freeData 
 */
static void freeTNode(TNode* node, void (*freeData)(void*))
{
	if (node == NULL) {} // Exception
	else // Cas Général
	{
		freeTNode(Left(node), freeData); // Appel récursif pour le fils gauche de node
		freeTNode(Right(node), freeData); // Appel récursif pour le fils droit de node

		if (freeData != NULL) // Si freeData != NULL, alors on libère aussi la donnée de node
			freeData(getTNodeData(node)); // Libération de la donnée de node
		
		free(node); // Libération de node
	}
}

/**
 * NB : Utilisez la procédure récursive freeTNode.
 * Vous devez initialiser le paramètre freeData
 * par rapport à la valeur de deleteData.
 */
void freeCBTree(CBTree* T, int deleteData)
{
	if (T == NULL) ShowMessage("freeCBTree : T ne doit pas etre NULL", 1);
	else if (deleteData != 0 && deleteData != 1) ShowMessage("freeCBTree : deleteData ne doit valoir que 0 ou 1", 1);

	if (deleteData == 0) // Si deleteData = 0, alors on libère l'arbre sans libérer les données des noeuds
		freeTNode(Root(T), NULL);
	else // deleteData == 1
		freeTNode(Root(T), T->freeData); // Sinon, on libère aussi bien les noeuds que leurs donnéees

	free(T); // Libération de T
}

/**
 * @brief Affiche les éléments de l'arbre raciné au nœud \p node
 * en réalisant un parcours préfixé.
 * Les données de chaque nœud sont afficher en utilisant le
 * pointer de fonction \p viewData.
 * 
 * @param[in] node 
 * @param[in] viewData 
 */
static void preorder(TNode *node, void (*viewData)(const void*))
{
	if (viewData == NULL) ShowMessage("preorder : viewData ne doit pas etre NULL", 1);

	if (node == NULL) {} // Exception
	else // Cas général
	{
		viewData(getTNodeData(node));
		printf(" ");
		preorder(Left(node), viewData); // Appel récursif pour le fils gauche
		preorder(Right(node), viewData); // Appel récursif pour le fils droit
	}
}

/**
 * @brief Affiche les éléments de l'arbre raciné au nœud \p node
 * en réalisant un parcours infixé.
 * Les données de chaque nœud sont afficher en utilisant le
 * pointer de fonction \p viewData.
 * 
 * @param[in] node 
 * @param[in] viewData 
 */
static void inorder(TNode *node, void (*viewData)(const void*))
{
	if (viewData == NULL) ShowMessage("inorder : viewData ne doit pas etre NULL", 1);

	if (node == NULL) {} // Exception
	else // Cas général
	{
		inorder(Left(node), viewData); // Appel récursif pour le fils gauche
		(viewData)(getTNodeData(node));
		printf(" ");
		inorder(Right(node), viewData); // Appel récursif pour le fils droit
	}
}

/**
 * @brief Affiche les éléments de l'arbre raciné au nœud \p node
 * en réalisant un parcours postfixé.
 * Les données de chaque nœud sont afficher en utilisant le
 * pointer de fonction \p viewData.
 * 
 * @param[in] node 
 * @param[in] viewData 
 */
static void postorder(TNode *node, void (*viewData)(const void*))
{
	if (viewData == NULL) ShowMessage("postorder : viewData ne doit pas etre NULL", 1);

	if (node == NULL) {} // Exception
	else // Cas général
	{
		postorder(Left(node), viewData); // Appel récursif pour le fils gauche
		postorder(Right(node), viewData); // Appel récursif pour le fils droit
		(viewData)(getTNodeData(node));
		printf(" ");
	}
}

/**
 * NB : Utilisez les procédures récursives preorder, inorder et postorder.
 * Rappel : order = 0 (preorder), 1 (postorder), 2 (inorder)
 */
void viewCBTree(const CBTree* T, int order)
{
	if (T == NULL) ShowMessage("viewCBTree : T ne doit pas etre NULL", 1);
	if (order != 0 && order != 1 && order != 2) ShowMessage("viewCBTree : order ne doit valoir que 0, 1 ou 2", 1);

	printf("Nombre d'éléments de l'arbre : %d\n", getCBTreeSize(T));
	printf("Elements de l'arbre : ");

	switch (order)
	{
		case 0 : 
			preorder(Root(T), T->viewData);
			break;
		case 1 :
			inorder(Root(T), T->viewData);
			break;
		case 2 :
			postorder(Root(T), T->viewData);
			break;
		// pas besoin de default car on vérifie avant que order soit valide
	}

	printf("\n");
}

/**
 * @brief Insère récursivement un nouveau nœud de donnée \p data
 * dans l'arbre raciné au nœud \p node.
 * La position (par rapport à la racine \p node) où le nouveau nœud
 * va être insérer est indiquée par le paramètre \p position
 * (voir la figure ci-dessous pour la définition de la position).
 *  
 *          0
 *       /     \
 *      1       2
 *     / \     / \
 *    3   4   5   6
 *   / \
 *  7  ...
 * 
 * @param[in] node La racine de l'arbre actuel.
 * @param[in] position La position du nouveau élément
 * 						par rapport à la racine \p node.
 * @param[in] data La donnée à insérer.
 * @return TNode* Le nœud \p node mis à jour.
 */
static TNode* insertAfterLastTNode(TNode* node, int position, void* data)
{
	if (node == NULL) ShowMessage("insertAfterLastTNode : node ne doit pas etre NULL", 1);
	if (position <= 0) ShowMessage("insertAfterLastTNode : position ne doit pas etre infrrieur ou egal a 0", 1);
   
	if (position == 1) // 1re exception : c'est le fils gauche
	{
		TNode* nouveauNoeud = newTNode(data);
		setLeft(node, nouveauNoeud);
		return node;
	}
	else if (position == 2) // 2e exception : c'est le fils droit
	{
		TNode* nouveauNoeud = newTNode(data);
		setRight(node, nouveauNoeud);
		return node;
	}
	else // Cas général
	{
		int chemin = (recherchePosition(&position)); // Appel de la l'algorithme de recherche de position

		if (chemin == 1) // Si chemin = 1, l'appel récursif se fait dans le FG
		{
			setLeft(node, insertAfterLastTNode(Left(node), position, data)); 
			return node;
		}
		else // Sinon, dans le FD
		{
			setRight(node, insertAfterLastTNode(Right(node),position, data));
			return node;
		}
	}
}

/**
 * NB : Utilisez la procédure récursive insertAfterLastTNode
 * afin de lancer l'insertion.
 */
void CBTreeInsert(CBTree* T, void* data)
{
	if (T == NULL) ShowMessage("CBTreeInsert : T ne doit pas etre NULL\n", 1);
	if (data == NULL) ShowMessage("CBTreeInsert : data ne doit pas etre NULL\n", 1);

	if (treeIsEmpty(T) == 1) // Si l'arbre est vide, alors on rajoute un nouveau noued à la racine
	{
		setRoot(T, newTNode(data));
		increaseCBTreeSize(T);
	}
	else // Sinon, on rajoute le noeud à la suite de l'arbre
	{
		setRoot(T, insertAfterLastTNode(Root(T), getCBTreeSize(T), data));
		increaseCBTreeSize(T);
	}
}

/**
 * @brief Supprime récursivement le dernier nœud
 * de l'arbre raciné au nœud \p node.
 * La position (par rapport à la racine \p node) du nœud à supprimer
 * est indiquée par le paramètre \p position
 * (voir la figure ci-dessous pour la définition de la position).
 * La mémoire du dernier nœud est libérée mais pas la mémoire de sa donnée.
 *  
 *          0
 *       /     \
 *      1       2
 *     / \     / \
 *    3   4   5   6
 *   / \
 *  7  ...
 * 
 * @param[in] node La racine de l'arbre actuel.
 * @param[in] position La position de l'élément à supprimer
 *                         par rapport à la racine \p node.
 * @param[out] data La donnée du nœud supprimé (sortie).
 * @return TNode* Le nœud \p node mis à jour.
 */
static TNode* removeLastTNode(TNode* node, int position, void** data)
{
	if (node == NULL) ShowMessage("removeLastTNode : node ne doit pas etre NULL", 1);
	if (position <= 0) ShowMessage("removeLastTNode : position ne doit pas etre inferieur ou egal a 0", 1);

	if (position == 1) // 1re exception : c'est le fils gauche
	{
		*data = getTNodeData(Left(node)); // 1) sauvegarde de la donnée
		free(Left(node)); // 2) libération du noeud
		setLeft(node, NULL); // 3) remplacement du fils gauche de node par NULL

		return node;
	}
	else if (position == 2) // 2e exception : c'est le fils droit
	{
		*data = getTNodeData(Right(node)); // 1) sauvegarde de la donnée
		free(Right(node)); // 2) libération du noeud
		setRight(node, NULL); // 3) remplacement du fils gauche de node par NULL

		return node;
	}
	else // Cas général
	{
		int chemin = recherchePosition(&position); // Appel de l'algorithme de recherche de position

		if (chemin == 1) // Si chemin = 1, alors l'appel récursif se fait dans le FG
		{
			setLeft(node, removeLastTNode(Left(node), position, data));

			return node;
		}
		else // Sinon, dans le FD
		{
			setRight(node, removeLastTNode(Right(node), position, data)); // Soit dans le fils droit

			return node;
		}
	}
}

/**
 * NB : Utilisez la procédure récursive removeLastTNode
 * afin de lancer la suppression.
 */
void* CBTreeRemove(CBTree* T)
{
	assert(Root(T));
	if (treeIsEmpty(T) == 1) ShowMessage("CBTreeRemove : T ne doit pas etre vide\n", 1);
	
	void* donnee; // Sauvegarde de la donnée du noeud supprimé

	if (getCBTreeSize(T) == 1) // Si l'arbre ne contient qu'un seul noeud (la racine), alors on le libère et le remplace par NULL
	{
		donnee = getTNodeData(Root(T));
		free(Root(T));
		setRoot(T, NULL);
		decreaseCBTreeSize(T);
	}
	else // Sinon, on appelle removeLastTnode
	{
		setRoot(T, removeLastTNode(Root(T), getCBTreeSize(T) - 1, &donnee));
		decreaseCBTreeSize(T);
	}

	return donnee;
}

/**
 * @brief Restitue récursivement le dernier nœud
 * de l'arbre raciné au nœud \p node.
 * La position (par rapport à la racine \p node) de ce dernier nœud
 * est indiquée par le paramètre \p position
 * (voir la figure ci-dessous pour la définition de la position).
 *  
 *          0
 *       /     \
 *      1       2
 *     / \     / \
 *    3   4   5   6
 *   / \
 *  7  ...
 * 
 * @param node La racine de l'arbre actuel.
 * @param position La position du dernier nœud par rapport à la racine \p node.
 * @return TNode* Le dernier nœud de l'arbre.
 */
static TNode* getLastTNode(TNode* node, int position)
{
	if (node == NULL) ShowMessage("getLastTNode : node ne doit pas etre NULL", 1);
	if (position <= 0) ShowMessage("getLastTNode : position ne doit pas etre inferieur ou egal a 0", 1);

	if (position == 1) // 1re exception : c'est le fils gauche
		return Left(node);
	else if (position == 2) // 2e exception : c'est le fils droit
		return Right(node);
	else // Cas général
	{
		int chemin = recherchePosition(&position); // Appel de l'algorithme de recherche de position

		if (chemin == 1) // Si chemin = 1, alors l'appel récursif se fait dans le FG
			return getLastTNode(Left(node), position);
		else // Sinon, dans le FD
			return getLastTNode(Right(node), position);
	}
}

/**
 * NB : Utilisez la procédure récursive getLastTNode
 * afin de lancer la recherche.
 */
TNode* CBTreeGetLast(CBTree* T)
{
	if (T == NULL) ShowMessage("CBTreeGetLast : T ne doit pas etre NULL", 1);
	else if (treeIsEmpty(T) == 1) ShowMessage("CBTreeGetLast : T ne doit pas etre vide", 1);

	if (getCBTreeSize(T) == 1) // Si l'arbre ne contient qu'un seul noeud, alors on renvoie la racine
		return Root(T);
	else // Sinon, on appelle getLastTNode
		return getLastTNode(Root(T), getCBTreeSize(T) - 1);
}

void CBTreeSwapData(TNode* node1, TNode* node2)
{
	if (node1 == NULL) ShowMessage("CBTreeSwapData : node1 ne doit pas etre NULL", 1);
	if (node2 == NULL) ShowMessage("CBTreeSwapData : node2 ne doit pas etre NULL", 1);

	void* temp = getTNodeData(node1);
	setTNodeData(node1, getTNodeData(node2));
	setTNodeData(node2, temp);
}