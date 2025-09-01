#include <stdio.h>
//primo modello non raffinato per il numero elevato di casi

#define N 4

int s[N][N];
int num_sol=0;

void powerset (int r, int c, int q);
int controlla();
void stampa();


int main ( ) {
  int i, j;

  for (i=0; i<N; i++)
    for (j=0; j<N; j++)
      s[i][j] = 0;

  powerset(0,0,0);

  printf("Numero di soluzioni trovate: %d \n", num_sol);
  return 0;
}

void powerset (int r, int c, int q) {
  //scandisco scacchiera -> data una certa riga, scandisco tutte le colonne fino all'ultima per passare alla riga successiva, e così fino all'ultima

  if (c>=N) { //ricomincio da colonna 0 della riga successiva
    c=0; r++;
  }

  if (r>=N) { //ho esaminato tutte le righe della scacchiera -> scacchiera finita -> dovrei aver piazzato 8 regine cond. di validità
    if (q!=N) //non ho piazzato 8 regine? soluzione non valida
      return;
	if (controlla()) { //ho piazzato 8 regine -> verifico che la condizione che le regine non si mettano reciprocamente sotto scacco sia valida
      num_sol++;
      stampa();//se valida stampa la soluzione
    }
    return;
  }

  //numero le regine dalla prima alla ottava
  s[r][c] = q+1; //comincio a piazzare regina sulla riga e colonna correnti
  powerset (r,c+1,q+1); //ricorro sulla prossima colonna e sulla prossima regina con l'altra regina su r,c

  //backtrack
  s[r][c] = 0; //tolgo la regina sulla riga e colonna
  powerset (r,c+1,q); //ricorro cercando di piazzare sempre la stessa regina ma sulla colonna successiva -> ricorri senza la regina su r,c
  return;
}

int controlla (void) {
  //conto su riga colonna diagonale(che sono 2n-1 in totale) e antidiagonale(che sono 2n-1 in totale) adiacente alla regina che sto controllando il numero di regine presenti -> se questo numero è >1 ci sono altre regine che possono essere scaccate -> soluzione non valida
  // 15 diagonali individuate dalla somma degli indici di riga e di colonna
  // 15 antidiagonali individuate dalla differenza degli indici di riga e di colonna (+ 7 per non avere valori negativi)
  int r, c, d, n;

  //controlla righe
  for (r=0; r<N; r++) {
    for (c=n=0; c<N; c++)
      if (s[r][c]!=0) //tutte le volte che su quella riga incontra una regina alla colonna c incrementa di 1 l'n numero di regine
        n++;
	if (n>1)
      return 0;
  }
  //controlla colonne
  for (c=0; c<N; c++) {
    for (r=n=0; r<N; r++)
      if (s[r][c]!=0) //tutte le volte che su quella colonna incontra una regina alla riga r incrementa di 1 l'n numero di regine
        n++;
	if (n>1)
      return 0;
  }
  //controlla diagonali
  for (d=0; d<2*N-1; d++) {
    n=0;
    for (r=0; r<N; r++) {
      c = d-r; //indice diagonale = indice riga + indice colonna
      if ((c>=0)&& (c<N)) //se questo indice colonna è interno alla scacchiera
        if (s[r][c]!=0)
          n++;
    }
    if (n>1)
      return 0;
  }
  //controlla antidiagonali
  for (d=0; d<2*N-1; d++) {
    n=0;
    for (r=0; r<N; r++) {
      c = r-d+N-1; //indice antidiagonale = indice riga - indice colonna + N-1
      if ((c>=0)&& (c<N))
        if (s[r][c]!=0)
          n++;
    }
    if (n>1)
      return 0;
  }
  return 1;

}

void stampa() {
  int r,c;
  printf("\nsoluzione %d\n",num_sol);
  for(r=0; r<N; r++) {
    for(c=0; c<N; c++)
      if (s[r][c]!=0)
        printf ("Q");
      else
        printf ("*");
    printf ("\n");
  }
}
