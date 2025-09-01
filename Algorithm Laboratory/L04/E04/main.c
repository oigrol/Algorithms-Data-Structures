#include <stdio.h>
#include <stdlib.h>

typedef enum {
    Z, //0
    R, //1
    T, //2
    S  //3
} pietre_id;

#define N 4

typedef struct pietre_s {
    int id; //identifica pietra
    int n; //pietre di id totali
    int disp; //pietre di id disponibili per ogni ciclo
    int used;  // Pietre usate finora
    int val;   // Valore della pietra
    int rip;   // Ripetizioni consecutive
} pietre_t;

int crea_struct(FILE *fp, pietre_t *pietre_tot, int *max_rip);
char converti(int id);
int disp_rip(int pos, pietre_t *pietre_disp, int *sol, int val, int n, int k, int prec, int max_rip, int *best_sol, int *best_val);
int check(int prec, int curr);
void stampa_struct(pietre_t *pietre_tot, int max_rip);
void stampa_collana(int *sol, int n);

int main(void) {
    int num_r, num_pietre_tot=0, *sol, len_sol=0, max_rip=0, *best_sol, best_val;
    pietre_t pietre_tot[N]; //posso non allocarlo dinamicamente dato che ha dimensione fissa (stiamo usando 4 tipi di pietre)

    FILE *fp_in = fopen("../test_set.txt", "r");
    if (fp_in == NULL) {
        printf("Errore apertura file.\n");
        return -1;
    }

    fscanf(fp_in, "%d", &num_r);

    for (int i=0; i<num_r; i++) {
        printf("\nCollana n. %d.\n", i+1);
        num_pietre_tot = crea_struct(fp_in, pietre_tot, &max_rip);
        stampa_struct(pietre_tot, max_rip);

        sol = (int*)calloc(num_pietre_tot, sizeof(int)); //il vettore soluzione può avere lunghezza che varia da 1 a num_pietre_tot=(z+r+t+s) -> gli alloco la dimensione massima
        best_sol = (int *)calloc(num_pietre_tot, sizeof(int));
        if (sol == NULL || best_sol == NULL) {
            printf("Errore allocazione memoria vettori soluzione.");
            free(sol);
            free(best_sol);
            fclose(fp_in);
            return -1;
        }

        len_sol = 0;
        best_val = 0;
        for (int k=1; k<=num_pietre_tot; k++) {
            for (int j=0; j<N; j++) {
                pietre_tot[j].disp = pietre_tot[j].n;
                pietre_tot[j].used = 0;
                pietre_tot[j].rip = 0;
            }
            if (disp_rip(0, pietre_tot, sol, 0, N, k, -1, max_rip, best_sol, &best_val)) {
                len_sol = k; //visito tutto lo spazio delle soluzioni (con pruining) fino a trovare il sottoinsieme di lunghezza k massima possibile
            }
        }

        if (len_sol == 0) {
            printf("Impossibile generare una collana.\n");
            free(sol);
            free(best_sol);
            continue;
        }

        printf("La collana (soluzione ottima) ha lunghezza %d e valore %d.\n", len_sol, best_val);
        stampa_collana(best_sol, len_sol);
        //libero memoria per questo set di valori e passo al prossimo, fino alla fine del ciclo for
        free(sol);
        free(best_sol);
    }
    fclose(fp_in);
    return 0;
}

int crea_struct(FILE *fp, pietre_t *pietre_tot, int *max_rip) {
    int tot=0;
    for (int i=0; i<N; i++) {
        pietre_tot[i].id = i;
        fscanf(fp, "%d", &pietre_tot[i].n);
        tot += pietre_tot[i].n;
    }
    for (int i=0; i<N; i++) {
        fscanf(fp, "%d", &pietre_tot[i].val); //val_Z val_R val_T val_S
    }
    fscanf(fp, "%d", max_rip);
    return tot;
}

void stampa_struct(pietre_t *pietre_tot, int max_rip) {
    printf("Pietre disponibili:\n");
    for (int i = 0; i < N; i++) {
        printf("%c -> numero: %d, valore: %d\n", converti(pietre_tot[i].id), pietre_tot[i].n, pietre_tot[i].val);
    }
    printf("Ogni pietra puo' apparire consecutivamente nella collana al massimo %d volte.\n", max_rip);
}

int disp_rip(int pos, pietre_t *pietre_disp, int *sol, int val, int n, int k, int prec, int max_rip, int *best_sol, int *best_val) {
    if (pos >= k) {
        if (pietre_disp[Z].used <= pietre_disp[S].used) {
            if (val > *best_val) {
                *best_val = val;
                for (int i = 0; i < pos; i++) {
                    best_sol[i] = sol[i];
                }
                return 1;
            }
        }
        return 0;
    }

    for (int i=0; i<n; i++) {
        if (pietre_disp[i].disp > 0) {
            if (check(prec, pietre_disp[i].id)) {
                // ogni pietra non si può ripetere più di max_rip volte consecutivamente nella collana
                if (prec == pietre_disp[i].id) {
                    if (pietre_disp[i].rip +1 > max_rip) continue;
                }
                int prec_rip = pietre_disp[i].rip;
                if (prec == pietre_disp[i].id) pietre_disp[i].rip = prec_rip+1;
                else pietre_disp[i].rip = 1;

                //inserisco una pietra nella collana
                pietre_disp[i].disp--;
                pietre_disp[i].used++;
                val += pietre_disp[i].val;
                sol[pos] = pietre_disp[i].id;

                if (disp_rip(pos+1, pietre_disp, sol, val, n, k, sol[pos], max_rip, best_sol, best_val)) return 1;

                //se non è andato a buon fine faccio backtrack
                pietre_disp[i].disp++; //backtrack -> annullo ultima scelta, la rimetto in gioco e quindi ne incremento la disponibilità
                pietre_disp[i].used--;
                pietre_disp[i].rip = prec_rip;
                val -= pietre_disp[i].val;
            }
        }
    }
    return 0;
}

int check(int prec, int curr) {
    if (prec != -1) {
        if(((prec == Z) || (prec == T)) && (curr != Z) && (curr != R)) return 0; //false // zaffiro e topopazio devono essere seguiti immediatamente o da un zaffiro o da un rubino
        if(((prec == R) || (prec == S)) && (curr != S) && (curr != T)) return 0; //false // smeraldo e rubino devono essere seguiti immediatamente o da un smeraldo o da un topazio
    }
    return 1; //true
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

void stampa_collana(int *sol, int n) {
    printf("<-");
    for (int i=0; i<n; i++) {
        printf("%c-", converti(sol[i]));
    }
    printf(">\n");
}