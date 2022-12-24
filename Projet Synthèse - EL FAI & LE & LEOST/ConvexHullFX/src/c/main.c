#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "../include/list.h"
#include "../include/tree.h"
#include "../include/heap.h"
#include "../include/sort.h"
#include "../include/geometry.h"
#include "../include/algo.h"

#include <time.h> 

int main(int argc, char *argv[]) { 
    clock_t start1, start2, start3, start4, start5, end1, end2, end3, end4, end5; 
    double cpu_time_used1, cpu_time_used2, cpu_time_used3, cpu_time_used4, cpu_time_used5; 

    start1 = clock();
    SlowConvexHull("fichier1.txt", "fichier2.txt"); 
    end1 = clock(); 
    start2 = clock();
    ConvexHull("fichier1.txt", "fichier2.txt", 1);
    end2 = clock();
    start3 = clock();
    ConvexHull("fichier1.txt", "fichier2.txt", 2);
    end3 = clock();
    start4 = clock();
    ConvexHull("fichier1.txt", "fichier2.txt", 3);
    end4 = clock();
    start5 = clock();
    RapidConvexHull("fichier1.txt", "fichier2.txt");
    end5 = clock();

    cpu_time_used1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC; 
    cpu_time_used2 = ((double) (end2 - start2)) / CLOCKS_PER_SEC;
    cpu_time_used3 = ((double) (end3 - start3)) / CLOCKS_PER_SEC;
    cpu_time_used4 = ((double) (end4 - start4)) / CLOCKS_PER_SEC;
    cpu_time_used5 = ((double) (end5 - start5)) / CLOCKS_PER_SEC;
    
    printf("SlowConvexHull : \t%lf sec\n", cpu_time_used1); 
    printf("ConvexHull (tri par tas avac un arbre) : \t%lf sec\n", cpu_time_used2); 
    printf("ConvexHull (tri par tas avec un tableau) : \t%lf sec\n", cpu_time_used3); 
    printf("ConvexHull (tri par sélection) : \t%lf sec\n", cpu_time_used4); 
    printf("RapidConvexHull : \t%lf sec\n", cpu_time_used5); 

    return 0;
}

/*
int main(int argc, char *argv[]) { 
    if (argc < 2) ShowMessage("main : erreur arg", 1);

    clock_t start, end; 
    double cpu_time_used; 

// entourer le sous-programme à évaluer avec deux appels à clock() 
    start = clock();

    switch (atoi(argv[1])) {
        case 1 : 
            printf("SlowConvexHull\n");
            SlowConvexHull("fichier1.txt", "fichier2.txt"); 
            break;
        case 2 :
            if (argc < 3) ShowMessage("main : erreur arg", 1);
            printf("ConvexHull\n");
            ConvexHull("fichier1.txt", "fichier2.txt", atoi(argv[2])); 
            break;
        case 3 :
            printf("RapidConvexHull\n");
            RapidConvexHull("fichier1.txt", "fichier2.txt"); 
            break;
        default :
            ShowMessage("main : erreur arg", 1);
    }
    
    end = clock(); 

// calculer le temps d’exécution 
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; 
    printf("Time used : \t%lf sec\n", cpu_time_used); 

    return 0;
}
*/

