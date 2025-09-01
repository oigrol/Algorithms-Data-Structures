#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXN 51
#define MAXCOM 8

typedef enum comando_e {
    r_inseriscixstdin,
    r_inseriscixfile,
    r_ricercaxcodice,
    r_cancellaxcodice,
    r_cancellaxdate,
    r_stampaxstdout,
    r_stampaxfile,
    r_fine
} comando_e;

typedef struct data_s {
    int dd;
    int mm;
    int aaaa;
} data_t;

typedef struct Item_s {
    char codice[6];
    char nome[MAXN];
    char cognome[MAXN];
    data_t data;
    char via[MAXN];
    char citta[MAXN];
    int cap;
} Item;

typedef struct node *link;
typedef struct node {
    Item val;
    link next;
} node_t;

comando_e trovaComando();
void menuComandi(comando_e comando, link *h);
link newNode(Item val, link next);
data_t prendiData(Item k);
int confrontaDate(data_t a, data_t b);
void stampaAnagrafica(link h, FILE *fp_out);
void stampaItem(FILE *fp_out, Item persona);
link inserimentoOrdinato(link h, Item k);
link inserisciAnagrafica(link h, FILE *fp_in);
Item ricercaCodice(link h, char *codice);
Item cancellaCodice(link *h, char *codice);
Item cancellaData(link *h, data_t in, data_t fin);
void freeMemory(link h);
int checkItem(Item k);
Item ItemVuoto();
Item leggiItem(FILE *fp_in);

//inizializzo lista vuota e poi farò operazioni
int main(void) {
    comando_e comando;
    link head = NULL; //inizializzo lista vuota

    do {
        comando = trovaComando();
        menuComandi(comando, &head);
    } while (comando != r_fine);

    freeMemory(head);
    head = NULL;
    return 0;
}

comando_e trovaComando() {
    char cmd[MAXCOM][MAXN] = {"Inserisci_nuova_persona_da_tastiera",
                        "Inserisci_nuova_persona_da_file",
                        "Ricerca_per_codice",
                        "Cancella_per_codice",
                        "Cancella_per_data",
                        "Stampa_a_video",
                        "Stampa_su_file",
                        "Fine"};
    char comando[MAXN];
    comando_e caso = r_inseriscixstdin;

    printf("\nInserire comando da eseguire tra quelli elencati:\n"
           "- Inserisci_nuova_persona_da_tastiera\n"
           "- Inserisci_nuova_persona_da_file\n"
           "- Ricerca_per_codice\n"
           "- Cancella_per_codice\n"
           "- Cancella_per_data\n"
           "- Stampa_a_video\n"
           "- Stampa_su_file\n"
           "- Fine\n");
    scanf("%s", comando);

    while((caso < MAXCOM) && (strcmp(comando, cmd[caso]) != 0)) {
        caso++;
    }
    return caso;
}

