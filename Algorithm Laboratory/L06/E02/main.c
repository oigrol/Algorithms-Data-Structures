#include <stdio.h>
#include <stdlib.h>
#define filename "../hard_test_set.txt"
#define N 4
#define MAX_Z 50
#define MAX_R 50
#define MAX_T 50
#define MAX_S 50

typedef enum {
    Z, //0
    R, //1
    T, //2
    S  //3
} pietre_id;

typedef struct pietre_s {
    int id; //identifica pietra
    int n; //pietre di id totali
} pietre_t;

int *****allocaMem(int z, int r, int t, int s);
void liberaMemoria(int *****m);
void calcLen(pietre_t *p, int *****m);
pietre_t* crea_struct(FILE *fp, int *n);
int fZ(int z, int r, int t, int s, int *****m);
int fR(int z, int r, int t, int s, int *****m);
int fT(int z, int r, int t, int s, int *****m);
int fS(int z, int r, int t, int s, int *****m);
int max(int l1, int l2);
void stampa_struct(pietre_t *p);
char converti(int id);


int main(void) {
    int num_righe, num_pietre, *****mem;
    pietre_t *pietre;

    FILE *fp_in = fopen(filename, "r");
    if (fp_in == NULL) {
        printf("Errore apertura file.");
        return -1;
    }

    fscanf(fp_in, "%d", &num_righe);

    mem = allocaMem(MAX_Z, MAX_R, MAX_T, MAX_S);

    for (int i=0; i<num_righe; i++) {
        printf("\nCollana n. %d.\n", i+1);
        pietre = crea_struct(fp_in, &num_pietre);
        stampa_struct(pietre);
        printf("Lunghezza massima possibile: %d\n", num_pietre);

        calcLen(pietre, mem);

        free(pietre);
    }
    liberaMemoria(mem);

    fclose(fp_in);

    return 0;
}

pietre_t* crea_struct(FILE *fp, int *n) {
    int tot=0;
    pietre_t *p = (pietre_t *)calloc(N, sizeof(pietre_t));
    if (p == NULL) {
        printf("Errore allocazione memoria pietre.");
        exit(-1);
    }

    for (int i=0; i<N; i++) {
        p[i].id = i;
        fscanf(fp, "%d", &p[i].n);
        tot += p[i].n;
    }

    *n = tot;

    return p;
}

int *****allocaMem(int z, int r, int t, int s) {
    int *****m = malloc(MAX_Z * sizeof(int ****));
    for (int z = 0; z < MAX_Z; z++) {
        m[z] = malloc(MAX_R * sizeof(int ***));
        for (int r = 0; r < MAX_R; r++) {
            m[z][r] = malloc(MAX_T * sizeof(int **));
            for (int t = 0; t < MAX_T; t++) {
                m[z][r][t] = malloc(MAX_S * sizeof(int *));
                for (int s = 0; s < MAX_S; s++) {
                    m[z][r][t][s] = malloc(N * sizeof(int));
                    for (int i = 0; i < N; i++) {
                        m[z][r][t][s][i] = -1;
                    }
                }
            }
        }
    }
    return m;
}

void liberaMemoria(int *****m) {
    for (int z = 0; z < MAX_Z; z++) {
        for (int r = 0; r < MAX_R; r++) {
            for (int t = 0; t < MAX_T; t++) {
                for (int s = 0; s < MAX_S; s++) {
                    free(m[z][r][t][s]);
                }
                free(m[z][r][t]);
            }
            free(m[z][r]);
        }
        free(m[z]);
    }
    free(m);
}

void calcLen(pietre_t *p, int *****m) {
    int z = p[Z].n, r = p[R].n, t = p[T].n, s = p[S].n;
    int maxZ = fZ(z, r, t, s, m);
    int maxR = fR(z, r, t, s, m);
    int maxT = fT(z, r, t, s, m);
    int maxS = fS(z, r, t, s, m);

    int maxLen = max(maxZ, max(maxR, max(maxT, maxS)));
    printf("Collana massima di lunghezza %d\n", maxLen);
}

int fZ(int z, int r, int t, int s, int *****m) {
    if (z <= 0) return 0; //caso base -> non ci sono altri zaffiri

    if (m[z][r][t][s][Z] != -1) return m[z][r][t][s][Z];

    int max_len = 1; //considero che vi è solo la prima gemma

    //Dopo un zaffiro posso aggiungere o un zaffiro o un rubino -> prendo quello che mi porta a len max
    max_len = 1 + max(fZ(z-1, r, t, s, m), fR(z-1, r, t, s, m));

    //memorizzo il risultato
    m[z][r][t][s][Z] = max_len;

    return max_len;
}

int fR(int z, int r, int t, int s, int *****m) {
    if (r <= 0) return 0; //caso base -> non ci sono altri rubini

    if (m[z][r][t][s][R] != -1) return m[z][r][t][s][R];

    int max_len = 1; //considero che vi è solo la prima gemma

    //Dopo un rubino posso aggiungere o un rubino o uno zaffiro -> prendo quello che mi porta a len max
    max_len = 1 + max(fS(z, r-1, t, s, m), fT(z, r-1, t, s, m));

    //memorizzo il risultato
    m[z][r][t][s][R] = max_len;

    return max_len;
}

int fT(int z, int r, int t, int s, int *****m) {
    if (t <= 0) return 0; //caso base -> non ci sono altre pietre

    if (m[z][r][t][s][T] != -1) return m[z][r][t][s][T];

    int max_len = 1; //considero che vi è solo la prima gemma

    //Dopo un topazio posso aggiungere o un topazio o uno smeraldo -> prendo quello che mi porta a len max
    max_len = 1 + max(fZ(z, r, t-1, s, m), fR(z, r, t-1, s, m));

    //memorizzo il risultato
    m[z][r][t][s][T] = max_len;

    return max_len;
}

int fS(int z, int r, int t, int s, int *****m) {
    if (s <= 0) return 0; //caso base -> non ci sono altre pietre

    if (m[z][r][t][s][S] != -1) return m[z][r][t][s][S];

    int max_len = 1; //considero che vi è solo la prima gemma

    //Dopo uno smeraldo posso aggiungere o uno smeraldo o un topazio -> prendo quello che mi porta a len max
    max_len = 1 + max(fS(z, r, t, s-1, m), fT(z, r, t, s-1, m));

    //memorizzo il risultato
    m[z][r][t][s][S] = max_len;

    return max_len;
}

int max(int l1, int l2) {
    if (l1>l2) return l1;
    return l2;
}

void stampa_struct(pietre_t *p) {
    printf("Pietre disponibili:\n");
    for (int i=0; i<N; i++) {
        printf("%c -> %d\n", converti(p[i].id), p[i].n);
    }
}

char converti(int i) {
    switch (i) {
        case Z: return 'Z';
        case R: return 'R';
        case T: return 'T';
        case S: return 'S';
        default: return '?';
    }
}