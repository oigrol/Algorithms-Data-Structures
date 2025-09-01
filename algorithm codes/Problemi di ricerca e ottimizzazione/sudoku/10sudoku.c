#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#define n 9
int schema[n][n], num_sol=0;
//matrice di interi
/********************************************************************
  Prototipi
********************************************************************/

void acquisisci(char* nomefile);
int controlla(int pos, int val);
void disp_ripet(int pos);

/********************************************************************
  Programma principale
********************************************************************/
int main() {
  char nomefile[20];

  printf("Inserire il nome del file: ");
  scanf("%s", nomefile);

  acquisisci(nomefile);

  disp_ripet(0);

  printf("\n Numero di soluzioni = %d\n", num_sol);

  return 0;
}

/********************************************************************
  Funzione ricorsiva di ricerca di una soluzione
********************************************************************/
void disp_ripet(int pos) {
  int i, j, t;

  /* verifica di terminazione */
  if (pos >= n*n) { //ho considerato tutta la griglia
    num_sol++;
    printf("Soluzione:\n");
    for (i=0; i<n; i++) {
      for (j=0; j<n; j++)
	    printf("%3d ", schema[i][j]);
      printf("\n");
    }
    return;
   }

   //pos è un indice linearizzato, dato che io lavoro su una matrice
   //devo calcolarmi la i(r) e la j(c) a partire da pos
  /* indici casella corrente */
  i = pos / n;
  j = pos % n;
  if (schema[i][j] != 0) {
    /* casella inizialmente piena: nessun tentativo da fare */
    disp_ripet(pos+1);
    return;
  }

  /* provo tutti i possibili valori da 1 a dim */
  for (t=1; t<=n; t++) { //scelte sono comprese tra k=1 e k=n
    schema[i][j] = t; //metto nella matrice il valore del numero e verifico se accettabile
    if (controlla(pos, t))
      disp_ripet(pos+1);
    //annullo scelta fatta -> smarcandolo (altrimenti si considera fissata a priori)
    schema[i][j] = 0;
  }
  return;
}

/********************************************************************
  Funzione di controllo della validita' di una soluzione parziale
********************************************************************/

int controlla(int pos, int val) {
  int i, j, r, c, dim=floor(sqrt(n));
  //se sudoku è 9*9 ogni quadrato è 3*3 =dim*dim

  i = pos/n;
  j = pos % n;

  /* controllo le righe */
  for (c=0; c<n; c++) {
    if (c!=j)
      if (schema[i][c]==val)
        return 0;
  }

  /* controllo le colonne */
  for (r=0; r<n; r++) {
    if (r!=i)
      if (schema[r][j]==val)
        return 0;
  }

  /* controllo i blocchi */
  for (r=(i/dim)*dim; r<(i/dim)*dim+dim; r++)
    for (c=(j/dim)*dim; c<(j/dim)*dim+dim; c++) {
      if ((r!=i) || (c!=j))
        if (schema[r][c]==val)
          return 0;
    }
  return 1;
}


/********************************************************************
  Funzione per la lettura dello schema di partenza
********************************************************************/
void acquisisci(char *nomefile) {
  int i, j;
  FILE *fp;

  fp = fopen(nomefile, "r");
  if (fp == NULL) {
    printf("Errore durante l'apertura del file\n");
    exit(1);
  }

  for (i=0; i<n; i++) {
    for (j=0; j<n; j++) {
	  fscanf(fp, "%d", &schema[i][j]);
    }
  }
  fclose(fp);

  return;
}
