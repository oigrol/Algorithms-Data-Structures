#include <stdio.h>
#include <ctype.h>

char *cercaRegexp(char *src, char *regexp);

int main() {
    char *find, regexp[50], src[100];
    printf("Inserisci stringa:\n");
    fgets(src, sizeof(src), stdin); //src equivale a scrivere &src[0] ovvero punta al primo carattere della stringa
    printf("Inserisci espressione regolare:\n");
    scanf("%s", regexp);
    find = cercaRegexp(src, regexp);
    if (find != NULL) {
        for (int i = 0; find[i] != ' ' && find[i] != '\0'; i++) {
            printf("%c", find[i]);
        }
    } else {
        printf("Nessuna corrispondenza trovata\n");
    }
    return 0;
}

char *cercaRegexp(char *src, char *regexp) {
    char *s = src;
    int corrisponde = 1;
    int escludi = 0, includi = 0;

    for (int i = 0, j = 0; src[i] != '\0'; i++, j++) {
        if (regexp[j] == '.') continue; //vai al carattere successivo
        if (regexp[j] == '[') {
            if (regexp[j+1] == '^') {
                escludi = 1;
                j++;
            } else includi = 1;
            i--; //ripeti il carattere attuale di src nel prossimo ciclo
        }
        else if (escludi) {
            corrisponde = 1;
            while (regexp[j] != ']') {
                if (regexp[j] == src[i]) corrisponde = 0;
                j++;
            }
            escludi = 0;
        } else if (includi) {
            corrisponde = 0;
            while (regexp[j] != ']') {
                if (regexp[j] == src[i]) corrisponde = 1;
                j++;
            }
            includi = 0;
        }
        else if (regexp[j] == '\\') {
            j++;
            if (regexp[j] == 'a' && isupper(src[i])) corrisponde = 0;
            else if (regexp[j] == 'A' && islower(src[i])) corrisponde = 0;
        } else if (regexp[j] != src[i]) corrisponde = 0;

        //se non c'è corrispondenza avanza al prossimo termine nella stringa
        if (!corrisponde) {
            while (src[i] != ' ' && src[i] != '\0') {
                i++;
            }
            if (src[i] == '\0') return NULL; //esci se sei alla fine della stringa
            j = -1; //reset dell'espressione regolare
            corrisponde = 1; //resetta corrispondenza per la prossima iterazione
            s = &src[i+1]; //avanza di un carattere per evitare spazi
        }
        if (regexp[j+1] == '\0') {
            return s;
        }
    }
    return NULL;
}
