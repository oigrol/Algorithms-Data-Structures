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
} pietre_t;

int crea_struct(FILE *fp, pietre_t *pietre_tot);
char converti(int id);
int disp_rip(int pos, pietre_t *pietre_disp, int *sol, int n, int k, int prec);
int check(int prec, int curr);
void stampa_struct(pietre_t *pietre_tot);
void stampa_collana(int *sol, int n);

int main(void) {
    int num_r, num_pietre_tot, *sol, len_sol=0;
    pietre_t pietre_tot[N]; //posso non allocarlo dinamicamente dato che ha dimensione fissa (stiamo usando 4 tipi di pietre)

    FILE *fp_in = fopen("../very_easy_test_set.txt", "r");
    if (fp_in == NULL) {
        printf("Errore apertura file.\n");
        return -1;
    }

    fscanf(fp_in, "%d", &num_r);

    for (int i=0; i<num_r; i++) {
        printf("\nCollana n. %d.\n", i+1);
        num_pietre_tot = crea_struct(fp_in, pietre_tot);
        stampa_struct(pietre_tot);

        sol = (int*)calloc(num_pietre_tot, sizeof(int)); //il vettore soluzione può avere lunghezza che varia da 1 a num_pietre_tot=(z+r+t+s) -> gli alloco la dimensione massima
        if (sol == NULL) {
            printf("Errore allocazione memoria sol");
            return -1;
        }

        len_sol = 0;

        for (int k=1; k<=num_pietre_tot; k++) {
            for (int j=0; j<N; j++)
                pietre_tot[j].disp = pietre_tot[j].n;
            if (disp_rip(0, pietre_tot, sol, N, k, -1)) {
                len_sol = k; //visito tutto lo spazio delle soluzioni (con pruining) fino a trovare il sottoinsieme di lunghezza k massima possibile
            }
        }

        /* (**)
         *Procedimento inverso (probabilmente più rapido, in quanto non genero tutte le soluzioni possibili, ma ne genero solo una valida e quella mi va bene): genero dalla collana più lunga alla più corta e esco in modo forzato dal ciclo appena ne trovo una valida (sicuramente è la più lunga = a dimensione k maggiore)
         *
         *avrei potuto non tener conto delle disponibilità, ma avere solo il campo n della struct
         *typedef struct pietre_s {
         *  int id; //identifica pietra
         *  int n; //pietre di id totali
         *} pietre_t;
         *
         *for (int k=num_pietre_tot; k>=1; k--) {
         *   if (disp_rip(0, pietre_tot, sol, N, k, -1)) {
         *       len_sol = k;
         *       break;
         *   }
         *}
         *
         * TUTTAVIA HO PREFERITO USARE IL PROCEDIMENTO NORMALE CHE GENERA TUTTE LE POSSIBILI SOLUZIONI STAMPANDO LA MIGLIORE SOLUZIONE TROVATA (partendo dall'inizio dello spazio delle soluzioni e non dalla fine dello spazio delle soluzioni)
         */

        if (len_sol == 0) {
            printf("Impossibile generare una collana.\n");
            free(sol);
            continue;
        }

        printf("La collana ha lunghezza %d.\n", len_sol);
        stampa_collana(sol, len_sol);
        //libero memoria per questo set di valori e passo al prossimo, fino alla fine del ciclo for
        free(sol);

    }
    fclose(fp_in);
    return 0;
}

int crea_struct(FILE *fp, pietre_t *pietre_tot) {
    int tot=0;
    for (int i=0; i<N; i++) {
        pietre_tot[i].id = i;
        fscanf(fp, "%d", &pietre_tot[i].n);
        tot += pietre_tot[i].n;
    }
    return tot;
}

void stampa_struct(pietre_t *pietre_tot) {
    printf("Pietre disponibili:\n");
    for (int i=0; i<N; i++) {
        printf("%c -> %d\n", converti(pietre_tot[i].id), pietre_tot[i].n);
    }
}

int disp_rip(int pos, pietre_t *pietre_disp, int *sol, int n, int k, int prec) {
    if (pos >= k) {
        //arrivato a questo punto, sono sicuro la mia soluzione sia corretta (ho applicato pruning)
        //ed è anche la miglior soluzione in quanto genero collane di lunghezza k (passato come parametro della funzione) sempre maggiore (1<=k<=z+r+t+s)
         return 1; //mantengo soluzione possibile
    }

    //se avessi seguito processo inverso illustrato nel commento sopra (**), avrei potuto iterare direttamente su pietre_disp[i].n, piuttosto che su pietre_disp[i].disp
    for (int i=0; i<n; i++) {
        if ((pietre_disp[i].disp > 0) && (check(prec, pietre_disp[i].id))) {
            pietre_disp[i].disp--;
            sol[pos] = pietre_disp[i].id;
            if (disp_rip(pos+1, pietre_disp, sol, n, k, sol[pos])) return 1;
            pietre_disp[i].disp++; //backtrack -> annullo ultima scelta, la rimetto in gioco e quindi ne incremento la disponibilità
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