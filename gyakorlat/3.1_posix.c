#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>

void* wait(void* _) //forditott esetben a fo szal nem varja meg, mig lefut az altala inditott, igy befejezodik a program
{
  pthread_t thread;

  thread = pthread_self();
  printf("[%p] Waiting 4 sec\n", thread.p);
  sleep(4);
  printf("[%p] Ready!\n", thread.p);
}

void* create_thread(void* _)
{
  pthread_t thread;

  thread = pthread_self();
  sleep(2);
  printf("[%p] Creating a thread \n", thread.p);

  pthread_t th;
  pthread_create(&th, NULL, wait, NULL);
  //pthread_join(th, NULL);

  sleep(6);
  printf("[%p] Ready!\n", thread.p);
}

int main()
{
    pthread_t thread;

    pthread_create(&thread, NULL, create_thread, NULL);

    pthread_join(thread, NULL);
    return 0;
}
