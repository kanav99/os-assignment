// Producer-Consumer Problem using semaphores
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// Global Variables
pthread_t *producers;
pthread_t *consumers;
sem_t mutex, /* To make sure only one consumer/producer accesses a buffer at a time*/
	  empty,
	  fill,
	  prnt;
int *buf,
	buf_pos = -1,
	nproducers,
	nconsumers,
	buf_len;

int produce(pthread_t self){
	int i = 0;
	int product = 1 + (rand() % 40);
	while(!pthread_equal(producers[i], self) && i < nproducers){
		i += 1;
	}
	sem_wait(&prnt);
	printf("Producer %d produced %d \n", i + 1, product);
	sem_post(&prnt);
	return product;
}


void consume(int p,pthread_t self){
	int i = 0;
	while(!pthread_equal(consumers[i], self) && i < nconsumers){
		i += 1;
	}
	sem_wait(&prnt);
	printf("\nConsumer %d consumed %d \n", i + 1, p);
	sem_post(&prnt);
}


void* producer(void *args){
	while(1){
		int product = produce(pthread_self());
		sem_wait(&empty);
		sem_wait(&mutex);
		buf_pos += 1;
		buf[buf_pos] = product; 
		sem_post(&mutex);
		sem_post(&fill);
		sleep(1 + (rand() % 7));
	}
	return NULL;
}


void* consumer(void *args){
	int c;
	while(1){
		sem_wait(&fill);
		sem_wait(&mutex);
		c = buf[buf_pos];
		consume(c, pthread_self());
		buf_pos--;
		sem_post(&mutex);
		sem_post(&empty);
		sleep(1 + (rand() % 11));
	}
	return NULL;
}

int main(){
	
	srand(time(NULL));

	sem_init(&mutex, 0, 1);
	sem_init(&fill, 0, 0);
	sem_init(&prnt , 0, 1);

	printf("Number of Producers?:");
	scanf("%d", &nproducers);
	producers = (pthread_t*) malloc(nproducers * sizeof(pthread_t));
	printf("Number of Consumers?:");
	scanf("%d", &nconsumers);
	consumers = (pthread_t*) malloc(nconsumers * sizeof(pthread_t));

	printf("Buffer Length?:");
	scanf("%d", &buf_len);
	buf = (int*) malloc(buf_len * sizeof(int));

	sem_init(&empty, 0, buf_len);

	// Initialize threads
	// Producers' thread
	for(int i = 0; i < nproducers; i++){
		pthread_create(producers + i, NULL, &producer, NULL);
	}
	// Consumer Threads
	for(int i = 0; i < nconsumers; i++){
		pthread_create(consumers + i, NULL, &consumer, NULL);
	}

	// Join the threads
	for(int i = 0; i < nproducers ; i++){
		pthread_join(producers[i], NULL);
	}
	for(int i = 0; i < nconsumers; i++){
		pthread_join(consumers[i], NULL);
	}
	return 0;
}

