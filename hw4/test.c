#include <stdio.h>
#include "mm.h"

int main()
{
    int i;
    int *intptr = mymalloc(5 * sizeof(int));
    char *charptr = mymalloc(20 * sizeof(char));
    double *doubleptr = mycalloc(5, sizeof(double));

    for (i = 0; i < 5; i++)//set value to intptr
        intptr[i] = i;
    for (i = 0; i < 5; i++)//malloc intptr and it's value
        printf("%d ", intptr[i]);
    printf("\n");
    for (i = 0; i < 5; i++)//malloc intptr 
        printf("%p ", &(intptr[i]));
    printf("\n");
    for (i = 0; i < 5; i++)//malloc charptr
        printf("%p ", &(charptr[i]));
    printf("\n");
    for (i = 0; i < 5; i++)//calloc doubleptr
        printf("%p ", &(doubleptr[i]));
    printf("\n");
    for (i = 0; i < 5; i++)//all zero
        printf("%lf ", doubleptr[i]);
    printf("\n");

    myfree(charptr);//free
    printf("free charptr to free_list\n");

    int *intptr_2 = myrealloc(intptr, 10 * sizeof(int));
    int *intptr_3 = mymalloc(3 * sizeof(int));

    for (i = 0; i < 5; i++)//realloc from intptr to intptr_2
        printf("%p ", &(intptr_2[i]));
    printf("\n");
    for (i = 0; i < 5; i++) //realloc from intptr to intptr_2 and it's value
        printf("%d ", intptr_2[i]);
    printf("\n");
    for (i = 0; i < 3; i++) //use free_list space
        printf("%p ", &(intptr_3[i]));
    printf("\n");
}