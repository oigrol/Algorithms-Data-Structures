#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXLEN 31
#define MAXCOM 8
#define MAXLENGTH 51

/*In questa variante sono usati vettori allocati dinamicamente*/

typedef enum comando_e {
    r_stampa,
    r_ordinaxdataeora,
    r_ordinaxcodice,
    r_ordinaxpartenza,
    r_ordinaxarrivo,
    r_ricercaxpartenza,
    r_acquisizione,
    r_fine,
} comando_e;

typedef struct data_t {
    int anno;
    int mese;
    int giorno;
} data_t;

typedef struct orario_t {
    int ore;
    int minuti;
    int secondi;
} orario_t;

typedef struct corse_t {
    char *codice_tratta;
    char *partenza;
    char *destinazione;
    data_t data;
    orario_t ora_partenza;
    orario_t ora_arrivo;
    int ritardo;
} corse_t;

int crea_tabella(corse_t **corse, char *filename);
comando_e leggiComando();
void malloc1dP(corse_t ***mat, int n);
void selezionaDati(corse_t *corse, int *N, comando_e comando);
void stampa_log(corse_t **corse, int N, int scelta);
void mergeSortRecursive(corse_t *A[], corse_t *B[], int l, int r, int (*compare)(corse_t*, corse_t*));
void mergeSort(corse_t *A[], corse_t *B[], int N, int (*compare)(corse_t*, corse_t*));
void merge(corse_t *A[], corse_t *B[], int l, int q, int r, int (*compare)(corse_t*, corse_t*));
int comparebydataeora(corse_t *A, corse_t *B);
int comparebycodice(corse_t *A, corse_t *B);
int comparebypartenza(corse_t *A, corse_t *B);
int comparebyarrivo(corse_t *A, corse_t *B);
void ricerca(corse_t *corse[], corse_t *B[], int N, int scelta);
void ricerca_lineare(corse_t *corse[], int N, char *prefisso);
void ricerca_dicotomica(corse_t *corse[], int l, int m, int r, char *prefisso);
void stampa_corsa(corse_t corsa);

//corse_t *A[] punta a un vettore di puntatori e ogni puntatore punta a una tratta (**A punta al primo elemento di un vettore di puntatori)

int main(void) {
    int num_tratte = 0;
    corse_t *corse = NULL;;
    comando_e comando = r_stampa;;


    num_tratte = crea_tabella(&corse, "../corse.txt");

    while (comando != r_fine) {
        comando = leggiComando();
        selezionaDati(corse, &num_tratte, comando);
    }

    for (int i = 0; i < num_tratte; i++) {
        free(corse[i].codice_tratta);
        free(corse[i].partenza);
        free(corse[i].destinazione);
    }
    free(corse);
    return 0;
}

void malloc1dP(corse_t ***mat, int n) {
    *mat = (corse_t **)malloc(n * sizeof(corse_t *)); //alloco vettore di puntatori
    if (*mat == NULL) exit(-1);
}

int crea_tabella(corse_t **corse, char *filename) {
    FILE *fp_in = fopen(filename, "r");
    int num_tratte = 0;

    if (fp_in == NULL) {
        printf("Errore apertura file\n");
        return -1;
    }

    fscanf(fp_in, "%d", &num_tratte);

    //alloca vettore
    *corse = (corse_t *)malloc(sizeof(corse_t ) * num_tratte);
    if (*corse == NULL) exit(-1);

    for (int i = 0; i < num_tratte; i++) {
        (*corse)[i].codice_tratta = (char *)malloc(MAXLEN * sizeof(char));
        (*corse)[i].partenza = (char *)malloc(MAXLEN * sizeof(char));
        (*corse)[i].destinazione = (char *)malloc(MAXLEN * sizeof(char));

        fscanf(fp_in, "%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d",
            (*corse)[i].codice_tratta,
            (*corse)[i].partenza,
            (*corse)[i].destinazione,
            &(*corse)[i].data.anno,
            &(*corse)[i].data.mese,
            &(*corse)[i].data.giorno,
            &(*corse)[i].ora_partenza.ore,
            &(*corse)[i].ora_partenza.minuti,
            &(*corse)[i].ora_partenza.secondi,
            &(*corse)[i].ora_arrivo.ore,
            &(*corse)[i].ora_arrivo.minuti,
            &(*corse)[i].ora_arrivo.secondi,
            &(*corse)[i].ritardo);
    }

    fclose(fp_in);
    return num_tratte;
}

