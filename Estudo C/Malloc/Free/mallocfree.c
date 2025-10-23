#include <stdio.h>
#include <stdlib.h>

int main() {
    int tamanho = 5;
    int *dinamico = (int *)malloc(tamanho * sizeof(int));
    if (dinamico == NULL) {
        printf("Erro: Falha na alocação de memória!\n");
        return 1;
    }
    for (int i = 0; i < tamanho; i++) {
        dinamico[i] = (i + 1) * 10;
    }
    printf("Valores no array dinâmico:\n");
    for (int i = 0; i < tamanho; i++) {
        printf("Índice %d: %d\n", i, dinamico[i]);
    }
    free(dinamico);
    dinamico = NULL;
    printf("Memória liberada com sucesso!\n");
    return 0;
}