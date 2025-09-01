#include <stdio.h>
#include <stdlib.h>

int crea_vet(int **a);
int majority(int *a, int N);
void stampa_vet(int *a, int N);
int conta_occorrenze(int *a, int N, int chiave);

int main(void) {
    int *vet, N, maggioritario;

    N = crea_vet(&vet);
    stampa_vet(vet, N);

    maggioritario = majority(vet, N);

    if (maggioritario != -1 && conta_occorrenze(vet, N, maggioritario) > N / 2) {
        printf("L'elemento maggioritario del vettore e': %d\n", maggioritario);
    } else {
        printf("Non esiste un elemento maggioritario nel vettore.\n");
    }

    free(vet);
    return 0;
}

int crea_vet(int **a) {
    int i=0, N, MAXN=1, d;

    *a = (int *)malloc(MAXN * sizeof(int));
    printf("Inserire numero:\n");
    while(scanf("%d", &d) > 0 && d > 0) {
        if (i == MAXN) {
            MAXN *= 2;
            *a = (int *)realloc(*a, MAXN * sizeof(int));
        }
        (*a)[i++] = d;
    }
    N = i;
    return N;
}

void stampa_vet(int *a, int N) {
    printf("\n[");
    for (int i=0; i<N; i++) {
        printf(" %d ", a[i]);
    }
    printf("]\n");
}

int majority(int *a, int N) {
    /* approccio ricorsivo, divido il vettori in sottovettori sx e dx,
     * fino a trovare sottovettore composto solo da 1 elemento (è sicuramente il maggioritario)
     * attraverso una chiamata ricorsiva alla funzione majority prima per il sottovettore sx poi per quello dx
     * decidendo man mano quale sia il maggioritario tra sx e dx*/
    int meta = N/2, majoritysx=0, majoritydx=0, occ_sx=0, occ_dx=0;
    if (N == 1) return a[0];

    majoritysx = majority(a, meta);
    majoritydx = majority(a + meta, N-meta);

    if (majoritysx == majoritydx) return majoritysx; //ritorno uno dei due è indifferente

    occ_sx = conta_occorrenze(a, N, majoritysx);
    occ_dx = conta_occorrenze(a, N, majoritydx);

    if (occ_sx > occ_dx) return majoritysx;
    if (occ_dx > occ_sx) return majoritydx;

    return -1; // non esiste elemento maggioritario
}

int conta_occorrenze(int *a, int N, int chiave) {
    int conta = 0;
    for (int i = 0; i < N; i++) {
        if (a[i] == chiave) conta++;
    }
    return conta;
}