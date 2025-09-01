#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 256

typedef struct {
    char **scelte;
    int n_scelte;
} val_t;

void leggi_file(val_t **val, char ***sol, int *N);
int crea_playlist(int pos, val_t *val, char **sol, int n, int cnt);
void free_memory(val_t *val, char **sol, int N);

int main(void) {
    val_t *val;
    char **playlist;
    int N, pos=0, cnt=0, num;
    leggi_file(&val, &playlist, &N);
    num = crea_playlist(pos, val, playlist, N, cnt);
    printf("\nLe playlist generate in totale sono: %d", num);
    free_memory(val, playlist, N);
    return 0;
}

void leggi_file(val_t **val, char ***sol, int *N) {
    FILE *fp = fopen("../brani.txt", "r");
    if (fp == NULL) {
        printf("Errore apertura file\n");
        exit(-1);
    }

    fscanf(fp, "%d", N);
    (*val) = (val_t *) malloc((*N) * sizeof(val_t));
    if (*val == NULL) {
        printf("Errore allocazione memoria val.\n");
        exit(-1);
    }


    for (int i = 0; i < (*N); i++) {
        fscanf(fp, "%d", &((*val)[i].n_scelte));
        (*val)[i].scelte = (char **) malloc((*val)[i].n_scelte * sizeof(char*)); //vettore che deve contere le canzoni per ogni persona
        if ((*val)[i].scelte == NULL) {
            printf("Errore allocazione memoria scelte.\n");
            exit(-1);
        }

        for (int j = 0; j < (*val)[i].n_scelte; j++) {
            ((*val)[i].scelte)[j] = (char *) malloc(MAXLEN * sizeof(char));
            if (((*val)[i].scelte)[j] == NULL) {
                printf("Errore allocazione memoria titoli canzoni.\n");
                exit(-1);
            }
            fscanf(fp, "%s", ((*val)[i].scelte)[j]);
        }
    }

    (*sol) = (char **) malloc((*N) * sizeof(char*)); //matrice = vettore di stringhe (puntatori al primo carattere della stringa)
    if (*sol == NULL) {
        printf("Errore allocazione memoria sol.\n");
        exit(-1);
    }

    for (int i = 0; i < (*N); i++) {
        (*sol)[i] = (char *) malloc(MAXLEN * sizeof(char));
        if ((*sol)[i] == NULL) {
            printf("Errore allocazione memoria sol[i].\n");
            exit(-1);
        }
    }

    fclose(fp);
}

int crea_playlist(int pos, val_t *val, char **sol, int k, int cnt) {
    int i;
    if (pos >= k) {
        printf("La playlist n. %d e': ", cnt+1);
        for (i = 0; i < k; i++)
            printf("%s ", sol[i]);
        printf("\n");
        return cnt+1;
    }
    for (i = 0; i < val[pos].n_scelte; i++) {
        strcpy(sol[pos], val[pos].scelte[i]);
        cnt = crea_playlist(pos+1, val, sol, k, cnt);
    }
    return cnt;
}

void free_memory(val_t *val, char **sol, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < val[i].n_scelte; j++) {
            free(val[i].scelte[j]);
        }
        free(val[i].scelte);
        free(sol[i]);
    }
    free(val);
    free(sol);
}