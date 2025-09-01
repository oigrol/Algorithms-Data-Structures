#include <stdio.h>

int gcd(int a, int b);

int main(void) {
    int a, b, result;
    printf("inserire due interi positivi (es: 54 21):\n");
    do {
        scanf("%d %d", &a, &b);
    } while (a<=0 || b<=0); //verifico che siano due interi positivi

    result = gcd(a,b);
    printf("Il massimo comune divisore e': %d\n", result);
    return 0;
}

int gcd(int a, int b) {
    int temp;
    if (a <= b) { //nel calcolo del massimo comune divisore deve essere a > b
        temp = a;
        a = b;
        b = temp;
    }

    if (b == 0) return a; //condizione di terminazione
    if (a%2 == 0 && b%2 == 0) { //a,b even
        return 2 * gcd(a/2, b/2);
    }
    if (a%2 != 0 && b%2 == 0) {
        return gcd(a, b/2);
    }
    if (a%2 == 0 && b%2 != 0) {
        return gcd(a/2, b);
    }
    if (a%2 != 0 && b%2 != 0) { //a,b odd
        return gcd((a-b)/2, b);
    }
}