#include <stdio.h>

int main() {
    int numero = 10;
    if (numero % 2 == 0 || numero % 5 == 0) {
        printf("%d é par ou divisível por 5.\n", numero);
    } else {
        printf("%d não é par nem divisível por 5.\n", numero);
    }
    return 0;
}