void menuComandi(comando_e comando, link *h) {

    switch(comando) {
        case r_inseriscixstdin: {
            *h = inserisciAnagrafica(*h, stdin);
        } break;
        case r_inseriscixfile: {
            char filename[MAXN];
            printf("\nInserire nome file (es: '../nomefile.txt'):\n");
            scanf("%s", filename);
            FILE *fp_in = fopen(filename, "r");
            if(fp_in == NULL) {
                printf("Errore nell'apertura del file %s\n ", filename);
                return;
            }
            *h = inserisciAnagrafica(*h, fp_in);
            fclose(fp_in);
        } break;
        case r_ricercaxcodice: {
            char code[6];
            Item trovato;
            printf("\nInserire codice da cercare nell'anagrafica (AXXXX):\n");
            scanf("%s", code);
            trovato = ricercaCodice(*h, code);
            if (checkItem(trovato) != 0) {
                printf("Non è stato trovato alcun risultato per la ricerca richiesta!\n");
                return;
            }
            printf("La persona cercata e':\n");
            stampaItem(stdout, trovato);
        } break;
        case r_cancellaxcodice: {
            char code[6];
            Item el_cancellato;
            printf("\nInserire codice relativo alla persona da cancellare dall'anagrafica (AXXXX):\n");
            scanf("%s", code);
            el_cancellato = cancellaCodice(h, code);
            if (checkItem(el_cancellato) != 0) {
                printf("Non e' stato trovato alcun risultato per la richiesta di cancellazione!\n");
                return;
            }
            printf("La persona cancellata dall'anagrafica e':\n");
            stampaItem(stdout, el_cancellato);
        } break;
        case r_cancellaxdate: {
            data_t data_in, data_fin;
            Item el_cancellato;
            printf("\nInserire intervallo di date relativo al periodo da cancellare dall'anagrafica (dd/mm/aaaa-dd/mm/aaaa):\n");
            scanf("%d/%d/%d-%d/%d/%d", &data_in.dd, &data_in.mm, &data_in.aaaa, &data_fin.dd, &data_fin.mm, &data_fin.aaaa);

            el_cancellato = cancellaData(h, data_in, data_fin);
            if (checkItem(el_cancellato) != 0) {
                printf("Non e' stato trovato alcun risultato per la richiesta di cancellazione!\n");
                return;
            }
            printf("La persona cancellata dall'anagrafica e':\n");
            stampaItem(stdout, el_cancellato);
            while (checkItem(el_cancellato) == 0) {
                el_cancellato = cancellaData(h, data_in, data_fin);
                if (checkItem(el_cancellato) != 0) {
                    return;
                }
                printf("La persona cancellata dall'anagrafica e':\n");
                stampaItem(stdout, el_cancellato);
            }
        } break;
        case r_stampaxstdout: {
            stampaAnagrafica(*h, stdout);
        } break;
        case r_stampaxfile: {
            char filename[MAXN];
            printf("\nInserire nome file (es: '../nomefile.txt'):\n");
            scanf("%s", filename);
            FILE *fp_out = fopen(filename, "w");
            if(fp_out == NULL) {
                printf("Errore nell'apertura del file %s\n ", filename);
                return;
            }
            stampaAnagrafica(*h, fp_out);
            fclose(fp_out);
        } break;
        case r_fine: break;
        default: {
            printf("\nComando errato!\n");
        }break;
    }
}

link newNode(Item val, link next) {
    link x = malloc(sizeof(*x));
    if (x == NULL) { //controllo errore
        printf("Errore allocazione memoria x.");
        return NULL;
    }
    x->val = val;
    x->next = next;
    return x; //ritorno puntatore a nodo allocato
}

data_t prendiData(Item k) {
    return k.data;
}

int confrontaDate(data_t a, data_t b) {
    if (a.aaaa != b.aaaa) return (a.aaaa - b.aaaa);
    if (a.mm != b.mm) return (a.mm - b.mm);
    if (a.dd != b.dd) return (a.dd - b.dd);
    return 0;
}

void stampaAnagrafica(link h, FILE *fp_out) {
    link x;
    if (h == NULL) {
        printf("Al momento l'anagrafica non contiene alcuna informazione!\n");
        return;
    }
    printf("\nCODICE NOME COGNOME NASCITA VIA       CITTA' CAP\n");
    for (x=h; x != NULL; x=x->next) {
        stampaItem(fp_out, x->val);
    }
}

void stampaItem(FILE *fp_out, Item persona) {
    fprintf(fp_out, "%s %s %s %d/%d/%d %s %s %d\n", persona.codice, persona.nome, persona.cognome, persona.data.dd, persona.data.mm, persona.data.aaaa, persona.via, persona.citta, persona.cap);
}

link inserimentoOrdinato(link h, Item k) { //lista anagrafica di Item va ordinata per data di nascita (più giovani appaiono prima)
    link x, p; //p sarà il predecessore e x il successore del nuovo nodo
    if (h == NULL || confrontaDate(prendiData(h->val), prendiData(k)) > 0) { //se la lista è vuota o il nuovo elemento viene prima del primo elemento inserisco il nuovo nodo in testa
        return newNode(k, h); //ritorna il nuovo nodo creato come head
    }
    for (x=h->next, p=h; x!=NULL && confrontaDate(prendiData(k), prendiData(x->val)) > 0; p=x, x=x->next); //prosegui finchè il valore da inserire è più grande del nodo x, prima di cui (trovato quando nuovo valore non è più maggiore) dobbiamo inserire il nuovo valore per far sì che x sia il suo successore e p il suo predecessore
    p->next = newNode(k, x);
    return h; //ritorna il vecchio head, dato che, con l'inserimento in mezzo, l'head non è cambiato
}

