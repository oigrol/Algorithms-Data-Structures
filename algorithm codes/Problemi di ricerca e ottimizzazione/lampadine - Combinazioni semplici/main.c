//04-esempi di problemi di ricerca e ottimizzazione -> interruttori e lampadine
#include <stdio.h>
#include <stdlib.h>

int powerset(int pos, int *sol, int n, int k, int start, int **interruttori, int m);
int verifica(int **interruttori, int m, int k, int *sol);
int **leggiFile(FILE *in, int *n, int *m);
void stampa(int k, int *sol);

int main(void) {
    int n, m, k, i, trovato=0; //n=interruttori, m=lampadine, k=dimensione sottoinsiemi interruttori presi, trovato=flag che ci permette di bloccare la ricerca ricorsiva appena trovata una soluzione
    FILE *fp = fopen("../switches.txt", "r");
    int **interruttori = leggiFile(fp, &n, &m); //ci dice quante righe(n)=interruttori e quante colonne(m)=lampadine e crea matrice dove nella cella (i,j) troviamo se la lampadina j è controllata dall'interruttore i (1) o no(0)
    int *sol = calloc(m, sizeof(int)); //alloco vettore soluzione che mi dice per ogni cella i-esima se la lampadina i è accesa(1) -> se il numero di interruttori che attivano la lampadina è dispari o spenta(0) -> se il numero di interruttori è pari per tutte le m lampadine
    //se tutte le m lampadine e quindi tutti i campi del vettore sol sono = 1 allora tutte le lampadine sono accese -> soluzione accettabile

    printf("Powerset mediante combinazioni semplici.\n");
    for (k=1; (k <= n) && (trovato == 0); k++) { //escludo k=0 ovvero insieme vuoto
        if (powerset(0, sol, n, k, 0, interruttori, m))
            trovato = 1;
    }

    free(sol);
    for (i=0; i<n;i++) {
        free(interruttori[i]);
    }
    free(interruttori);

    return 0;
}

int **leggiFile(FILE *in, int *n, int *m) {
    int i, j, **mat;
    fscanf(in, "%d %d", n, m);
    mat = calloc(*n, sizeof(int *));

    for (i=0; i<*n; i++) {
        mat[i] = calloc(*m, sizeof(int));
        for (j=0; j<*m; j++)
            fscanf(in, "%d", &mat[i][j]);
    }
    return mat;
}

int powerset(int pos, int *sol, int n, int k, int start, int **interruttori, int m) { //n=dimensione vettore soluzione=numero lampadine, k=dimensione sottoinsieme interruttori
    int i;
    //condizione di terminazione
    if (pos >= k) {
        if (verifica(interruttori, m, k, sol)) { //verifico che la soluzione sia valida, ovvero che in base al numero di interruttori accesi(vedi dove sol[pos]!=0) questo numero sia dispari per ogni lampadina -> se esiste almeno una lampadina spenta soluzione non accettabile
            printf("Insieme minimo di interruttori per accendere  tutte le lampadine:\n");
            stampa(k, sol); //se la soluzione è valida la stampo e
            return 1; //ritorno successo
        }
        return 0; //soluzione non valida? procederò con la ricerca nel ciclo for
    }

    //ricorsione
    for (i=start; i<n; i++) {
        sol[pos]=i; //ho forzato ordine con start, che rappresenta interruttore da cui parto a premere fino all'n-esimo interruttore
        //sol[pos] contiene l'indice dell'interruttore se premuto o 0 se non premuto
        if (powerset(pos+1, sol, n, k, i+1, interruttori, m))
            return 1; //se riesco a trovare una soluzione
    }
    return 0; //insuccesso se non riesco a trovare soluzione nella ricorsione
}

void stampa(int k, int *sol) {
    for (int i=0; i < k; i++)
        printf("%d ", sol[i]);
    printf("\n");
}

int verifica(int **interruttori, int m, int k, int *sol) {
    //m=num lampadine
    int i, j, ok=1, *lampadine;
    lampadine = calloc(m, sizeof(int)); //registro i conteggi di interruttori accesi per ogni lampadina nella cella i

    //flag ok appena diventa = 0 perchè una lampadina risulta spenta (ha interruttori pari) -> esco perchè non è una soluzione valida
    for (j=0; j<m && ok; j++) { //itero su lampadine
        for (i=0; i<k; i++) { //itero su interruttori presi nel sottoinsieme
            lampadine[j] += interruttori[sol[i]][j]; //sol[i] mi da solo gli interruttori che modificano la lampadina j
            //conta quanti interruttori del sottoinsieme la controllano //interruttori[sol[i]][j] può essere 0(non controlla) o 1(controlla)
        }
        if (lampadine[j]%2 == 0)
            ok=0; //numero interruttori dispari -> lampadina spenta
        //se non sono mai entrato nell'if allora tutti gli interruttori per ogni lampadina erano dispari -> tutte le lampadine accese
    }
    free(lampadine);
    return ok;
}

/*int verifica(int **inter, int m, int k, int *sol) {
    int i, j, ok = 1, *lampadine;
    lampadine = calloc(m, sizeof(int));
    for (j=0; j<m && ok; j++) {
        for (i=0; i<k; i++)
            lampadine[j] ^= inter[sol[i]][j];
    if (lampadine[j]==0
        ok = 0;
    }
    free(lampadine);
    return ok;
}*/