#include <ctype.h>
#include <stdio.h>
#include <string.h>
#define MAXLEN 31
#define MAXCOM 6

typedef enum comando_e {
    r_date,
    r_partenza,
    r_capolinea,
    r_ritardo,
    r_ritardo_tot,
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
    char codice_tratta[MAXLEN];
    char partenza[MAXLEN];
    char destinazione[MAXLEN];
    data_t data;
    orario_t ora_partenza;
    orario_t ora_arrivo;
    int ritardo;
} corse_t;

int crea_tabella(corse_t corse[]);
comando_e leggiComando();
void selezionaDati(corse_t corse[], int N, comando_e comando);
void stampa_tratta(corse_t corse);
void partenze_date(corse_t corse[], int N, data_t data_in, data_t data_fin);
void partenze_fermata(corse_t corse[], int N, char fermata_in[]);
void destinazione_fermata(corse_t corse[], int N, char fermata_fin[]);
void ritardo_date(corse_t corse[], int N, data_t data_in, data_t data_fin);
void ritardo_tot(corse_t corse[], int N, char codice_tratta[]);

int main(void) {
    int num_tratte = 0;
    corse_t corse[1000+1];
    comando_e comando;
    num_tratte = crea_tabella(corse);

    while (comando != r_fine) {
        comando = leggiComando();
        selezionaDati(corse, num_tratte, comando);
    }

    return 0;
}

int crea_tabella(corse_t corse[]) {
    FILE *fp_in = fopen("../corse.txt", "r");
    int num_tratte = 0;

    if (fp_in == NULL) {
        printf("Errore apertura file\n");
        return -1;
    }

    fscanf(fp_in, "%d", &num_tratte);
    int i = 0;
    while (fscanf(fp_in, "%s %s %s %d/%d/%d %d:%d:%d %d:%d:%d %d",
        corse[i].codice_tratta,
        corse[i].partenza,
        corse[i].destinazione,
        &corse[i].data.anno,
        &corse[i].data.mese,
        &corse[i].data.giorno,
        &corse[i].ora_partenza.ore,
        &corse[i].ora_partenza.minuti,
        &corse[i].ora_partenza.secondi,
        &corse[i].ora_arrivo.ore,
        &corse[i].ora_arrivo.minuti,
        &corse[i].ora_arrivo.secondi,
        &corse[i].ritardo) == 13) {
        if (i >= 1000) {
            printf("Errore: hai superato il limite massimo di tratte.\n");
            return -1;
        }
        i++;
    }
    return num_tratte;
}

comando_e leggiComando() {
    char cmd[MAXCOM][12] = {"date",
                            "partenza",
                            "capolinea",
                            "ritardo",
                            "ritardo_tot",
                            "fine"};
    char comando[12];
    comando_e caso = r_date;
    printf("\nInserire comando da eseguire tra quelli elencati:\n- date\n- partenza\n- capolinea\n- ritardo\n- ritardo_tot\n- fine\n");
    scanf("%s", comando);

    while ((caso < MAXCOM) && (strcmp(comando, cmd[caso])!=0)) {
        caso++;
    }
    return caso;
}

void selezionaDati(corse_t corse[], int N, comando_e comando) {
    switch (comando) {
        case r_date: {
            data_t data_in, data_fin;
            printf("Inserire intervallo date (dd/mm/aaaa-dd/mm/aaaa):\n");
            scanf("%d/%d/%d-%d/%d/%d",
                &data_in.giorno, &data_in.mese, &data_in.anno,
                &data_fin.giorno, &data_fin.mese, &data_fin.anno);
            partenze_date(corse, N, data_in, data_fin);
            break;
        }
        case r_partenza: {
            char fermata_in[MAXLEN];
            printf("Inserire fermata di partenza:\n");
            scanf("%s", fermata_in);
            partenze_fermata(corse, N, fermata_in);
            break;
        }
        case r_capolinea: {
            char fermata_fin[MAXLEN];
            printf("Inserire fermata di capolinea:\n");
            scanf("%s", fermata_fin);
            destinazione_fermata(corse, N, fermata_fin);
            break;
        }
        case r_ritardo: {
            data_t data_in, data_fin;
            printf("Inserire intervallo date (dd/mm/aaaa-dd/mm/aaaa):\n");
            scanf("%d/%d/%d-%d/%d/%d",
                &data_in.giorno, &data_in.mese, &data_in.anno,
                &data_fin.giorno, &data_fin.mese, &data_fin.anno);
            ritardo_date(corse, N, data_in, data_fin);
            break;
        }
        case r_ritardo_tot: {
            char codice_tratta[MAXLEN];
            printf("Inserire codice tratta (es: GTT000):\n");
            scanf("%s", codice_tratta);
            ritardo_tot(corse, N, codice_tratta);
            break;
        }
        case r_fine: break;
    }
}

void stampa_tratta(corse_t corse) {
    printf("\nTratta che rispetta i criteri:\n");
    printf("%s %s %s %d/%d/%d %.2d:%.2d:%.2d %.2d:%.2d:%.2d %d\n\n",
        corse.codice_tratta,
        corse.partenza,
        corse.destinazione,
        corse.data.giorno,
        corse.data.mese,
        corse.data.anno,
        corse.ora_partenza.ore,
        corse.ora_partenza.minuti,
        corse.ora_partenza.secondi,
        corse.ora_arrivo.ore,
        corse.ora_arrivo.minuti,
        corse.ora_arrivo.secondi,
        corse.ritardo);
}

void partenze_date(corse_t corse[], int N, data_t data_in, data_t data_fin) {
    for (int i = 0; i < N; i++) {
        if ((data_in.anno <= corse[i].data.anno && corse[i].data.anno <= data_fin.anno) &&
            (data_in.mese <= corse[i].data.mese && corse[i].data.mese <= data_fin.mese) &&
            (data_in.giorno <= corse[i].data.giorno && corse[i].data.giorno <= data_fin.giorno)) {
            stampa_tratta(corse[i]);
        }
    }
}

void partenze_fermata(corse_t corse[], int N, char fermata_in[]) {
    for (int i = 0; i < N; i++) {
        if (strcmp(corse[i].partenza, fermata_in) == 0) {
            stampa_tratta(corse[i]);
        }
    }
}

void destinazione_fermata(corse_t corse[], int N, char fermata_fin[]) {
    for (int i = 0; i < N; i++) {
        if (strcmp(corse[i].destinazione, fermata_fin) == 0) {
            stampa_tratta(corse[i]);
        }
    }
}

void ritardo_date(corse_t corse[], int N, data_t data_in, data_t data_fin) {
    for (int i = 0; i < N; i++) {
        if ((data_in.anno <= corse[i].data.anno && corse[i].data.anno <= data_fin.anno) &&
            (data_in.mese <= corse[i].data.mese && corse[i].data.mese <= data_fin.mese) &&
            (data_in.giorno <= corse[i].data.giorno && corse[i].data.giorno <= data_fin.giorno) &&
            (corse[i].ritardo >0)) {
            stampa_tratta(corse[i]);
        }
    }
}

void ritardo_tot(corse_t corse[], int N, char codice_tratta[]) {
    int ritardo = 0;
    for (int i = 0; i < N; i++) {
        if (strcmp(corse[i].codice_tratta, codice_tratta) == 0) {
            ritardo += corse[i].ritardo;
        }
    }
    printf("\nRitardo complessivo della tratta %s: %d\n\n", codice_tratta, ritardo);
}