#include <stdio.h>

void verificarPar(int numero) {
    if (numero % 2 == 0) {
        printf("%d é par.\n", numero);
        return;  // Encerra a função aqui
    }
    printf("%d é ímpar.\n", numero);
}

int main() {
    verificarPar(4);  // Exibe: 4 é par.
    verificarPar(8);  // Exibe: 8 é ímpar.
    return 0;
}