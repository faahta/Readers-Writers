/*READERS WRITERS PROBLEM WITH READERS PRECEDENCE
WITH MUTEX AND CONDITION VARIABLES*/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>

int nR, nW;
pthread_mutex_t lock;
pthread_cond_t r_cond, w_cond;

static void *readers(void *arg){
	int *id = (int *)arg;
	sleep(1);
	pthread_mutex_lock(&lock);
		nR++;
		while(nW > 0)
			pthread_cond_wait(&r_cond, &lock);
	pthread_mutex_unlock(&lock);
	
	printf("Reader %d reading...\n", *id);
	sleep(1);
	pthread_mutex_lock(&lock);
		nR--;
	pthread_mutex_unlock(&lock);
	
	pthread_cond_broadcast(&r_cond);
	pthread_cond_broadcast(&w_cond);
  return NULL;
}

static void *writers(void *arg){
	int *id = (int *)arg;
	pthread_mutex_lock(&lock);
		while(nW > 0 && nR>0)
			pthread_cond_wait(&w_cond, &lock);
	pthread_mutex_unlock(&lock);
	
	pthread_mutex_lock(&lock);
		nW++;
	pthread_mutex_unlock(&lock);
	printf("Writer %d writing...\n", *id);
	sleep(5);
	pthread_mutex_lock(&lock);
		nW--;
	pthread_mutex_unlock(&lock);
	
	pthread_cond_broadcast(&r_cond);
	pthread_cond_broadcast(&w_cond);
  return NULL;
}

void init(){
	nR = 0;
	nW = 0;
	pthread_mutex_init(&lock, NULL);
	pthread_cond_init(&r_cond, NULL);
	pthread_cond_init(&w_cond, NULL);
}

int main(){
	
	pthread_t *thr,*thw;
	thr = (pthread_t *)malloc(20 * sizeof(pthread_t));
	thw = (pthread_t *)malloc(20 * sizeof(pthread_t));
	init();
	int i;
	int *pi;
	for(i=0; i<20; i++){
		pi = (int *)malloc(sizeof(int));
		*pi = i;
		pthread_create(&thr[i], NULL, readers, (void *)pi);
		pthread_create(&thw[i], NULL, writers, (void *)pi);
			sleep(1);
	}
	
	pthread_exit (0);


}
