//04-esempi di problemi di ricerca e ottimizzazione -> interruttori e lampadine
#include <stdio.h>
#include <stdlib.h>

int **leggiFile(FILE *in, int *n, int *m);

int main(void) {

    return 0;
}
//disposizioni ripetute LIS
void powerset(int pos, int *v, int *s, int k, int *ml, int *bs) {
    //v vettore delle scelte, s vettore delle soluzioni, dimensione k del raggruppamento/sottoinsieme che andrà da 1 a n nel for del main, ml = lunghezza soluzione ottima by pointer, bs vettore soluzione ottima
    int j, l=0, ris;

    //condizione di terminazione
    if (pos >= k) {
        for (j=0; j<k; j++) {
            //vedo quanti elementi compongono questo gruppo scandendo la soluzione e cercando se il valore corrispondere in s[j] non è 0 -> elemento preso -> aumenta lunghezza sottosequenza
            if (s[j] != 0) l++; //lunghezza sottosequenza corrente inizializzata a 0 conta
        }
        ris = check(v, k, s, l); //condizione di validità -> quella soluzione s che ha lunghezza l è valida -> ha tutti gli elementi che crescono in maniera strettamente crescente? 1 se valida / 0 se non valida
        if (ris == 1) {
            //soluzione valida
            //verifico se è ottima: se si diventa miglior soluzione altrimenti soluzione ottima non cambia
            if (l >= *ml) {
                for (j=0; j<k; j++) {
                    bs[j] = s[j];
                    *ml = l;
                }
            }
        }
        return; //la funzione non ritorna nulla ma salva in bs
    }

    //parte ricorsiva
    //non prendo l'elemento e ricorro sulla prossima decisione
    s[pos] = 0;
    powerset(pos+1, v, s, k, ml, bs);

    //prendo l'elemento (annullo scelta fatta prima) e ricorro sulla prossima decisione
    s[pos] = 1;
    powerset(pos+1, v, s, k, ml, bs);

    return;
}

int check(int *v, int k, int *s, int l) {
    int i=0, j, t, ok=1;
    //presi i valori, deve controllare che sia soddisfatta la condizione di monotonia crescente, ovvero che il valore del predecessore deve essere strettamente minore del valore del successore
    //individuo indice elemento e indice elemento successivo nella sottosequenza
    //se trovo almeno un caso in cui il valore del predecessore è >= del successore (non <) allora soluzione non valida -> ritorno fallimento
    for (t=0; t<l-1; t++) { //per tutti gli l-1 elementi della sottosequenza presi (l'ultimo elemento della sequenza no perchè devo confrontare l-1+successore(che è l)
        //individuo indici i e j della coppia di elementi che confrontiamo
        while ((s[i] == 0) && (i<k-1)) //s[i] == 0 valore corrente non preso nella sottosequenza
            i++; //indice predecessore -> elemento corrente
        j=i+1; //conto dall'elemento successivo in poi per trovare valore della sequenza preso
        while((s[j] == 0) && (j<k))
            j++; //successore
        if (v[i] >= v[j])
            ok=0; //condizione non soddisfatta -> soluzione non valida
        i=j; //successore diventa nuovo elemento corrente (predecessore) da cui partire
    }
    return ok;
}