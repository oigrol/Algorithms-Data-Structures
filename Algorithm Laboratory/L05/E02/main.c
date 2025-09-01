#include <stdio.h>
#include <stdlib.h>

/*scacchiera rettangolare di RxC caselle
 *in ogni casella inserire tessera con due segmenti, uno in verticale e uno in orizzontale (no obliquo)
 *ogni segmento ha colore(char) e punteggio(int)
 *
 *REGOLE:
 *ottieni punti associati ai segmenti se allinei:
 *1. lungo una intera riga segmenti in orizzontale dello stesso colore
 *2. lungo una intera colonna segmenti in verticale dello stesso colore
 *
 *Punteggio si calcola solo per righe (orizz) (o colonne (vert)) complete con segmenti dello stesso colore
 *
 *Le tessere (con segm. vert. e orizz.) possono essere ruotate di 90 gradi
 *in caso di rotazione di 90 gradi scambio
 *coloreT1 <-> coloreT2
 *valoreT1 <-> valoreT2
    int val1;
    char color2;
    int val2;
 *Ogni tessera è unica e distinta e può essere usata nella scacchiera max 1 volta
 *Ip: esistono abbastanza tessere per completare scacchiera
 *
 *Completa tutte le celle della scacchiera con le tessere che hai in modo che ottieni il max punteggio possibile
 *
 *tiles.txt -> contiene l'elenco delle tessere disponibili
 *(1 riga: numero T di tessere;
 *a seguire, le altre T righe hanno su ogni riga una tessera/quadruple nella forma: <coloreT1><valoreT1><coloreT2><valoreT2>, dove T1 e T2 sono i due segmenti (1=orizz, 2=vert) che costituiscono la tessera
 *)
 *Ongi tessera ha associato un identificativo da 0(prima tessera) a T-1(ultima tessera)
 *
 *board.txt -> contiene una configurazione iniziale della scacchiera con alcune tessere già posizionate (la 0, la 2, la 3, la 7 su intervallo da 0 a 8)
 *(1 riga: R C, sono le righe e le colonne della scacchiera
 *a seguire R righe con C elementi ciascuna che definiscono le configurazioni iniziali di ogni cella della scacchiera
 *nella forma ti(indice tessera usata da 0 a 8)/r(0 se non ruotata - 1 se ruotata)
 *se ti/r = -1/-1 allora quella cella è quota e va riempita in modo che la soluzione abbia punteggio massimo possibile
 */

typedef struct tessera_s {
    char color1;
    int val1;
    char color2;
    int val2;
    int mark;
} tessera_t;

typedef struct config_s {
    int id;
    int rot;
} config_t;

tessera_t *caricaTessere(FILE *fp, int *n);
config_t **caricaConfig(FILE *fp, int *r, int *c, tessera_t *tessera);
void rotateTessera(tessera_t *t);
void resolve(tessera_t *t, int n, config_t **conf, int R, int C);
void resolveR(int pos, tessera_t *t, int n, config_t **conf, int R, int C, config_t **bestSol, int *bestPunt);
int calcPunt(tessera_t *t, config_t **conf, int r, int c);
void stampa_scacchiera(tessera_t *t, config_t **conf, int r, int c);

int main(void) {
    tessera_t *tessere;
    config_t **config;
    int num_tessere=0, R=0, C=0;

    FILE *fp_tessere = fopen("../tiles.txt", "r");
    FILE *fp_config = fopen("../board.txt", "r");
    if (fp_tessere == NULL || fp_config == NULL) {
        printf("Errore apertura file.");
        return -1;
    }

    //salvo tutti i dati che mi servono nelle opportune strutture dati
    tessere = caricaTessere(fp_tessere, &num_tessere);
    config = caricaConfig(fp_config, &R, &C, tessere);

    fclose(fp_tessere);
    fclose(fp_config);

    //lavoro con i dati salvati per trovare la soluzione al mio problema
    resolve(tessere, num_tessere,config, R, C);

    for (int i=0; i<R; i++) {
        free(config[i]);
    }
    free(tessere);
    free(config);
    return 0;
}

tessera_t *caricaTessere(FILE *fp, int *n) {
    tessera_t *t;
    fscanf(fp, "%d", n);
    t = (tessera_t *)malloc(*n * sizeof(tessera_t));
    for (int i=0; i<*n; i++) {
        fscanf(fp, " %c %d %c %d", &t[i].color1, &t[i].val1, &t[i].color2, &t[i].val2); //aggiungo spazio prima di %c perchè, a differenza di %d e %s, %c legge anche gli spazi o altri caratteri (\n, \t, ecc) -> con lo spazio ignoro tutto ciò e salvo solo il char del file che mi serve
        t[i].mark = 0;
    }
    return t;
}

config_t **caricaConfig(FILE *fp, int *r, int *c, tessera_t *tessera) {
    config_t **conf;
    fscanf(fp, "%d %d", r, c);
    conf = (config_t **)calloc(*r, sizeof(config_t *));
    for (int i=0; i<*r; i++) {
        conf[i] = (config_t *)calloc(*c, sizeof(config_t));
        for (int j=0; j<*c; j++) {
            fscanf(fp, "%d/%d", &conf[i][j].id, &conf[i][j].rot);
            if (conf[i][j].id != -1) { //allora devo inserire una tessera
                tessera[conf[i][j].id].mark = 1; //indico che sto usando la tessera id, perchè non la potrò più usare
            }
        }
    }
    return conf;
}

void rotateTessera(tessera_t *t) {
    //salvo variabili temporanee per effettuare scambi
    char temp_c = t->color1;
    int temp_i = t->val1;

    //scambio colore verticale e orizzontale
    t->color1 = t->color2;
    t->color2 = temp_c;
    //scambio punteggio verticale e orizzontale
    t->val1 = t->val2;
    t->val2 = temp_i;
}

