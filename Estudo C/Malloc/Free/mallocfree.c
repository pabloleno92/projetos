#include <stdio.h>
#include <stdlib.h>

int main() {
    int x = 5;
    int *prt = (int *)malloc(x * sizeof(int));
    if (prt == NULL) {
        printf("Erro: Falha na alocação de memória!\n");
        return 1;
    }
    for (int i = 0; i < x; i++) {
        prt[i] = (i + 1) * 10;
    }
    printf("Valores no array x:\n");
    for (int i = 0; i < x; i++) {
        printf("Índice %d: %d\n", i, prt[i]);
    }
    free(prt);
    prt = NULL;
    printf("Memória liberada com sucesso!\n");
    return 0;
}