comando_e leggiComando() {
    char cmd[MAXCOM][MAXLENGTH] = {"stampa",
                            "ordina per data e ora",
                            "ordina per codice di tratta",
                            "ordina per stazione di partenza",
                            "ordina per stazione di arrivo",
                            "ricerca per stazione di partenza",
                            "aggiorna tratte",
                            "fine"};
    char comando[MAXLENGTH];
    comando_e caso = r_stampa;
    printf("\nInserire comando da eseguire tra quelli elencati:\n- stampa\n- ordina per data e ora\n- ordina per codice di tratta\n- ordina per stazione di partenza\n- ordina per stazione di arrivo\n- ricerca per stazione di partenza\n- aggiorna tratte\n- fine\n");
    fgets(comando, MAXLENGTH, stdin); //evita il buffer
    comando[strcspn(comando, "\n")] = 0; //fgets include '\n' nella stringa letta, allora lo elimino per non causare problemi nel confronto con i comandi
    // strcspn restituisce l'indice in cui si trova \n in comando
    while ((caso < MAXCOM) && (strcmp(comando, cmd[caso])!=0)) {
        caso++;
    }
    return caso;
}

void selezionaDati(corse_t *corse, int *N, comando_e comando) {
    corse_t **perData = NULL, **perCodice = NULL, **perPartenza = NULL, **perArrivo = NULL, **B = NULL, **corse_original = NULL;
    malloc1dP(&B, *N);
    malloc1dP(&corse_original, *N);
    for (int i = 0; i < *N; i++) {
        B[i] = &corse[i];
        corse_original[i] = &corse[i];
    }

    switch (comando) {
        case r_stampa: {
            int su_file = -1;
            while (su_file != 0 && su_file != 1) {
                printf("Stampa a video [0] o stampa su file [1]:\n");
                scanf("%d", &su_file);
            }
            stampa_log(corse_original, *N, su_file);

            int c;
            while ((c = getchar()) != '\n' && c != EOF); // svuoto il buffer di input per evitare che rimanga qualche carattere \n lasciato dagli scanf che venga poi catturato da fgets e che dia problemi nel riconoscere il comando nella funzione leggiComandoù
            // infatti, usare sia lo scanf sia l'fgets (perchè gets non effettua controlli sulla lunghezza dell’input e può provocare overflow nel buffer), può portare a conflitti con il buffer

            break;
        }
        case r_ordinaxdataeora: {
            malloc1dP(&perData, *N);
            for (int i = 0; i < *N; i++) {
                perData[i] = &corse[i];
            }
            mergeSort(perData, B, *N, comparebydataeora);
            stampa_log(perData, *N, 0);
            free(perData);
            break;
        }
        case r_ordinaxcodice: {
            malloc1dP(&perCodice, *N);
            for (int i = 0; i < *N; i++) {
                perCodice[i] = &corse[i];
            }
            mergeSort(perCodice, B, *N, comparebycodice);
            stampa_log(perCodice, *N, 0);
            free(perCodice);
            break;
        }
        case r_ordinaxpartenza: {
            malloc1dP(&perPartenza, *N);
            for (int i = 0; i < *N; i++) {
                perPartenza[i] = &corse[i];
            }
            mergeSort(perPartenza, B, *N, comparebypartenza);
            stampa_log(perPartenza, *N, 0);
            free(perPartenza);
            break;
        }
        case r_ordinaxarrivo: {
            malloc1dP(&perArrivo, *N);
            for (int i = 0; i < *N; i++) {
                perArrivo[i] = &corse[i];
            }
            mergeSort(perArrivo, B, *N, comparebyarrivo);
            stampa_log(perArrivo, *N, 0);
            free(perArrivo);
            break;
        }
        case r_ricercaxpartenza: {
            int ricerca_dicotomica = -1;
            while (ricerca_dicotomica != 0 && ricerca_dicotomica != 1) {
                printf("Ricerca lineare [0] o ricerca dicotomica [1]:\n");
                scanf("%d", &ricerca_dicotomica);
            }
            ricerca(corse_original, B, *N, ricerca_dicotomica);

            int c;
            while ((c = getchar()) != '\n' && c != EOF); // svuoto il buffer di input per evitare che rimanga qualche carattere \n lasciato dagli scanf che venga poi catturato da fgets e che dia problemi nel riconoscere il comando nella funzione leggiComandoù
            // infatti, usare sia lo scanf sia l'fgets (perchè gets non effettua controlli sulla lunghezza dell’input e può provocare overflow nel buffer), può portare a conflitti con il buffer

            break;
        }
        case r_acquisizione: {
            char nomefile[MAXLEN];
            printf("Inserire nome file ('../nomefile.txt'):\n");
            scanf("%s", nomefile);

            free(corse); //libero memoria prima di allocare nuovi dati
            *N = crea_tabella(&corse, nomefile);

            int c;
            while ((c = getchar()) != '\n' && c != EOF); // svuoto il buffer di input per evitare che rimanga qualche carattere \n lasciato dagli scanf che venga poi catturato da fgets e che dia problemi nel riconoscere il comando nella funzione leggiComandoù
            // infatti, usare sia lo scanf sia l'fgets (perchè gets non effettua controlli sulla lunghezza dell’input e può provocare overflow nel buffer), può portare a conflitti con il buffer
            break;
        }
        case r_fine: break;
        default: printf("Comando errato.\n"); break;
    }
    free(corse_original);
    free(B);
}

