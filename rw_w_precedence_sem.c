#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>
#include<pthread.h>

sem_t *meR1, meR2, *rSem
sem_t *meW, *wSem;
int nR, nW;

static void *readers(void *arg){
	int *id = (int *)arg;
	sem_wait(meR1);
		sem_wait(rSem);
			sem_wait(meR2);
			nR++;
			if(nR == 1)
				sem_wait(wSem);
			sem_post(meR2);
		sem_post(rSem);
	sem_post(meR1);
	
	printf("Reader %d reading...\n", *id);
	sleep(1);

	sem_wait(meR2);
	nR--;
	if(nR == 0)
		sem_post(wSem);
	sem_post(meR2);
  return NULL;
}

static void *writers(void *arg){
	int *id = (int *)arg;
	sem_wait(meW);
	nW++;
	if(nW == 1)
		sem_wait(rSem);
	sem_post(meW);
	
	/*Guarded Write*/
	sem_wait(wSem);
		printf("Writer %d writing\n",*id);
		sleep(1);
	sem_post(wSem);
	
	sem_wait(meW);
	nW--;
	if(nW == 0)
		sem_post(rSem);
	sem_post(meW);
  return NULL;
}

void init(){
	nR = nW = 0;
	
	meR1 = (sem_t *)malloc(sizeof(sem_t));
	sem_init(meR1, 0, 1);
	meR2 = (sem_t *)malloc(sizeof(sem_t));
	sem_init(meR2, 0, 1);
	rSem = (sem_t *)malloc(sizeof(sem_t));
	sem_init(rSem, 0, 1);
		
	meW = (sem_t *)malloc(sizeof(sem_t));
	sem_init(meW, 0, 1);
	wSem = (sem_t *)malloc(sizeof(sem_t));
	sem_init(wSem, 0, 1);
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
