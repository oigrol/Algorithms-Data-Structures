#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inv.h"
#include "pg.h"

static const int MAXLEN = 41;

typedef enum {
    load_pg,
    load_inv,
    add_pg,
    remove_pg,
    add_equipPg,
    remove_equipPg,
    print_stat,
    finish,
    comando_errato
} comando_e;

comando_e selectComando(void);
void menu(void);

int main(void) {
    menu();
    return 0;
}

comando_e selectComando(void) {
    comando_e comando = load_pg;
    char *scelta = calloc(MAXLEN, sizeof(char));
    if (scelta == NULL) {
        printf("Errore allocazione memoria scelta.\n");
        return finish;
    }

    char *cmd[] = {
        "carica_lista_personaggi",
        "carica_vettore_oggetti",
        "aggiungi_personaggio",
        "elimina_personaggio",
        "aggiungi_oggetto_equipaggiamento",
        "elimina_oggetto_equipaggiamento",
        "calcola_statistiche_personaggio",
        "termina"};

    printf("Inserire comando tra quelli elencati:\n");
    for (int i=0; i < finish+1; i++) {
        printf("- %s\n", cmd[i]);
    }
    scanf("%s", scelta);

    while ((strcmp(scelta, cmd[comando])) != 0) {
        if (comando+1 > finish) return comando_errato;
        comando++;
    }

    free(scelta);
    return comando;
}

void menu(void) {
    int termina = 0;
    comando_e comando;
    tabPg_t *tabPg = NULL;
    tabInv_t *tabInv = NULL;
    pg_t *pg;
    while (!termina) {
        comando = selectComando();
        switch(comando) {
            case finish: {
                printf("Hai terminato il programma!\n");
                termina = 1;
            } break;

            case load_pg: {
                tabPg = creaTabPg("../pg.txt");
                if (tabPg == NULL) {
                    printf("Errore caricamento personaggi.\n");
                    continue;
                }
                printTabPg(tabPg);
            } break;

            case load_inv: {
                tabInv = creaTabInv("../inventario.txt");
                if (tabInv == NULL) {
                    printf("Errore caricamento inventario.\n");
                    continue;
                }
                printTabInv(tabInv);
            } break;

            case add_pg: {
                addPg(tabPg);
                printTabPg(tabPg);
            } break;

            case remove_pg: {
                removePg(tabPg);
                printTabPg(tabPg);
            } break;

            case add_equipPg: {
                if ((pg = searchCode(tabPg))->codice != pgVoid()->codice) {
                    addEquipPg(pg, tabInv);
                }
                printf("Statistiche aggiornate personaggio con codice: %s\n", pg->codice);
                printStat(&pg->stat);
                printf("\n");
            } break;

            case remove_equipPg: {
                if ((pg = searchCode(tabPg))->codice != pgVoid()->codice) {
                    removeEquipPg(pg, tabInv);
                }
                printf("Statistiche aggiornate personaggio con codice: %s\n", pg->codice);
                printStat(&pg->stat);
                printf("\n");
            } break;

            case print_stat: {
                if ((pg = searchCode(tabPg))->codice != pgVoid()->codice) {
                    printf("Statistiche aggiornate personaggio con codice: %s\n", pg->codice);
                    printStat(&pg->stat);
                    printf("\n");
                }
            } break;
            case comando_errato: {
                printf("Comando errato!\n");
            } break;
        }
    }
}