void stampa_log(corse_t **corse, int N, int scelta) {
    if (scelta == 1) {
        FILE *fp_out = fopen("../stampa_corse.txt", "w");
        if (fp_out == NULL) {
            printf("Errore apertura file\n");
            return;
        }
        for (int i = 0; i < N; i++) {
            fprintf(fp_out, "%s %s %s %d/%d/%d %.2d:%.2d:%.2d %.2d:%.2d:%.2d %d\n",
            corse[i]->codice_tratta,
            corse[i]->partenza,
            corse[i]->destinazione,
            corse[i]->data.giorno,
            corse[i]->data.mese,
            corse[i]->data.anno,
            corse[i]->ora_partenza.ore,
            corse[i]->ora_partenza.minuti,
            corse[i]->ora_partenza.secondi,
            corse[i]->ora_arrivo.ore,
            corse[i]->ora_arrivo.minuti,
            corse[i]->ora_arrivo.secondi,
            corse[i]->ritardo);
        }
        fclose(fp_out);
    } else if (scelta == 0) {
        for (int i = 0; i < N; i++) {
            printf("%s %s %s %d/%d/%d %.2d:%.2d:%.2d %.2d:%.2d:%.2d %d\n",
            corse[i]->codice_tratta,
            corse[i]->partenza,
            corse[i]->destinazione,
            corse[i]->data.giorno,
            corse[i]->data.mese,
            corse[i]->data.anno,
            corse[i]->ora_partenza.ore,
            corse[i]->ora_partenza.minuti,
            corse[i]->ora_partenza.secondi,
            corse[i]->ora_arrivo.ore,
            corse[i]->ora_arrivo.minuti,
            corse[i]->ora_arrivo.secondi,
            corse[i]->ritardo);
        }
    }
}

void mergeSort(corse_t *A[], corse_t *B[], int N, int (*compare)(corse_t*, corse_t*)) {
    int l = 0, r = N-1;
    mergeSortRecursive(A, B, l, r, compare);
}

void mergeSortRecursive(corse_t *A[], corse_t *B[], int l, int r, int (*compare)(corse_t*, corse_t*)) {
    int q;
    if (l >= r) return; //ho finito di dividere il vettore
    q = (l+r) /2;
    mergeSortRecursive(A, B, l, q, compare);
    mergeSortRecursive(A, B, q+1, r, compare);
    merge(A, B, l, q, r, compare); //una volta che ho solo sottovettori di 1 elemento li fondo in modo ordinato
}

void merge (corse_t *A[], corse_t *B[], int l, int q, int r, int (*compare)(corse_t*, corse_t*)) {
    int i, j, k;
    i = l;
    j = q+1;
    for (k=l; k<=r; k++) {
        if (i > q) //esaurito sottovettore sx
            B[k] = A[j++]; //inserisco elementi sottovettore dx ordinato
        else if (j > r) //esaurito sottovettore dx
            B[k] = A[i++]; //inserisco elementi sottovettore sx ordinato
        else if (compare(A[i], A[j]) <= 0) //A[i] <= A[j], dove A[i] e A[j] puntano a due elementi diversi
            B[k] = A[i++];
        else
            B[k] = A[j++];
    }
    for (k = l; k <= r; k++)
        A[k] = B[k]; //A contiene i puntatori, che puntano agli elementi del vettore originale, in modo che siano ordinati
    return;
}

