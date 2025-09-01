#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inv.h"

static const int MAXLEN = 31;

tabInv_t *creaTabInv(char *filename) {
    tabInv_t *tabInv = (tabInv_t*)malloc(sizeof(tabInv_t));
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Errore apertura file %s", filename);
        return NULL;
    }

    fscanf(fp, "%d", &(tabInv->nInv));
    tabInv->vettInv = (inv_t*)malloc(tabInv->nInv * sizeof(inv_t));

    for (int i=0; i<tabInv->nInv; i++) {
        loadInv(fp, &tabInv->vettInv[i]);
    }
    fclose(fp);
    return tabInv;
}

void loadInv(FILE *fp, inv_t *inv) {
    char nome[MAXLEN], tipo[MAXLEN];
    fscanf(fp, "%s %s", nome, tipo);
    inv->nome = strdup(nome);
    inv->tipo = strdup(tipo);
    loadStat(fp, &inv->stat);
}

void loadStat(FILE *fp, stat_t *stat) {
    fscanf(fp, "%d %d %d %d %d %d",
        &stat->hp,
        &stat->mp,
        &stat->atk,
        &stat->def,
        &stat->mag,
        &stat->spr);
}

void printStat(stat_t *stat) {
    printf("hp: %d, mp: %d, atk: %d, def: %d, mag: %d, spr: %d ",
        stat->hp,
        stat->mp,
        stat->atk,
        stat->def,
        stat->mag,
        stat->spr);
}
void printInv(inv_t *inv) {
    printf("%s %s ", inv->nome, inv->tipo);
    printStat(&inv->stat);
    printf("\n");
}

void printTabInv(tabInv_t *tabInv) {
    for (int i=0; i<tabInv->nInv; i++) {
        printf("Oggetto n.%d: ", i+1);
        printInv(&tabInv->vettInv[i]);
    }
    printf("\n");
}

void freeInv(tabInv_t *tabInv) {
    for (int i=0; i<tabInv->nInv; i++) {
        freeObj(&tabInv->vettInv[i]);
    }
    free(tabInv->vettInv);
    free(tabInv);
}

void freeObj(inv_t *obj) {
    free(obj->nome);
    free(obj->tipo);
    free(obj);
}