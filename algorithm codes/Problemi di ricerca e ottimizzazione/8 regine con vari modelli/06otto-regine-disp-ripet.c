#include <stdio.h>
//dato che voglio che regine non si mettano reciprocamente sottoscacco
//ogni riga può contenere una sola regina al massimo -> per costruzione non serve più controllo sulle righe ma solo su colonne, diag e antidiag
//mi basta allora registrare per quella riga i nella cella il valore della colonna in cui la regina è presente
//regine distinte -> ordinamento conta -> possiamo avere la stessa colonna riutilizzata per regine distinte
//dispodizioni con ripetizione

//riga[n] contiene alla posizione n quale colonna occupa la regina sulla riga n
//se riga ha tutti elementi distinti allora ogni regina è disposta su colonne diverse
//altrimenti se riga ha elementi uguali non è una soluzione valida -> più regine sulla stessa colonna
#define N 8

int riga[N];

int num_sol=0;
void disp_ripet(int q);
int controlla();
void stampa();

void main ( ) {
  int i;

  for (i=0; i<N; i++)
    riga[i] = 0;

  disp_ripet(0);

  printf("Numero di soluzioni trovate: %d \n", num_sol);
}

void disp_ripet(int q) {
  int i;
  if (q >= N) {
    if(controlla()) {
      num_sol++;
      stampa();
    }
    return;
  }
  for (i=0; i<N; i++) { //ciclo sulle scelte -> scelgo la colonna per una determinata riga
    riga[q] = i; //dato indice di ricorsione q=pos, la colonna può variare tra 0 e N-1
    //provo a mettere la regina su quella riga alla colonna i
    disp_ripet(q+1); //finisco di riempire vettore riga iterando su tutte le regine
  }

  return;
}

void stampa() {
  int r,c;
  printf("\nsoluzione %d\n",num_sol);
  for(r=0; r<N; r++) {
    for(c=0; c<N; c++)
      if (c==riga[r])
        printf ("%d", riga[r]);
      else
        printf ("*");
    printf ("\n");
  }
}

int controlla (void) {
  int r, n, d;
  int occ[N];
  //le righe non vannno controllate perchè le ho disposte una per riga da 0 a N-1
  for (r=0; r<N; r++)
    occ[r]=0;

  // controlla colonne
  for (r=0; r<N; r++)
    occ[riga[r]]++;

  for (r=0; r<N; r++)
    if (occ[r]>1)
      return 0;

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