void resolve(tessera_t *t, int n, config_t **conf, int R, int C) {
    config_t **bestConf;
    int maxPunt=0;

    bestConf = (config_t**)calloc(R, sizeof(config_t*));
    for (int i = 0; i < R; i++) {
        bestConf[i] = (config_t*)calloc(C, sizeof(config_t));
    }
    resolveR(0, t, n, conf, R, C, bestConf, &maxPunt);

    printf("Scacchiera finale (miglior soluzione):\n");
    stampa_scacchiera(t, bestConf, R, C);

    printf("punteggio: %d\n", maxPunt);
    for (int r=0; r<R; r++) {
        for (int c=0; c<C; c++) {
            printf("%d/%d ", bestConf[r][c].id, bestConf[r][c].rot);
        }
        printf("\n");
    }

    for (int i=0; i<R; i++) {
        free(bestConf[i]);
    }
    free(bestConf);
}

void resolveR(int pos, tessera_t *t, int n, config_t **conf, int R, int C, config_t **bestSol, int *bestPunt) {
    int punt, r, c;

    if (pos >= (R*C)) {
        punt=calcPunt(t, conf, R, C);
        if (punt > *bestPunt) {
            *bestPunt = punt;
            for (int i=0; i<R; i++)
                for (int j=0; j<C; j++)
                    bestSol[i][j] = conf[i][j];
        }
        return;
    }

    //trasformo [pos] matrice unidim. in [r][c] matrice bidim.
    r = pos / C;
    c = pos % C;

    if (conf[r][c].id == -1) { //solo se la cella della scacchiera è vuota cerco una possibile soluzione
        //provo tutte le combinazioni possibili di tessere non ancora usate
        for (int k=0; k<n; k++) {
            if (t[k].mark == 0) { //ancora non usata
                t[k].mark = 1; //segno che sto usando la tessera
                conf[r][c].id = k; //k indica l'indice della tessera (va da 0 a 8)
                conf[r][c].rot = 0; //provo prima con la tessera non ruotata
                resolveR(pos+1, t, n, conf, R, C, bestSol, bestPunt);

                //rotateTessera(&t[k]);
                conf[r][c].rot = 1; //ora provo con la stessa tessera ruotata
                resolveR(pos+1, t, n, conf, R, C, bestSol, bestPunt);

                //backtrack -> nessuna di queste due soluzioni mi ha portato al risultato che volevo
                //rotateTessera(&t[k]);
                conf[r][c].id = -1;
                conf[r][c].rot = 0;
                t[k].mark = 0; //libero la cella e segno che potrò riusare la tessera non più usata
            }
        }
    }
    else resolveR(pos+1, t, n, conf, R, C, bestSol, bestPunt); //se la cella pos (r,c) è già occupata, passo avanti
}

int calcPunt(tessera_t *t, config_t **conf, int r, int c) {
    int punteggio_tot = 0, punt_r, punt_c, prec_val, flag;
    char prec_id;
    // calcolo punteggio sulle colonne
    punt_c = 0;
    for (int j=0; j<c; j++) {
        if (conf[0][j].rot == 1) {
            rotateTessera(&t[conf[0][j].id]);
        }
        prec_id = t[conf[0][j].id].color2; //per ogni colonna mi segno il colore che sta in testa a ognuna di esse
        prec_val = t[conf[0][j].id].val2; //per ogni colonna mi segno il colore che sta in testa a ognuna di esse
        flag = 1;
        for (int i=1; i<r && flag; i++) {
            if (conf[i][j].rot == 1) {
                rotateTessera(&t[conf[i][j].id]);
            }
            if (t[conf[i][j].id].color2 == prec_id) {
                prec_val += t[conf[i][j].id].val2;
            } else {
                prec_val = 0;
                flag = 0;
            }
        }
        punt_c += prec_val;
    }

    // calcolo punteggio sulle righe
    punt_r = 0;
    for (int i=0; i<r; i++) {
        if (conf[i][0].rot == 1) {
            rotateTessera(&t[conf[i][0].id]);
        }
        prec_id = t[conf[i][0].id].color1; //per ogni colonna mi segno il colore che sta in testa a ognuna di esse
        prec_val = t[conf[i][0].id].val1; //per ogni colonna mi segno il colore che sta in testa a ognuna di esse
        flag = 1;
        for (int j=1; j<c && flag; j++) {
            if (conf[i][j].rot == 1) {
                rotateTessera(&t[conf[i][j].id]);
            }
            if (t[conf[i][j].id].color1 == prec_id) {
                prec_val += t[conf[i][j].id].val1;
            } else {
                prec_val = 0;
                flag = 0;
            }
        }
        punt_r += prec_val;
    }
    punteggio_tot = punt_c + punt_r;

    return punteggio_tot;
}

void stampa_scacchiera(tessera_t *t, config_t **conf, int r, int c) {
    for (int i=0; i<r; i++) {
        for (int j=0; j<c; j++) {
            if (conf[i][j].rot == 1) {
                rotateTessera(&t[conf[i][j].id]);
            }
            printf("\t%c\t", t[conf[i][j].id].color2);
        }
        printf("\n");
        for (int j=0; j<c; j++) {
            printf("%c\t\t%d", t[conf[i][j].id].color1, t[conf[i][j].id].val1);
        }
        printf("\n");
        for (int j=0; j<c; j++) {
            printf("\t%d\t", t[conf[i][j].id].val2);
        }
        printf("\n");
    }
    printf("\n");

}