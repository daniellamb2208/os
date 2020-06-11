#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <time.h>

#define true 1
#define length 1024
#define handel_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

void create(int mode)
{
    char *addr;
    int fd;
    struct stat sb;

    fd = open("mmap.txt", O_RDWR | O_CREAT);
    if (fd == -1)
        handel_error("opening the file is failure\n");
    if (fstat(fd, &sb) == -1)
        handel_error("fstat error\n");

    lseek(fd, length + 1, SEEK_SET);
    write(fd, "", 1);
    lseek(fd, 0, SEEK_SET);

    addr = mmap(0, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED)
        handel_error("mmap failure\n");

    if (mode == 1)
    {
        char r = (rand() % 26 + 65);
        sprintf(addr, "%c\n", r);
        printf("txt is now %c\n", r);
    }
    else
    {
        char txt[1024];
        printf("please enter your txt: ");
        scanf("%s", txt);
        sprintf(addr, "%s\n", txt);
        printf("txt is %s now\n", txt);
    }

    msync(addr, length, MS_SYNC);
    munmap(addr, length);
    close(fd);
    printf("\n");
}

int main()
{
    srand(time(NULL));
    while (true)
    {
        int mode;
        printf("Random to change txt by upper-alphabet enter 1\nCustimize input enter 2\nZero for exit 0\n");
        scanf("%d", &mode);
        if (mode == 0)
            exit(EXIT_SUCCESS);

        create(mode);
    }

    return 0;
}