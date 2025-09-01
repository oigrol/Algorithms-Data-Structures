#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pg.h"

static const int MAXLEN = 31;
static const int MAXEQ = 8;

tabPg_t* creaTabPg(char *filename) {
    pg_t pg;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore apertura file %s", filename);
        return NULL;
    }
    tabPg_t *tabPg = (tabPg_t*)malloc(sizeof(tabPg_t));
    if (tabPg == NULL) {
        printf("Errore allocazione memoria *tabPg");
        return NULL;
    }
    tabPg->headPg = tabPg->tailPg = NULL; //inizializzo puntatore a lista vuota
    tabPg->nPg = 0;

    while (loadPg(fp, &pg))
        listInsert(tabPg, pg); //inserisce la struct del personaggio in lista

    fclose(fp);
    return tabPg;
}

int loadPg(FILE *fp, pg_t *pg) {
    char codice[MAXLEN], nome[MAXLEN], classe[MAXLEN];
    if (pg == NULL)
        return 0;

    if (fscanf(fp, "%s %s %s",
        codice, nome, classe) == 3) {
        pg->codice = strdup(codice);
        pg->nome = strdup(nome);
        pg->classe = strdup(classe);
        loadStat(fp, &pg->stat);
        pg->stat_init = pg->stat;
        pg->equip = (tabEquip_t*)calloc(1, sizeof(tabEquip_t));
        pg->equip->vettEquip = (inv_t**)calloc(MAXEQ, sizeof(inv_t*));
        if (pg->equip == NULL || pg->equip->vettEquip == NULL) {
            printf("Errore allocazione equip");
            return -1;
        }
        pg->equip->inUso = 0;
        return 1;
    }
    return 0;
}

linkPg newNode(pg_t pg, linkPg next) {
    linkPg node = (linkPg)malloc(sizeof(nodoPg_t));
    if (node == NULL) {
        return NULL;
    }
    node->pg = pg;
    node->next = next;
    return node;
}

void listInsert(tabPg_t *tabPg, pg_t pg) {
    if (tabPg->headPg == NULL) { //lista vuota
        tabPg->headPg = tabPg->tailPg = newNode(pg, NULL);
    } else {
        tabPg->tailPg->next = newNode(pg, NULL); //crea nuovo nodo dopo l'ultimo in lista, a cui è linkato
        tabPg->tailPg = tabPg->tailPg->next; //il nodo appena creato diventa il nuovo ultimo elemento della lista
    }
    tabPg->nPg++;
}

void addPg(tabPg_t *tabPg) {
    pg_t pg;

    printf("Inserire nuovo personaggio:\n"
           "Codice Nome Classe Hp Mp Atk Def Mag Spr:\n");
    if (loadPg(stdin, &pg)) {
        listInsert(tabPg, pg);
    }
}

void removePg(tabPg_t *tabPg) {
    linkPg x, p;
    char code[MAXLEN];

    printf("Inserire codice personaggio da eliminare:\n");
    scanf("%s", code);

    if (tabPg->headPg == NULL) {
        printf("Non ci sono personaggi da eliminare\n");
        return;
    }
    for (x=tabPg->headPg, p=NULL; x!=NULL; p=x, x=x->next) {
        if (strcmp(x->pg.codice, code) == 0) {
            if (x==tabPg->headPg) {
                tabPg->headPg = x->next;
            } else if (x==tabPg->tailPg) {
                tabPg->tailPg = p;
                p->next = NULL;
            } else {
                p->next = x->next;
            }
            freePg(&x->pg);
            free(x);
            return;
        }
    }
    printf("Errore rimozione personaggio\n");
}

pg_t *searchCode(tabPg_t *tabPg) {
    linkPg x = NULL;
    char code[MAXLEN];

    printf("Inserire codice personaggio da cercare: ");
    scanf("%s", code);

    for (x=tabPg->headPg; x!=NULL; x=x->next) {
        if (strcmp(x->pg.codice, code) == 0) {
            return &x->pg;
        }
    }
    return NULL;
}

pg_t *pgVoid() {
    pg_t *pg = (pg_t*)malloc(sizeof(pg_t));
    pg->codice = NULL;
    pg->nome = NULL;
    pg->classe = NULL;
    return pg;
}

