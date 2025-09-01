#include <stdio.h>
//versione con pruining -> non tratto tutti i casi, ma escludo casi che a priori non ci porteranno a soluzioni -> blocco ricorsione anticipatamente
//uso due vettori che marcano le diagonali e antidiagonali messe sotto scacco da una regina
//uso di 2 vettori d[2*N-1] e ad[2*N-1] per marcare le diagonali e le antidiagonali messe sotto scacco da una regina
#define N 8
#define ND (2*N-1)

int riga[N], mark[N], diag[ND], antidiag[ND], num_sol=0;

void perm_sempl(int q);
void stampa(void);

int main ( ) {
  int i;

  for (i=0; i<N; i++) {
    riga[i] = -1;
    mark[i] =0;
  }

  for (i=0; i<ND; i++) {
    diag[i] = 0;
    antidiag[i] = 0;
  }

  perm_sempl(0);

  printf("\nNumero di soluzioni trovate: %d \n", num_sol);

}

void perm_sempl(int q) {
  int c;
  if (q >= N) { //condizione di terminazione semplificata -> se ho piazzato regine sono sicuro al 100% che la soluzione sia accettabile grazie al pruining
    num_sol++;
    stampa();
    return;
  }

  for (c=0; c<N; c++) //ciclo sulle scelte/colnne
    if ((mark[c] == 0) && (diag[q+c]==0) && (antidiag[q-c+(N-1)]==0)  ) {
      //pruining: non c'è ancora una regina nella colonna + la diagonale corrispondente non è messa sotto scacco da un altra regina + lo stesso per l'antidiagonale
      mark[c] = 1; //piazza la regina alla colonna c
      diag[q+c] = 1; //quella diagonale è messa sotto scacco dalla regina
      antidiag[q-c+(N-1)] = 1; //anche per la antidiagonale
      riga[q] = c; //registro che la regina q messa alla riga q, è quella messa alla colonna c
      perm_sempl(q+1); //ricorro alla scelta/regina successiva
      mark[c] = 0; //backtrack -> colonna torna libera
      diag[q+c] = 0;//anche diag e antidiag tornano libere -> non sotto-scacco
      antidiag[q-c+(N-1)] = 0;
    }
  return;
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


