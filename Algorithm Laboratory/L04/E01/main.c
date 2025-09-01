#include <stdio.h>
#include <stdlib.h>
#define NOMEFILE "../grafo.txt"

typedef struct arco_t{
    int u;
    int v;
} arco_t;

void crea_struct(arco_t *arco, int *N, int *E);
void stampa_struct(arco_t *arco, int N, int E);
int powerset(int pos, int *sol, int N, int E, arco_t *arco, int count);
int check(int *sol, int N, int E, arco_t *arco);


int main(void) {
    int N, E, count=0, *sol;
    arco_t *arco; //alloco dinamicamente

    arco = (arco_t *)calloc(E, sizeof(arco_t));
    if (arco == NULL) {
        printf("Errore allocazione arco\n");
        exit(-1);
    }

    crea_struct(arco, &N, &E); //per allocare memoria in funzioni che non siano il main passo puntatore a puntatore del primo elemento del vettore
    stampa_struct(arco, N, E);

    sol = (int *)calloc(N, sizeof(int));
    if (sol == NULL) {
        printf("Errore allocazione sol\n");
        exit(-1);
    }

    printf("Vertex cover\n");
    count = powerset(0, sol, N, E, arco, count);
    printf ("numero di soluzioni: %d \n", count);

    free(sol);
    free(arco);

    return 0;
}

void crea_struct(arco_t *arco, int *N, int *E) {
    FILE *fp;
    fp = fopen(NOMEFILE, "r");
    int i=0;

    if (fp == NULL) {
        printf("Errore apertura file\n");
        exit(-1);
    }

    fscanf(fp, "%d %d", N, E); //grafo non orientato G di N vertici, identificato da interi che vanno da 0 a N-1, ed E archi, identificati come coppie di vertici (nelle E righe successive ci sono gli archi nella forma ("%d %d", &u, &v))

    while (fscanf(fp, "%d %d", &((arco[i]).u), &((arco[i]).v)) == 2) i++;

    fclose(fp);
}

void stampa_struct(arco_t *arco, int N, int E) {
    for (int i = 0; i < E; i++) {
        printf("%d %d\n", arco[i].u, arco[i].v);
    }
}

int powerset(int pos, int *sol, int N, int E, arco_t *arco, int count) { //genera tutte le n^k possibili combinazioni di sottoinsiemi composti da 2 elementi presi due vertici tra 4 numeri che vanno da 0 a 3
    //cond. di terminazione
    if (pos >= N) {
        if (check(sol, N, E, arco)==1) {
            printf("{ ");
            for (int i=0; i<N; i++) {
                if (sol[i] == 1) { //es: prendo lo 0, prendo l'1, prendo il 2 o prendo il 3 corrispondente a indice i
                    printf("%d ", i); //essendo vettori numerati da 0 a N-1
                }
            }
            printf("}\n");
            return count+1;
        }
        return count;
    }

    sol[pos] = 0; //non prendo elemento
    count = powerset(pos+1, sol, N, E, arco, count); //ricorro su elemento successivo pos+1 da inserire in sol
    sol[pos] = 1; //backtrack -> prendo elemento pos
    count = powerset(pos+1, sol, N, E, arco, count); //ricorro su elemento successivo pos+1 da inserire in sol

    return count;
}

int check(int *sol, int N, int E, arco_t *arco) {
    int flag=1;
    for (int i=0; i<E; i++) {
        if ((sol[arco[i].u] == 0) && (sol[arco[i].v] == 0)) {
            flag = 0;
        }
    }
    return flag;
}