void printPg(pg_t *pg) {
    printf("%s %s %s ", pg->codice, pg->nome, pg->classe);
    printStat(&pg->stat);
    printf("Equipaggiamento in uso: %d", pg->equip->inUso);
    printf("\n");
}

void printTabPg(tabPg_t *tabPg) {
    linkPg x = NULL;
    int i=0;
    for (x=tabPg->headPg; x!=NULL; x=x->next) {
        printf("Personaggio n.%d: ", i+1);
        printPg(&x->pg);
        i++;
    }
    printf("\n");
}

void addEquipPg(pg_t *pg, tabInv_t *tabInv) {
    int aggiunto = 0;
    if (pg->equip->inUso >= MAXEQ) {
        printf("Hai raggiunto il massimo di oggetti equipaggiati\n");
        return;
    }
    char obj[MAXLEN];
    printf("Inserire oggetto da equipaggiare:\n");
    scanf("%s", obj);
    for (int i = 0; i < tabInv->nInv && !aggiunto; i++) {
        if (strcmp(tabInv->vettInv[i].nome, obj) == 0) {
            // Verifica se l'oggetto è già presente nell'equipaggiamento
            int giaEquip = 0;
            for (int j = 0; j < pg->equip->inUso; j++) {
                if (pg->equip->vettEquip[j] == &tabInv->vettInv[i]) {
                    giaEquip = 1;
                    break;
                }
            }

            if (!giaEquip) {
                pg->equip->vettEquip[pg->equip->inUso++] = &tabInv->vettInv[i];
                aggiunto = 1;
            } else {
                printf("Oggetto già equipaggiato\n");
            }
        }
    }
    if (aggiunto) {
        upgradeStat(pg);
    }
}

void removeEquipPg(pg_t *pg, tabInv_t *tabInv) {
    int rimosso = 0;
    if (pg->equip->inUso <= 0) {
        printf("Non ci sono oggetti da rimuovere\n");
        return;
    }

    char obj[MAXLEN];
    printf("Inserire oggetto da rimuovere: ");
    scanf("%s", obj);

    for (int i = 0; i < pg->equip->inUso; i++) {
        if (strcmp(pg->equip->vettEquip[i]->nome, obj) == 0) {
            for (int j = i; j < pg->equip->inUso - 1; j++) {
                pg->equip->vettEquip[j] = pg->equip->vettEquip[j + 1]; //sposto tutti gli oggetti dopo quello rimosso, sovrascrivendone la posizione
            }
            pg->equip->inUso--;
            rimosso = 1;
        }
    }
    if (rimosso)
        upgradeStat(pg);
}

void upgradeStat(pg_t *pg) {
    pg->stat = pg->stat_init;
    for (int i=0; i<pg->equip->inUso; i++) {
        pg->stat.hp += pg->equip->vettEquip[i]->stat.hp;
        pg->stat.mp += pg->equip->vettEquip[i]->stat.mp;
        pg->stat.atk += pg->equip->vettEquip[i]->stat.atk;
        pg->stat.def += pg->equip->vettEquip[i]->stat.def;
        pg->stat.mag += pg->equip->vettEquip[i]->stat.mag;
        pg->stat.spr += pg->equip->vettEquip[i]->stat.spr;
    }

    //se la statistica è inferiore a 0, la pongo uguale a 0
    if (pg->stat.hp < 0) pg->stat.hp = 0;
    if (pg->stat.mp < 0) pg->stat.mp = 0;
    if (pg->stat.def < 0) pg->stat.def = 0;
    if (pg->stat.mag < 0) pg->stat.mag = 0;
    if (pg->stat.spr < 0) pg->stat.spr = 0;
}

void freePg(pg_t *pg) {
    free(pg->codice);
    free(pg->nome);
    free(pg->classe);
    freeEquip(pg->equip);
    free(pg);
}

void freeEquip(tabEquip_t *equip) {
    for (int i = 0; i < MAXEQ; i++) {
        free(equip->vettEquip[i]);
    }
    free(equip->vettEquip);
    free(equip);
}