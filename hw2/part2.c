#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#define TRUE 1

#define N 5
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N
#define THINKING 0
#define HUNGRY 1
#define EATING 2
typedef int semaphore;
int state[N];
semaphore mutex = 1;
semaphore s[N];

void what_are_you_doing(int num, char *doing)
{
    printf("Philosopher %d is going to %s\n", num, doing);
}

void think()
{
    usleep(1000000 * (rand() % 3 + 1));
}

void eat()
{
    usleep(1000000 * (rand() % 3 + 1));
}

void down(semaphore *m)
{
    while (m == 1)
        ;
    *m = 1; //lock
}

void up(semaphore *m)
{
    *m = 0; //unlock
}

void test(int i)
{
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
    {
        state[i] = EATING;
        what_are_you_doing(i, "eat");
        up(&s[i]);
    }
}
void take_forks(int i)
{
    down(&mutex);
    state[i] = HUNGRY;
    what_are_you_doing(i, "be hungry");
    test(i);
    up(&mutex);
    down(&s[i]);
}
void put_forks(int i)
{
    down(&mutex);
    state[i] = THINKING;
    what_are_you_doing(i, "think");
    test(LEFT);
    test(RIGHT);
    up(&mutex);
}
void *philosopher(void *i)
{
    what_are_you_doing((int)i, "think");
    while (TRUE)
    {
        think();
        take_forks(i);
        eat();
        put_forks(i);
    }
}

int main()
{
    srand(time(NULL));

    pthread_t thread[N];
    pthread_attr_t attr;
    long int i;
    pthread_attr_init(&attr);
    mutex = 0;

    for (i = 0; i < N; i++)
    {
        s[i] = 0;
        pthread_create(&thread[i], &attr, &philosopher, (void *)i); //to meet the arguments, func. philosoper and it's parameter had modeified, different from textbook
    }
    pthread_attr_destroy(&attr);
    for (i = 0; i < N; i++)
        pthread_join(thread[i], NULL);

    return 0;
}