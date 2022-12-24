#include <stdio.h>
#include <stdlib.h>
#include "../include/geometry.h"
#include "../include/util.h"

Point* newPoint(long long int x, long long int y)
{
    Point *newP = (Point*)malloc(sizeof(Point)); // Création d'un nouveau point
    if (newP == NULL) ShowMessage("newPoint : echec de l'allocation du point", 1);
    newP->x = x;
    newP->y = y;
    return newP;
}

long long int X(const Point* P)
{
    if (P == NULL) ShowMessage("X : P ne doit pas etre NULL", 1);
    return P->x;
}

long long int Y(const Point* P) 
{
    if (P == NULL) ShowMessage("Y : P ne doit pas etre NULL", 1);
    return P->y;
}

void viewPoint(const void* P)
{
    if (P == NULL) ShowMessage("viewPoint : P ne doit pas etre NULL", 1);
    printf("(%d, %d)", X((Point*)P), Y((Point*)P));
}

void freePoint(void* P)
{
    if (P == NULL) ShowMessage("freePoint : P ne doit pas etre NULL", 1);
    free(P);
}

int onRight(const Point* origin, const Point* destination, const Point* P)
{
    if (origin == NULL) ShowMessage("onRight : origin ne doit pas etre NULL", 1);
    if (destination == NULL) ShowMessage("onRight : destination ne doit pas etre NULL", 1);
    if (P == NULL) ShowMessage("onRight : P ne doit pas etre NULL", 1);

    if ((X(destination) - X(origin)) * (Y(P) - Y(origin)) - (Y(destination) - Y(origin)) * (X(P) - X(origin)) < 0)
        return 1;
    else 
        return 0;
}

int onLeft(const Point* origin, const Point* destination, const Point* P)
{
    if (origin == NULL) ShowMessage("onRight : origin ne doit pas etre NULL", 1);
    if (destination == NULL) ShowMessage("onRight : destination ne doit pas etre NULL", 1);
    if (P == NULL) ShowMessage("onRight : P ne doit pas etre NULL", 1);

    if ((X(destination) - X(origin)) * (Y(P) - Y(origin)) - (Y(destination) - Y(origin)) * (X(P) - X(origin)) > 0)
        return 1;
    else 
        return 0;
}

int collinear(const Point* origin, const Point* destination, const Point* P)
{
    if (origin == NULL) ShowMessage("onRight : origin ne doit pas etre NULL", 1);
    if (destination == NULL) ShowMessage("onRight : destination ne doit pas etre NULL", 1);
    if (P == NULL) ShowMessage("onRight : P ne doit pas etre NULL", 1);

    if ((X(destination) - X(origin)) * (Y(P) - Y(origin)) - (Y(destination) - Y(origin)) * (X(P) - X(origin)) == 0)
        return 1;
    else 
        return 0;
}

int isIncluded(const Point* origin, const Point* destination, const Point* P)
{
    if (origin == NULL) ShowMessage("isIncluded : origin ne doit pas etre NULL", 1);
    if (destination == NULL) ShowMessage("isIncluded : destination ne doit pas etre NULL", 1);
    if (P == NULL) ShowMessage("isIncluded : P ne doit pas etre NULL", 1);

    /* Les distances sont en fait les carrées des distances */
    int distanceSegment = (Y(destination) - Y(origin)) * (Y(destination) - Y(origin)) + (X(destination) - X(origin)) * (X(destination) - X(origin));
    int d1 = (Y(P) - Y(origin)) * (Y(P) - Y(origin)) + (X(P) - X(origin)) * (X(P) - X(origin)); // Distance entre origin et P
    int d2 = (Y(P) - Y(destination)) * (Y(P) - Y(destination)) + (X(P) - X(destination)) * (X(P) - X(destination)); // Distance entre destination et P
    int d = max(d1, d2); // On prend le maximum entre les deux

    if (collinear(origin, destination, P) == 1 && distanceSegment >= d)
        return 1;
    else
        return 0;
}

DEdge* newDEdge(Point* origin, Point* destination)
{
    if (origin == NULL) ShowMessage("newDEdge : origin ne doit pas etre NULL", 1);
    if (destination == NULL) ShowMessage("newDEdge: destination ne doit pas etre NULL", 1);

    DEdge* new_Edge = (DEdge*)malloc(sizeof(Point)); // Création d'un nouvel arc
    if (new_Edge == NULL) ShowMessage("newDEdge: echec de l'allocation de l'arc", 1);
    new_Edge->origin = origin;
    new_Edge->destination = destination;
    return new_Edge;
}

Point* getOrigin(const DEdge* DE)
{
    if (DE == NULL) ShowMessage("getOrigin : DE ne doit pas etre NULL", 1);
    return DE->origin;
}

Point* getDestination(const DEdge* DE)
{
    if (DE == NULL) ShowMessage("getDestination : DE ne doit pas etre NULL", 1);
    return DE->destination;
}

void viewDEdge(const void* DE)
{
	if (DE == NULL) ShowMessage("viewDEdge : DE ne doit pas etre NULL", 1);

    printf("[ Origine : ");
    viewPoint(getOrigin(DE));
    printf(" ; Destination : ");
    viewPoint(getDestination(DE));
    printf(" ]");
}

void freeDEdge(void* DE)
{
    if (DE == NULL) ShowMessage("freeDEdge : DE ne doit pas etre NULL", 1);
    free(DE);
}