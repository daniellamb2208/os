#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string>
#include <iostream>
using namespace std;

#define N 5
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int state[N];
pthread_mutex_t mutex;
pthread_cond_t mthread[N];

void *p_doing(void *a)
{
    while (true)
    {
        usleep(500000);
        for (int i = 0; i < N; i++)
        {
            switch (state[i])
            {
            case 0:
                cout << "T ";break;
            case 1:
                cout << "H ";break;
            case 2:
                cout << "E ";break;
            default:
                break;
            }
        }
        cout << endl;
    }
}

class Monitor
{
public:
    void think()
    {
        usleep(1000000 * (rand() % 3 + 1));
    }

    void eat()
    {
        usleep(1000000 * (rand() % 3 + 1));
    }

    void test(int i)
    {
        if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
        {
            state[i] = EATING;
            pthread_cond_signal(&mthread[i]);
        }
    }
    void take_forks(int i)
    {
        pthread_mutex_lock(&mutex);
        state[i] = HUNGRY;
        test(i);
        if (state[i] != EATING)
            pthread_cond_wait(&mthread[i], &mutex);
        pthread_mutex_unlock(&mutex);
    }
    void put_forks(int i)
    {
        pthread_mutex_lock(&mutex);
        state[i] = THINKING;
        test(LEFT);
        test(RIGHT);
        pthread_mutex_unlock(&mutex);
    }
} monitor;
void *philosopher(void *i)
{
    long x = (long)i;   //strange
    while (true)
    {
        monitor.think();
        monitor.take_forks(x);
        monitor.eat();
        monitor.put_forks(x);
    }
}

int main()
{
    srand(time(NULL));

    pthread_t thread[N];
    pthread_attr_t attr;
    long int i;
    pthread_attr_init(&attr);
    pthread_mutex_init(&mutex, NULL);

    pthread_t t;            //for outputing
    void *tt;
    pthread_create(&t, &attr, &p_doing, tt);

    for (i = 0; i < N; i++)
    {
        pthread_cond_init(&mthread[i], NULL);
        pthread_create(&thread[i], &attr, &philosopher, (void *)i);
    }
    pthread_attr_destroy(&attr);

    pthread_join(t, NULL);//joining the output thread
    for (i = 0; i < N; i++)
    {
        pthread_join(thread[i], NULL);
        pthread_cond_destroy(&mthread[i]);
    }
    pthread_mutex_destroy(&mutex);
    return 0;
}