#include <stdio.h>
#include "mm.h"

int main()
{
    int *int_array, i;
    long *long_array;
    double *double_array;
    float *float_array;
    int_array = mymalloc(3 * sizeof(int));
    printf("mymalloc allocated a integer array of size 3, bellow is their addresses\n");
    for (i = 0; i < 3; ++i)
    {
        printf("%p ", &(int_array[i]));
    }
    printf("\n");
    long_array = mycalloc(3, sizeof(long));
    printf("mycalloc allocated a long array of size 3, bellow is their values\n");
    for (i = 0; i < 3; ++i)
    {
        printf("%ld ", long_array[i]);
    }
    printf("\n");
    double_array = mymalloc(3 * sizeof(double));
    printf("mymalloc allocated a double array of size 3\n %p %p %p\n", &double_array[0],&double_array[1],&double_array[2]);
    float_array = myrealloc(double_array, 3 * sizeof(float));
    printf("myrealloc reallocated double_array as a float array of size 3, bellow is their addresses\n");
    for (i = 0; i < 3; ++i)
    {
        printf("%p ", &(float_array[i]));
    }
    printf("\n");
    myfree(int_array);
    printf("int_array was free\n");
    myfree(long_array);
    printf("long_array was free\n");
    myfree(float_array);
    printf("float_array was free\n");
}