void ricerca(corse_t *corse[], corse_t *B[], int N, int scelta) {
    char prefisso[MAXLEN];
    printf("Inserire il prefisso della stazione di partenza desiderata:\n");
    scanf("%s", prefisso);

    if (scelta == 0)
        ricerca_lineare(corse, N, prefisso);
    else { //per utilizzare la ricerca dicotomica necessito di un vettore ordinato per stazione di partenza
        int l = 0, r = N-1;
        int m = (l+r)/2;
        mergeSort(corse, B, N, comparebypartenza); //ordina le tratte per stazione di partenza in ordine alfabetico
        ricerca_dicotomica(corse, l, m , r, prefisso);
    }
}

void ricerca_lineare(corse_t *corse[], int N, char *prefisso) {
    int trovato = 0;
    printf("I risultati che corrispondono alla tua ricerca sono:\n");
    for (int i=0; i<N; i++) {
        if (strncmp(corse[i]->partenza, prefisso, strlen(prefisso)) == 0) {
            stampa_corsa(*corse[i]);
            trovato = 1; //1 true | 0 false
        }
    }
    if (!trovato) printf("Nessuna corsa trovata con il prefisso %s\n", prefisso);
}

void ricerca_dicotomica(corse_t *corse[], int l, int m, int r, char *prefisso) {
    int trovato = 0;
    printf("I risultati che corrispondono alla tua ricerca sono:\n");
    while (l<=r && trovato == 0) {
        int i = m;
        while (i<=r && strncmp(corse[i]->partenza, prefisso, strlen(prefisso)) == 0) {
            stampa_corsa(*corse[i]);
            trovato = 1;
            i++;
        }
        i = m-1;
        while (i>=l && strncmp(corse[i]->partenza, prefisso, strlen(prefisso)) == 0) {
            stampa_corsa(*corse[i]);
            trovato = 1;
            i--;
        }
        if (strncmp(corse[m]->partenza, prefisso, strlen(prefisso)) > 0) { //se corse[m].partenza è maggiore di prefisso, allora devo trovare prefisso nell'intervallo sx
            r = m-1;
            m = (l+r)/2;
        }
        else if (strncmp(corse[m]->partenza, prefisso, strlen(prefisso)) < 0) { //se corse[m].partenza è minore di prefisso, allora devo trovare prefisso nell'intervallo dx
            l = m+1;
            m = (l+r)/2;
        }
    }
    if (!trovato) printf("Nessuna corsa trovata con il prefisso %s\n", prefisso);
}

void stampa_corsa(corse_t corsa) {
    printf("%s %s %s %d/%d/%d %.2d:%.2d:%.2d %.2d:%.2d:%.2d %d\n",
            corsa.codice_tratta,
            corsa.partenza,
            corsa.destinazione,
            corsa.data.giorno,
            corsa.data.mese,
            corsa.data.anno,
            corsa.ora_partenza.ore,
            corsa.ora_partenza.minuti,
            corsa.ora_partenza.secondi,
            corsa.ora_arrivo.ore,
            corsa.ora_arrivo.minuti,
            corsa.ora_arrivo.secondi,
            corsa.ritardo);
}

// queste funzioni compare prendono due puntatori e confrontano i campi delle strutture puntate
int comparebydataeora(corse_t *A, corse_t *B) {
    if (A->data.anno != B->data.anno)
        return A->data.anno - B->data.anno;
    if (A->data.mese != B->data.mese)
        return A->data.mese - B->data.mese;
    if (A->data.giorno != B->data.giorno)
        return A->data.giorno - B->data.giorno;
    if (A->ora_partenza.ore != B->ora_partenza.ore)
        return A->ora_partenza.ore - B->ora_partenza.ore;
    if (A->ora_partenza.minuti != B->ora_partenza.minuti)
        return A->ora_partenza.minuti - B->ora_partenza.minuti;
    return A->ora_partenza.secondi - B->ora_partenza.secondi;
}  // N.B: avrei potuto semplificare questa funzione considerando le date e gli orari come stringhe e applicando la strcmp

int comparebycodice(corse_t *A, corse_t *B) {
    return strcmp(A->codice_tratta, B->codice_tratta);
}

int comparebypartenza(corse_t *A, corse_t *B) {
    return strcmp(A->partenza, B->partenza);
}

int comparebyarrivo(corse_t *A, corse_t *B) {
    return strcmp(A->destinazione, B->destinazione);
}