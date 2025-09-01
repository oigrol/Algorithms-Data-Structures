#include <stdio.h>
#include <stdlib.h>
#define NOMEFILE "../mat.txt"

void malloc2dP(int ***mat, int *nr, int *nc);
void separa(int **mat, int nr, int nc, int **bianchi, int **neri);
void free2d(int **mat, int nr);

int main(void) {
    int nr, nc, **mat, *v_bianchi, *v_neri;
    malloc2dP(&mat, &nr, &nc); //alloca e crea matrice da file

    separa(mat, nr, nc, &v_bianchi, &v_neri); //alloco e creo due vettori: uno per le celle bianche, uno per le nere con i relativi valori

    //de-alloco e libero memoria
    free2d(mat, nr);
    return 0;
}

void malloc2dP(int ***mat, int *nr, int *nc) { //restituisce il puntatore alla matrice tra i parametri passati per riferimento
    int **m; //variabile locale
    FILE *fp_in = fopen(NOMEFILE, "r");
    if (fp_in == NULL) {
        printf("Errore apertura file\n");
        exit(1);
    }

    fscanf(fp_in, "%d %d", nr, nc); //num. righe e num. colonne

    m = (int **)malloc(*nr * sizeof(int *)); //alloca vettore che contiene puntatori ad altri vettori
    if (m == NULL) exit(1);

    for (int i = 0; i < *nr; i++) {
        m[i] = (int *)malloc(*nc * sizeof(int)); //alloca vettori puntati che contengono interi
        if (m[i] == NULL) exit(1);

        for (int j = 0; j < *nc; j++) {
            fscanf(fp_in, "%d", &m[i][j]);
        }
    }
    *mat = m;
    fclose(fp_in);
}

void separa(int **mat, int nr, int nc, int **bianchi, int **neri) {
    int k = 0, celle_tot, len_bianchi, len_neri;

    celle_tot = nr * nc;
    len_bianchi = (celle_tot+1) / 2; //definisco dimensione vettore: celle bianche sono una in più rispetto alle nere
    len_neri = celle_tot/2; //celle nere

    (*bianchi) = (int *)malloc(len_bianchi * sizeof(int));
    if (*bianchi == NULL) exit(1);
    (*neri) = (int *)malloc(len_neri * sizeof(int));
    if (*neri == NULL) exit(1);

    for (int i = 0; i < nr; i++) {
        for (int j = 0; j < nc; j++) {
            if ((i+j)%2 == 0) { //celle bianche
                (*bianchi)[k] = mat[i][j];
            }
            else { //celle nere
                (*neri)[k] = mat[i][j];
                k++;
            }
        }
    }

    printf("Stampo celle bianche:\n[");
    for (int i = 0; i < len_bianchi; i++) {
        printf(" %d ", (*bianchi)[i]);
    }
    printf("]\nStampo celle nere:\n[");
    for (int i = 0; i < len_neri; i++) { //le celle nere sono una in meno rispetto alle celle bianche
        printf(" %d ", (*neri)[i]);
    }
    printf("]\n");
    free(*bianchi);
    free(*neri);
}

void free2d(int **mat, int nr) {
    for (int i = 0; i < nr; i++) {
        free(mat[i]);
    }
    free(mat);
}