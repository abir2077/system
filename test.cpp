#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define K 5
#define T 4
#define D 5

int stock[K];
int ventes_ok = 0, ventes_refusees = 0;
int stop = 0;

void* caisse(void* arg) {
    int id = *(int*)arg;
    while (!stop) {
        int article = rand() % K;
        if (stock[article] > 0) {
            stock[article]--;
            ventes_ok++;
        } else {
            ventes_refusees++;
        }
        usleep(50000);
    }
    return NULL;
}

void* reassort(void* arg) {
    while (!stop) {
        int article = rand() % K;
        stock[article] += 3;
        usleep(200000);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    for (int i = 0; i < K; i++)
        stock[i] = 5;
    pthread_t th_caisses[T], th_reassort;
    int ids[T];
    for (int i = 0; i < T; i++) {
        ids[i] = i;
        pthread_create(&th_caisses[i], NULL, caisse, &ids[i]);
    }
    pthread_create(&th_reassort, NULL, reassort, NULL);
    sleep(D);
    stop = 1;
    for (int i = 0; i < T; i++)
        pthread_join(th_caisses[i], NULL);
    pthread_join(th_reassort, NULL);
    printf("\n=== VERSION NAÏVE ===\n");
    for (int i = 0; i < K; i++)
        printf("Article %d : %d en stock\n", i, stock[i]);
    printf("Ventes réussies : %d\n", ventes_ok);
    printf("Ventes refusées : %d\n", ventes_refusees);
    return 0;
}
