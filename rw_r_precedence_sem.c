#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>

sem_t *meR, *meW;
sem_t *wSem;
int nR;

static void *readers(void *arg){
	int *id = (int *)arg;
	sem_wait(meR);
		printf("nR = %d\n",nR);
		nR++;
		if(nR == 1)
			sem_wait(wSem);
	sem_post(meR);
	
	printf("Reader %d reading...\n", *id);
	sleep(1);

	sem_wait(meR);
		nR--;
		if(nR == 0)
			sem_post(wSem);
	sem_post(meR);	
  return NULL;
}

static void *writers(void *arg){
	int *id = (int *)arg;
	sem_wait(meW);
		sem_wait(wSem);
	printf("Writer %d writing...\n",*id);
	sleep(10);
	sem_post(wSem);
	sem_post(meW);
  return NULL;
}

void init(){
	nR = 0;
	meR = (sem_t *)malloc(sizeof(sem_t));
	sem_init(meR, 0, 1);
	
	meW = (sem_t *)malloc(sizeof(sem_t));
	sem_init(meW, 0, 1);
	
	w = (sem_t *)malloc(sizeof(sem_t));
	sem_init(w, 0, 1);
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
