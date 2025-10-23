#include <stdio.h>

int main() {
    int numero = 42;
    int *ponteiro;
    ponteiro = &numero;

    printf("Valor de numero: %d\n", numero);
    printf("Endereço de numero: %p\n", (void *)ponteiro);
    printf("Valor apontado por ponteiro: %d\n", *ponteiro);

    *ponteiro = 100;
    
    printf("Novo valor de numero: %d\n", numero);
    
    return 0;
}