#include <stdio.h>
#include <locale.h>  // Para setlocale() - biblioteca padrão do C
#include <stdlib.h>  // Para malloc() e free()

// Função para atualizar um poder usando ponteiro
void atualizarPoder(int *poder, int novoValor) {
    *poder = novoValor;  // Dereferenciamos o ponteiro para modificar o valor original
}

// Função para pausar a execução e fechar automaticamente após Enter
void pausarExecucao() {
    printf("Aperte Enter para finalizar...");
    getchar(); // Captura o Enter do usuário, fechando após isso
}

int main() {
    // Configura a localidade para português brasileiro (suporte a ç, á, é, etc.)
    const char *locale_set = setlocale(LC_ALL, "pt_BR.UTF-8");

    // Alocando memória dinamicamente para o array de poderes
    int *poderes = (int *)malloc(4 * sizeof(int));
    if (poderes == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    // Inicializa todos os poderes com 0
    for (int i = 0; i < 4; i++) {
        poderes[i] = 0;
    }

    // Ponteiro para o início da array
    int *ptr = poderes;

    // Atualizando cada poder com input do usuário
    printf("Bem-vindo ao Elemental Powers! Atualize a força de cada elemento:\n");

    for (int i = 0; i < 4; i++) {
        int novoValor;
        const char *elementos[] = {"Fogo", "Água", "Ar", "Terra"};

        printf("Digite a força para %s (atual: %d): ", elementos[i], *(ptr + i));
        scanf("%d", &novoValor);

        // Chamamos a função passando o ponteiro para o elemento atual
        atualizarPoder(ptr + i, novoValor);

        // Limpa o buffer de entrada após cada scanf
        while (getchar() != '\n');
    }

    // Imprimindo os valores novos usando ponteiro
    printf("\nPoderes atualizados:\n");
    ptr = poderes;  // Reseta o ponteiro para o início
    for (int i = 0; i < 4; i++) {
        const char *elementos[] = {"Fogo", "Água", "Ar", "Terra"};
        printf("%s: %d\n", elementos[i], *ptr);
        ptr++;  // Avança o ponteiro para o próximo elemento
    }

    // Liberando a memória alocada
    free(poderes);

    // Pausa a execução e fecha após Enter
    pausarExecucao();

    return 0;
}