#include <stdio.h>
#include <stdlib.h>
#define filename "../att1.txt"

typedef struct att_s {
    int s;
    int f;
} att_t;

att_t* collectAct(FILE *fp, int *N);
void mergeSort(att_t *v, int N);
void mergeSortR(int l, int r, att_t *v, att_t *w);
void merge(int l, int m, int r, att_t *v, att_t *w);
void attSelDP(att_t *v, int N);
int checkSol(att_t v1, att_t v2);
int calcDurata(att_t v);
void attSelPrint(att_t *val, int *P, int i);

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

    // Ordino in modo crescente le attività in base ai tempi di fine, così che possiamo costruire
    // una soluzione ottima selezionando prima le attività che finiscono prima,
    // lasciando così il massimo spazio disponibile per le attività successive.
    //Così, infatti, posso fare in modo che una volta che un'attività termina, posso considerare solo le attività che iniziano dopo quel momento.
    mergeSort(v, N);

    attSelDP(v, N);

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

// funzione wrapper -> Chiamata al MergeSort ricorsivo
void mergeSort(att_t *v, int N) {
    int l=0, r=N-1;
    att_t *w; //vettore ausiliario

    w = (att_t *)calloc(N, sizeof(att_t));
    if (w==NULL) {
        printf("Errore allocazione memoria w.");
        exit(-1);
    }

    for (int i = 0; i < N; i++) {
        printf("{%d, %d} ", v[i].s, v[i].f);
    }
    mergeSortR(l, r, v, w);
    printf("\n");
    for (int i = 0; i < N; i++) {
        printf("{%d, %d} ", v[i].s, v[i].f);
    }

}

// MergeSort ricorsivo -> La funzione suddivide il vettore in due parti chiamando sé stessa ricorsivamente per ordinare prima la metà sinistra e poi quella destra
void mergeSortR(int l, int r, att_t *v, att_t *w) {
    if (l < r) { //se l>=r -> il sottovettore ha 1 elemento = è già ordinato
        int m = (l+r)/2;
        mergeSortR(l, m, v, w); // Ordina la prima metà, dividendo fino ad avere un unico sottovettore da 1 elemento
        mergeSortR(m+1, r, v, w);  // Ordina la seconda metà, dividendo fino ad avere un unico sottovettore da 1 elemento
        merge(l, m, r, v, w); // Fonde le due metà ordinate
    }
}

// Funzione di fusione di due sottovettori ordinati (quello da l a q e quello da q+1 a r) in un unico vettore ordinato (merge)
void merge(int l, int m, int r, att_t *v, att_t *w) {
    int i=l, j=m+1, k=l;

    while ((i<=m) && (j<=r)) { //confronta e unisce le due metà (sottovettore sx e dx hanno elementi in posizioni uguali da confrontare)
        if (v[i].f <= v[j].f) {
            w[k++] = v[i++];
        } else {
            w[k++] = v[j++];
        }
    }

    while (i<=m) { // Copia gli elementi rimanenti della prima metà
        w[k++] = v[i++];
    }

    while (j<=r) { // Copia gli elementi rimanenti della seconda metà
        w[k++] = v[j++];
    }

    for (k=l; k<=r; k++) {
        v[k] = w[k]; // Ricopia il vettore ausiliario nel vettore originale
    }
}

void attSelDP(att_t *v, int N) {
    int *maxDur, *prec, ris = 0, last = 0;
    maxDur = (int*)calloc(N+1, sizeof(int));
    prec = (int*)calloc(N+1, sizeof(int));
    if (maxDur == NULL || prec == NULL) {
        printf("Errore allocazione memoria maxDur o prec.");
        exit(-1);
    }

    //inizializzo soluzione con la prima attività che sicuramente è valida e ne salvo durata e indice valore da cui è preceduta
    maxDur[0] = calcDurata(v[0]);
    prec[0] = -1;

    for (int i = 1; i <=N ; i++) {
        maxDur[i] = calcDurata(v[i]); //considere che c'è solo quell'elemento
        prec[i] = -1;
        for (int j = 0; j < i; j++) {
            if (v[j].f <= v[i].s && maxDur[i] < maxDur[j] + calcDurata(v[i])) {
                maxDur[i] = maxDur[j] + calcDurata(v[i]);
                prec[i] = j;
            }
        }
        if (ris < maxDur[i]) {
            ris = maxDur[i];
            last = i;
        }
    }
    printf("\nOne of the Longest Increasing Sequences is ");
    attSelPrint(v, prec, last);
    printf("and its length is %d\n", ris);
}

void attSelPrint(att_t *val, int *P, int i) {
    if (P[i]==-1) {
        printf("(%d %d) ", val[i].s, val[i].f);
        return;
    }
    attSelPrint(val, P, P[i]);
    printf("(%d %d) ", val[i].s, val[i].f);
}

int max(int d1, int d2) {
    if (d1 > d2) return d1;
    return d2;
}

int checkSol(att_t v1, att_t v2) {
    if ((v1.s < v2.f) && (v2.s < v1.f)) return 0;
    return 1;
}

int calcDurata(att_t v) {
    return (v.f-v.s);
}