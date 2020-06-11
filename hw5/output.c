#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define true 1

void output()
{
    FILE *fd = fopen("mmap.txt", "r");

    if(fd == NULL)
    {
        perror("FILE DOESN'T EXIST\n");
        exit(EXIT_FAILURE);
    }
    char buf[1024];
    fgets(buf, 1024, fd);
    printf("%s", buf);
    fclose(fd);
}

int main()
{
    while (true)
    {
        output();
        sleep(1);
    }

    return 0;
}