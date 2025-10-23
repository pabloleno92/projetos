#include <stdio.h>

int main() {
    int x[5] = {10, 20, 30, 40, 50};
    int *ponteiro = x;

    printf("Valores e endereços dos elementos do array:\n");
    for (int i = 0; i < 5; i++) {
        printf("x[%d] = %d, Endereço: %p\n", i, x[i], (void*)&x[i]);
    }

    printf("\nEndereço do ponteiro: %p\n", (void*)&ponteiro);
    printf("Endereço para onde o ponteiro aponta (x[0]): %p\n", (void*)ponteiro);

    return 0;
}