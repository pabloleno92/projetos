#include <stdio.h>

int main() {
    int numero = 15;
    if (numero > 10 && numero < 20) {
        printf("%d está entre 10 e 20.\n", numero);
    } else {
        printf("%d não está entre 10 e 20.\n", numero);
    }
    return 0;
}