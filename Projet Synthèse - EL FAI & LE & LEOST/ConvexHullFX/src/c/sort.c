#include <stdlib.h>
#include "../include/tree.h"
#include "../include/heap.h"
#include "../include/sort.h"

#include "../include/util.h"

void ArrayHeapSort(void** A, int N,
					int (*preceed)(const void*, const void*),
					void (*viewHeapData)(const void*),
					void (*freeHeapData)(void*))
{
	if (A == NULL) ShowMessage("ArrayHeapSort : A ne doit pas etre NULL", 1);
	if (N <= 0) ShowMessage("ArrayHeapSort : N ne doit pas etre inferieur ou egal a 0", 1);
	if (preceed == NULL) ShowMessage("ArrayHeapSort : preceed ne doit pas etre NULL", 1);
	if (viewHeapData == NULL) ShowMessage("ArrayHeapSort : viewHeapData ne doit pas etre NULL", 1);
	if (freeHeapData == NULL) ShowMessage("ArrayHeapSort : freeHeapData ne doit pas etre NULL", 1);

	/* On fait confiance sur le fait que N ne dépasse pas de A[] */

	ArrayHeap* AH = ArrayToArrayHeap(A, N, preceed, viewArrayHeap, freeHeapData);

	for (int i = N - 1 ; i >= 0 ; i--)
		ArrayHeapExtractMin(AH);

	for (int i = 0 ; i < N / 2 ; i++)
	{
		void* temp = A[i];
		A[i] = A[N - 1 - i];
		A[N - 1 - i] = temp;
	}

	freeArrayHeap(AH, 0);
}

void CBTHeapSort(void** A, int N,
					int (*preceed)(const void*, const void*),
					void (*viewHeapData)(const void*),
					void (*freeHeapData)(void*))
{
	if (A == NULL) ShowMessage("CBTHeapSort : A ne doit pas etre NULL", 1);
	if (N <= 0) ShowMessage("CBTHeapSort : N ne doit pas etre inferieur ou egal a 0", 1);
	if (preceed == NULL) ShowMessage("CBTHeapSort : preceed ne doit pas etre NULL", 1);
	if (viewHeapData == NULL) ShowMessage("CBTHeapSort : viewHeapData ne doit pas etre NULL", 1);
	if (freeHeapData == NULL) ShowMessage("ERREUR CBTHeapSort: freeHeapData ne doit pas etre NULL", 1);

	/* On fait confiance sur le fait que N ne dépasse pas de A[] */

	CBTHeap* H = newCBTHeap(preceed, viewHeapData, freeHeapData);

	for (int i = 0 ; i < N ; i++)
		CBTHeapInsert(H, A[i]);

	for (int i = 0 ; i < N ; i++)
		A[i] = CBTHeapExtractMin(H);

	freeCBTHeap(H, 0);
}

void SelectionSort(void** A, int N, int (*preceed)(const void*, const void*))
{
	if (A == NULL) ShowMessage("SelectionSort : A ne doit pas etre NULL", 1);
	if (N <= 0) ShowMessage("SelectionSort : N ne doit pas etre inferieur ou egal a 0", 1);
	if (preceed == NULL) ShowMessage("SelectionSort: preceed ne doit pas etre NULL", 1);

	int PosMin;
	void* temp;

	for (int i = 0 ; i < N - 1 ; i++)
	{
		PosMin = i;

		for (int j = i + 1 ; j < N ; j++) // Recherche du min
			if (preceed(A[j], A[PosMin]) == 1)
				PosMin = j;

		temp = A[PosMin]; // Echange du min avec la 1re valeur valide du tableau
		A[PosMin] = A[i];
		A[i] = temp;
	}
}