link inserisciAnagrafica(link h, FILE *fp_in) {
    Item persona;
    if (fp_in == stdin) {
        printf("\nInserire i dati della persona da aggiungere all'anagrafica nel seguente modo\n"
               "<codice> <nome> <cognome> <data_di_nascita> <via> <citta'> <cap>\n"
               "(es: 'A0001 Mario Rossi 01/02/1990 ViaMarcoPolo Torino 10129')\n");
        persona = leggiItem(fp_in);
        if (checkItem(persona) == 0) //se valido lo inserisco in Anagrafica
            h = inserimentoOrdinato(h, persona);
        return h;
    }

    persona = leggiItem(fp_in);
    while (checkItem(persona) == 0) {
        h = inserimentoOrdinato(h, persona);
        persona = leggiItem(fp_in);
    }
    return h;
}

int confrontaCodice(char *a, char *b) {
    return strcmp(a, b);
}

Item ricercaCodice(link h, char *codice) {
    link x;
    for (x=h; x != NULL; x = x->next) {
        if (confrontaCodice(codice, x->val.codice) == 0)
            return x->val;
    }
    return ItemVuoto(); //se non trovo il codice cercato ritorno NULL
}

Item cancellaCodice(link *h, char *codice) {
    Item persona;
    link x, p;

    for (x=*h, p=NULL; x!=NULL; p=x, x = x->next) {
        if (confrontaCodice(codice, (x->val).codice) == 0) {
            /*quando trovo il nodo, se il nodo da cancellare è il primo faccio bypass da head*/
            if (x == *h) {
                *h = x->next;
            } /*se il nodo non è il primo faccio il bypass dal predecessore*/
            else {
                p->next = x->next;
            }
            persona = x->val; //salvo il valore prima di cancellare il nodo bypassato
            /*dopo il bypasso libero il nodo puntato da x e esco dal ciclo*/
            free(x);
            return persona;
        }
    }
    return ItemVuoto();
}

Item cancellaData(link *h, data_t in, data_t fin) {
    Item persona;
    link x, p;

    for (x=*h, p=NULL; x!=NULL && (confrontaDate(fin, prendiData(x->val)) >= 0); p=x, x = x->next) { //essendo lista ordinata, finchè la data finale è più grande della data corrente possiamo avanzare. Se data finale non è + >= data corrente allora non potremo più trovarlo nel resto della lista -> siamo usciti da intervallo
        if (confrontaDate(in, prendiData(x->val)) <= 0 && (confrontaDate(fin, prendiData(x->val)) >= 0)) { //se mi trovo all'interno dell'intervallo delle date
            /*quando trovo il nodo, se il nodo da cancellare è il primo faccio bypass da head*/
            if (x == *h) {
                *h = x->next;
            } /*se il nodo non è il primo faccio il bypass dal predecessore*/
            else {
                p->next = x->next;
            }
            persona = x->val; //salvo il valore prima di cancellare il nodo bypassato
            /*dopo il bypass esco dal ciclo*/
            free(x);
            return persona;
        }
    }
    return ItemVuoto();
}

void freeMemory(link h) {
    link x, t;

    for(x = h; x != NULL; x=t) { //libero ogni nodo a partire da head fino alla coda
        t = x->next; //mi salvo nodo successivo a quello da eliminare
        free(x);
    }
}

int checkItem(Item k) { //verifica se la nuova persona da inserire in anagrafica ha dati validi o è un Item vuoto
    if (strlen(k.codice) == 0) return 1; // Item vuoto
    return 0; //Item corretto -> da inserire in anagrafica
}

Item ItemVuoto() {
    Item vuoto = {"", "", "", {0, 0, 0}, "", "", 0};
    return vuoto;
}

Item leggiItem(FILE *fp_in) {
    Item persona;
    if (fscanf(fp_in, "%s %s %s %d/%d/%d %s %s %d", persona.codice, persona.nome, persona.cognome, &persona.data.dd, &persona.data.mm, &persona.data.aaaa, persona.via, persona.citta, &persona.cap) == 9) {
        return persona;
    }
    return ItemVuoto();
}