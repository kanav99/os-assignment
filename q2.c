#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

sem_t mutex,
      write;
int data = 0,
    nreaders = 0;

void *reader(void *arg)
{
  int idx;
  idx = (int) arg;

  sem_wait(&mutex); 
  nreaders +=  1;
  if (nreaders == 1)
    sem_wait(&write);

  sem_post(&mutex);
  printf("Reader - %d read %d\n", idx, data);
  sleep(1);
  sem_wait(&mutex);
  nreaders = nreaders - 1;

  if(nreaders == 0)
   sem_post(&write);
  sem_post(&mutex);
}

void *writer(void *arg)
{
  int writer;
  writer = ((int) arg);
  sem_wait(&write);
  data++;
  printf("Writer - %d wrote %d\n", writer, data);
  sleep(1);
  sem_post(&write);
}

int main()
{
  int i,b; 
  pthread_t readers[5],writers[5];
  sem_init(&mutex,0,1);
  sem_init(&write,0,1);

  // Create both reader and writer threads
  for(i = 0; i <= 2; i++) {
    pthread_create(&writers[i], NULL, writer, (void *) i);
    pthread_create(&readers[i], NULL, reader, (void *) i);
  }
  // Join the created threads
  for(i=0;i<=2;i++)
  {
    pthread_join(writers[i],NULL);
    pthread_join(readers[i],NULL);
  }
  return 0;
}
