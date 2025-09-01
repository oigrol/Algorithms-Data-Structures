#include <stdio.h>
#include <stdlib.h>
#define MAXLEN 101
#define MAXES 5
#define MAXDIAG 3

typedef enum {
    transizione, //0
    acrobatico_indietro, //1
    acrobatico_avanti //2
} tipo;

typedef enum {
    spalle, //0
    frontalmente //1
} direzione;

typedef enum {
    no_preceduto, //0 = iniziale
    si_preceduto //1
} requisito_prec;

typedef enum {
    si_seguito, //0
    no_seguito //1 = finale
} requisito_seg;

typedef struct {
    char nome[MAXLEN];
    tipo tipologia;
    direzione ingresso;
    direzione uscita;
    requisito_prec preceduto;
    requisito_seg seguito;
    float punteggio;
    int difficolta;
} acrobazia;

typedef struct {
    acrobazia esercizi[MAXES]; //vettore che contiene le varie acrobazie di ogni diagonale
    int num_esercizi;
    float punteggio;
    int difficolta;
} diagonale;

acrobazia *leggiFile(char *filename, int *N);
void powerset(int pos, acrobazia *A, int N, diagonale *D, diagonale *best_sol, float *best_punteggio, int DD, int DP, int diag_count);
void diagReset(diagonale *D, int d, int e);
acrobazia acrobaziaVoid();
int checkDiag(diagonale D, int max_elem, int DD);

void stampaSoluzione(diagonale *best_sol) {
    for (int i = 0; i < MAXDIAG; i++) {
        printf("Diagonale %d: ", i + 1);
        for (int j = 0; j < best_sol[i].num_esercizi; j++) {
            printf("%s ", best_sol[i].esercizi[j].nome);
        }
        printf("(Punteggio: %.2f, Difficolta: %d)\n", best_sol[i].punteggio, best_sol[i].difficolta);
    }
}

int main(void) {
    acrobazia *A;
    diagonale D[MAXDIAG];
    diagonale best_D[MAXDIAG];
    float best_punteggio=0.0;
    int num_acrobazie=0, k = 5, DD, DP; //num_acrobazie max per ogni diagonale = dimensione max sottoinsieme
    A = leggiFile("../elementi.txt", &num_acrobazie);
    if (A == NULL) return -1;

    printf("Inserire difficolta max per ogni diagonale (DD): ");
    scanf("%d", &DD);
    printf("Inserire difficolta max del programma (DP): ");
    scanf("%d", &DP);

    diagReset(D, MAXDIAG, MAXES);
    diagReset(best_D, MAXDIAG, MAXES);
    powerset(0, A, num_acrobazie, D, best_D, &best_punteggio, DD, DP, 0);
    printf("\nMiglior programma trovato con punteggio totale %.2f:\n", best_punteggio);
    stampaSoluzione(best_D);

    free(A);
    return 0;
}

void diagReset(diagonale *D, int d, int e) {
    for (int i = 0; i < d; i++) {
        for (int j=0; j < e; j++) {
            D[i].esercizi[j] = acrobaziaVoid();
        }
        D[i].num_esercizi = 0;
        D[i].punteggio = 0;
        D[i].difficolta = 0;
    }
}

acrobazia acrobaziaVoid() {
    acrobazia A;
    A.nome[0] = '\0';
    A.tipologia = 0;
    A.ingresso = 0;
    A.uscita = 0;
    A.preceduto = 0;
    A.seguito = 0;
    A.punteggio = 0.0;
    A.difficolta = 0;
    return A;
}

acrobazia *leggiFile(char *filename, int *N) {
    acrobazia *A;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore apertura file.\n");
        exit(-1);
    }

    fscanf(fp, "%d", N);

    A = malloc(*N * sizeof(*A));

    //ruota 0 1 0 0 0 0.0 1
    for (int i=0; i<(*N); i++) {
        fscanf(fp, "%s %d %d %d %d %d %f %d",
            A[i].nome,
            &A[i].tipologia,
            &A[i].ingresso,
            &A[i].uscita,
            &A[i].preceduto,
            &A[i].seguito,
            &A[i].punteggio,
            &A[i].difficolta);
    }
    fclose(fp);
    return A;
}

float calcolaPunteggio(diagonale *D) {
    float punteggio_tot = 0;
    for (int i = 0; i < MAXDIAG; i++) {
        float punteggio = D[i].punteggio;
        if (i == 2) {
            for (int j = 0; j < D[i].num_esercizi; j++) {
                if (D[i].esercizi[j].difficolta >= 8) {
                    punteggio *= 1.5;
                    break;
                }
            }
        }
        punteggio_tot += punteggio;
    }
    return punteggio_tot;
}

void powerset(int pos, acrobazia *A, int N, diagonale *D, diagonale *best_sol, float *best_punteggio, int DD, int DP, int diag_count) {

    if (diag_count >= MAXDIAG) {
        float punteggio_tot = calcolaPunteggio(D);
        int difficolta_tot = 0;

        for (int i=0; i<MAXDIAG; i++) {
            difficolta_tot += D[i].difficolta;
        }

        if (difficolta_tot <= DP && punteggio_tot > *best_punteggio) {
            for (int i=0; i<MAXDIAG;i++)
                best_sol[i] = D[i];
            *best_punteggio = punteggio_tot;
        }
        return;
    }

    // Se non ci sono più elementi disponibili, passo alla diagonale successiva
    if (pos >= N) {
        if (D[diag_count].num_esercizi > 0) {
            powerset(0, A, N, D, best_sol, best_punteggio, DD, DP, diag_count + 1);
        }
        return;
    }

    D[diag_count].esercizi[D[diag_count].num_esercizi++] = A[pos];
    D[diag_count].punteggio += A[pos].punteggio;
    D[diag_count].difficolta += A[pos].difficolta;

    //controllo se diagonale è valida
    if (checkDiag(D[diag_count], MAXES, DD)) {
        powerset(pos+1, A, N, D, best_sol, best_punteggio, DD, DP, diag_count);
    }

    //backtrack
    D[diag_count].num_esercizi--;
    D[diag_count].punteggio -= A[pos].punteggio;
    D[diag_count].difficolta -= A[pos].difficolta;

    powerset(pos + 1, A, N, D, best_sol, best_punteggio, DD, DP, diag_count);

}

int checkDiag(diagonale D, int max_elem, int DD) {
    int acrobatico_avantiFlag = 0, acrobatico_indietroFlag = 0;

    if (D.difficolta > DD) return 0; //superata difficoltà max

    for (int i=0; i < D.num_esercizi; i++) {
        if (i == 0 && (D.esercizi[i].ingresso != frontalmente || D.esercizi[i].preceduto == si_preceduto)) return 0;

        if (i == D.num_esercizi - 1 && D.esercizi[i].seguito == no_seguito) return 0;

        if (i>0 && D.esercizi[i-1].uscita != D.esercizi[i].ingresso) return 0;

        if (D.esercizi[i].tipologia == acrobatico_avanti) acrobatico_avantiFlag=1;
        if (D.esercizi[i].tipologia == acrobatico_indietro) acrobatico_indietroFlag=1;

        if (!(acrobatico_avantiFlag || acrobatico_indietroFlag)) return 0;
    }
    return 1;
}