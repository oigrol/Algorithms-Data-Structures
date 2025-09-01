#ifndef _INV
#define _INV

typedef struct stat_s {
    int hp, mp, atk, def, mag, spr;
} stat_t;

typedef struct inv_s {
    char *nome;
    char *tipo;
    stat_t stat;
} inv_t;

typedef struct tabInv_s {
    inv_t *vettInv; //puntatore al vettore di oggetti dell'inventario
    int nInv;
    int maxInv;
} tabInv_t;

tabInv_t *creaTabInv(char *filename);
void loadInv(FILE *fp, inv_t *inv);
void loadStat(FILE *fp, stat_t *stat);
void printStat(stat_t *stat);
void printInv(inv_t *inv);
void printTabInv(tabInv_t *tabInv);
void freeInv(tabInv_t *tabInv);
void freeObj(inv_t *obj);

#endif