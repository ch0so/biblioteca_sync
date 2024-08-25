#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int wait;
} barrera;

void barrera_init(barrera *barr, int count){

    pthread_mutex_init(&barr->mutex, NULL);
    pthread_cond_init(&barr->cond, NULL);
    barr->count = count;
    barr->wait = 0;

}

void barrera_wait(barrera *barr){

    pthread_mutex_lock(&barr->mutex);
    barr->wait++;
    if(barr->wait == barr->count){
        pthread_cond_broadcast(&barr->cond);
        barr->wait = 0;
    } else {
        pthread_cond_wait(&barr->cond, &barr->mutex);
    }

    pthread_mutex_unlock(&barr->mutex);

}

void barrera_destroy(barrera *barr){

    pthread_mutex_destroy(&barr->mutex);
    pthread_cond_destroy(&barr->cond);

}

#define HILOS 70

barrera barr;

void* func_ej(void* arg){

    int id = *((int*) arg);

    printf("Hilo %d llego a la barrera\n", id);

    barrera_wait(&barr);

    printf("Hilo %d paso la barrera\n", id);

    return NULL;
    
}

int main(){

    barrera_init(&barr, HILOS);

    pthread_t hilos[HILOS];
    int ids[HILOS];

    for(int i=0; i<HILOS; i++){
        ids[i] = i;
        pthread_create(&hilos[i], NULL, func_ej, &ids[i]);
    }

    for (int i=0; i<HILOS; i++){
        pthread_join(hilos[i], NULL);
    }

    barrera_destroy(&barr);

    return 0;
}