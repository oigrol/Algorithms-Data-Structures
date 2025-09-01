#include <stdio.h>
//la regina compare una sola volta sia nella riga che nella colonna
//regina distina -> ordinamento conta
//ogni riga e ogni colonna contengono una e una sola regina distinta in una delle 8 colonne (n=8)
//ci sono 8 righe (k=8)
//permutazioni semplici
//permutazioni di 8 elementi == disposizioni semplici di n elementi presi a n a n
//so di aver piazzato una sola regina per riga, una sola per colonna // devo controllare solo diagonali e antidiagonali
#define N 8

int riga[N], mark[N], num_sol=0;

void perm_sempl(int q);
int controlla(void);
void stampa(void);

int main ( ) {
  int c;

  for (c=0; c<N; c++) {
    riga[c] = -1;
    mark[c] =0;
  }

  perm_sempl(0);

  printf("\nNumero di soluzioni trovate: %d \n", num_sol);

}

void perm_sempl(int q) {
  int c;
  if (q >= N) {
    if (controlla()) {
      num_sol++;
      stampa();
      return;
    }
    return;
  }

  //permutiamo
  for (c=0; c<N; c++) //per le nostre scelte (colonne che vanno da 0 a N-1)
    if (mark[c] == 0) {//se non ho usato la regina per quella colonna
      mark[c] = 1; //la piazzo (indice = colonna -> valore=la prendo=1)
      riga[q] = c; //registro che quella regina q è in in quella colonna c
      perm_sempl(q+1); //itero sulla prossima regina con le permutazioni semplici
      mark[c] = 0; //backtrack -> non la piazzo (indice = colonna -> valore=non la prendo=0)
    }
  return;
}

int controlla (void) {
  int r, n, d;
  // controlla diagonali
  for (d=0; d<2*N-1; d++) {
    n=0;
    for (r=0; r<N; r++) {
      if (d==r+riga[r])
        n++;
    }
    if (n>1) return 0;
  }
  // controlla antidiagonali
  for (d=0; d<2*N-1; d++) {
    n=0;
    for (r=0; r<N; r++) {
      if (d==(r-riga[r]+N-1))
        n++;
    }
    if (n>1) return 0;
  }
  return 1;
}

void stampa() {
  int r, c;
  printf ("\nRisultato\n");
  for(r=0; r<N; r++) {
    for(c=0; c<N; c++)
      if (c==riga[r])
        printf ("%d", riga[r]);
      else
        printf ("*");
      printf ("\n");
  }
}

