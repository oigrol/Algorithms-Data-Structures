#ifndef _PG
#define _PG

#include "inv.h"

typedef struct tapEquip_s {
    int inUso;
    inv_t **vettEquip; //vettore di puntatori a oggetti dell'inventario
} tabEquip_t;

typedef struct pg_s {
    char *codice;
    char *nome;
    char *classe;
    tabEquip_t *equip;
    stat_t stat, stat_init;
} pg_t;

typedef struct nodoPg_s *linkPg;
typedef struct nodoPg_s {
    pg_t pg;
    linkPg next;
} nodoPg_t;

typedef struct tabPg_s {
    linkPg headPg, tailPg;
    int nPg;
} tabPg_t;

tabPg_t* creaTabPg(char *filename);
int loadPg(FILE *fp, pg_t *pg);
linkPg newNode(pg_t pg, linkPg next);
void listInsert(tabPg_t *tabPg, pg_t pg);
pg_t *searchCode(tabPg_t *tabPg);
void addPg(tabPg_t *tabPg);
void removePg(tabPg_t *tabPg);
void addEquipPg(pg_t *pg, tabInv_t *tabInv);
void removeEquipPg(pg_t *pg, tabInv_t *tabInv);
void upgradeStat(pg_t *pg);
void printPg(pg_t *pg);
void printTabPg(tabPg_t *tabPg);
void freePg(pg_t *pg);
void freeEquip(tabEquip_t *equip);
pg_t *pgVoid();

#endif