#include <stdio.h>
#include <stdlib.h>
#define filename "../att.txt"

typedef struct att_s {
    int s;
    int f;
} att_t;

att_t* collectAct(FILE *fp, int *N);
void attSel(int N, att_t *v);
void attSelR(int pos, int *sol, int *bestSol, int durata, int *bestDurata, att_t *v, int N);
int checkSol(att_t *v, int *sol, int pos);
int checkBest(int d1, int d2);
int calcDurata(att_t v);
void stampaSol(att_t *v, int n, int *sol, int durata);

int main(void) {
    int N=0;
    att_t *v;

    FILE *fp_in = fopen(filename, "r");
    if (fp_in == NULL) {
        printf("Errore apertura file.");
        return -1;
    }

    v = collectAct(fp_in, &N);
    fclose(fp_in);

    attSel(N, v);
    free(v);
    return 0;
}

att_t* collectAct(FILE *fp, int *N) {
    att_t *v;
    fscanf(fp, "%d", N);

    v = (att_t*)calloc(*N, sizeof(att_t));
    if (v == NULL) {
        printf("Errore allocazione memoria v.");
        exit(-1);
    }

    for (int i=0; i<*N; i++) {
        fscanf(fp, "%d %d", &v[i].s, &v[i].f);
    }
    return v;
}

void attSel(int N, att_t *v) {
    int *sol, *bestSol, bestDurata=0;

    sol = (int *)calloc(N, sizeof(int));
    bestSol = (int *)calloc(N, sizeof(int));
    if (sol == NULL || bestSol == NULL) {
        printf("Errore allocazione memoria vettori soluzione.");
        free(sol);
        free(bestSol);
        exit(-1);
    }

    attSelR(0, sol, bestSol, 0, &bestDurata, v, N);
    stampaSol(v, N, bestSol, bestDurata);

    free(sol);
    free(bestSol);
}

void attSelR(int pos, int *sol, int *bestSol, int durata, int *bestDurata, att_t *v, int N) {
    if (pos >= N) {
        if (checkBest(durata, *bestDurata)) {
            *bestDurata = durata;
            for (int i=0; i<N; i++) {
                bestSol[i] = sol[i];
            }
        }
        return;
    }

    //non prendo attività
    sol[pos] = 0;
    attSelR(pos+1, sol, bestSol, durata, bestDurata, v, N);
    //prendi attività solo se non si interseca con altre -> pruining
    if (checkSol(v, sol, pos)) {
        //posso prendere l'attività
        sol[pos] = 1;
        durata += calcDurata(v[pos]);
        attSelR(pos+1, sol, bestSol, durata, bestDurata, v, N);
    }

}

int checkSol(att_t *v, int *sol, int pos) {
    for (int i=0; i<pos; i++) {
        if (sol[i] != 0) {
            if ((v[pos].s < v[i].f) && (v[i].s < v[pos].f)) return 0;
        }
    }
    return 1;
}

int checkBest(int d1, int d2) {
    if (d1 > d2) return 1;
    return 0;
}

int calcDurata(att_t v) {
    return (v.f-v.s);
}

void stampaSol(att_t *v, int n, int *sol, int durata){
    printf("Se S =( ");
    for (int j=0; j<n; j++) {
        printf("(%d,%d) ", v[j].s, v[j].f);
    }
    printf("),\nuno dei sottoinsiemi di S di attivita' compatibili che massimizza la somma delle durate e':\n");
    for (int i=0; i<n; i++) {
        if (sol[i]) { // != 0 -> true
            printf("(%d,%d) ", v[i].s, v[i].f);
        }
    }
    printf("\nper una somma delle durate pari a %d.", durata);
}