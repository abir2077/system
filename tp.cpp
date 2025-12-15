#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>

typedef struct{
    pthread_mutex_t m;
    pthread_cond_t cv;
    bool has_value;
    int value ;
} monitor_t;

void mon_init(monitor_t *mon){
    pthread_mutex_init(&mon->m, NULL);
    pthread_cond_init(&mon->cv, NULL);
    mon->has_value= false;
    mon->value = 0;
}
void mon_destroy(monitor_t *mon){
    pthread_cond_destroy(&mon->cv);
    pthread_mutex_destroy(&mon->m);
}
void* producer(void* arg){
    monitor_t* mon = (monitor_t*)arg;
    pthread_mutex_lock(&mon->m);
    while(mon->has_value)
    pthread_cond_wait(&mon->cv,&mon->m);
mon->value= 42;
mon->has_value = true;
pthread_cond_signal(&mon->cv);
pthread_mutex_unlock(&mon->m);
return NULL;
}
void* consumer(void* arg){
    monitor_t* mon = (monitor_t*)arg;
    pthread_mutex_lock(&mon->m);
    while(!mon->has_value)
    pthread_cond_wait(&mon->cv,&mon->m);
int v = mon->value;
mon->has_value = false;
pthread_cond_signal(&mon->cv);
pthread_mutex_unlock(&mon->m);
printf("[A] Consommateur a recu :%d\n",v);
return NULL;
}
int main(void){
    monitor_t mon; mon_init(&mon);
    pthread_t p,c;
    pthread_create(&p, NULL,producer,&mon);
    pthread_create(&c,NULL,consumer,&mon);
    pthread_join(p,NULL);
    pthread_join(c,NULL);
    mon_destroy(&mon);
    return 0;
}
