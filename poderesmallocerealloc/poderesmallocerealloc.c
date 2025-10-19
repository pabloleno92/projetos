#include <stdio.h>
#include <locale.h>  // Para setlocale()
#include <stdlib.h>  // Para malloc(), realloc(), free()
#include <string.h>  // Para strcpy()

// Função para atualizar um poder usando ponteiro
void atualizarPoder(int *poder, int novoValor) {
    *poder = novoValor;  // Dereferenciamos o ponteiro para modificar o valor original
}

// Função para pausar a execução e fechar automaticamente após Enter
void pausarExecucao() {
    printf("Aperte Enter para continuar...");
    getchar(); // Captura o Enter do usuário
}

// Função para limpar o buffer de entrada
void limparBuffer() {
    while (getchar() != '\n');
}

int main() {
    // Configura a localidade para português brasileiro
    setlocale(LC_ALL, "pt_BR.UTF-8");

    int numPoderes;
    printf("Digite o número inicial de poderes: ");
    scanf("%d", &numPoderes);
    limparBuffer();

    // Alocando memória para o array de poderes
    int *poderes = (int *)malloc(numPoderes * sizeof(int));
    if (poderes == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    // Alocando memória para os nomes dos poderes
    char **nomes = (char **)malloc(numPoderes * sizeof(char *));
    if (nomes == NULL) {
        printf("Erro ao alocar memória para nomes!\n");
        free(poderes);
        return 1;
    }

    // Inicializando poderes com 0 e nomes
    for (int i = 0; i < numPoderes; i++) {
        poderes[i] = 0; // Inicializa poder com 0
        nomes[i] = (char *)malloc(50 * sizeof(char)); // Aloca espaço para o nome (máximo 50 caracteres)
        if (nomes[i] == NULL) {
            printf("Erro ao alocar memória para nome!\n");
            for (int j = 0; j < i; j++) free(nomes[j]);
            free(nomes);
            free(poderes);
            return 1;
        }
        printf("Digite o nome do poder %d: ", i + 1);
        fgets(nomes[i], 50, stdin);
        nomes[i][strcspn(nomes[i], "\n")] = '\0'; // Remove o '\n' do fgets
    }

    int opcao;
    do {
        // Menu de opções
        printf("\n=== Elemental Powers ===\n");
        printf("1. Atualizar poderes\n");
        printf("2. Adicionar novo poder\n");
        printf("3. Remover poder\n");
        printf("4. Mostrar poderes\n");
        printf("5. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            // Atualizar poderes
            printf("\nAtualize a força de cada elemento:\n");
            for (int i = 0; i < numPoderes; i++) {
                int novoValor;
                printf("Digite a força para %s (atual: %d): ", nomes[i], poderes[i]);
                scanf("%d", &novoValor);
                atualizarPoder(&poderes[i], novoValor);
                limparBuffer();
            }
        } else if (opcao == 2) {
            // Adicionar novo poder
            numPoderes++;
            int *tempPoderes = (int *)realloc(poderes, numPoderes * sizeof(int));
            char **tempNomes = (char **)realloc(nomes, numPoderes * sizeof(char *));
            if (tempPoderes == NULL || tempNomes == NULL) {
                printf("Erro ao realocar memória!\n");
                if (tempPoderes != NULL) poderes = tempPoderes;
                if (tempNomes != NULL) nomes = tempNomes;
                numPoderes--;
                continue;
            }
            poderes = tempPoderes;
            nomes = tempNomes;

            // Inicializa o novo poder com 0
            poderes[numPoderes - 1] = 0;
            nomes[numPoderes - 1] = (char *)malloc(50 * sizeof(char));
            if (nomes[numPoderes - 1] == NULL) {
                printf("Erro ao alocar memória para nome!\n");
                numPoderes--;
                poderes = (int *)realloc(poderes, numPoderes * sizeof(int));
                nomes = (char **)realloc(nomes, numPoderes * sizeof(char *));
                continue;
            }
            printf("Digite o nome do novo poder: ");
            fgets(nomes[numPoderes - 1], 50, stdin);
            nomes[numPoderes - 1][strcspn(nomes[numPoderes - 1], "\n")] = '\0';
            printf("Novo poder '%s' adicionado com força 0.\n", nomes[numPoderes - 1]);
            pausarExecucao();
        } else if (opcao == 3) {
            // Remover poder
            if (numPoderes <= 0) {
                printf("Nenhum poder para remover!\n");
                pausarExecucao();
                continue;
            }
            printf("Digite o índice do poder a remover (0 a %d): ", numPoderes - 1);
            int indice;
            scanf("%d", &indice);
            limparBuffer();
            if (indice < 0 || indice >= numPoderes) {
                printf("Índice inválido!\n");
                pausarExecucao();
                continue;
            }
            // Libera o nome do poder removido
            free(nomes[indice]);
            // Move os elementos à direita para preencher o espaço
            for (int i = indice; i < numPoderes - 1; i++) {
                poderes[i] = poderes[i + 1];
                nomes[i] = nomes[i + 1];
            }
            numPoderes--;
            // Realoca memória para o tamanho reduzido
            poderes = (int *)realloc(poderes, numPoderes * sizeof(int));
            nomes = (char **)realloc(nomes, numPoderes * sizeof(char *));
            if (poderes == NULL || nomes == NULL) {
                printf("Erro ao realocar memória!\n");
                if (poderes == NULL) poderes = (int *)malloc(numPoderes * sizeof(int));
                if (nomes == NULL) nomes = (char **)malloc(numPoderes * sizeof(char *));
                pausarExecucao();
                continue;
            }
            printf("Poder removido com sucesso.\n");
            pausarExecucao();
        } else if (opcao == 4) {
            // Mostrar poderes
            printf("\nPoderes atuais:\n");
            for (int i = 0; i < numPoderes; i++) {
                printf("%s: %d\n", nomes[i], poderes[i]);
            }
            pausarExecucao();
        }
    } while (opcao != 5);

    // Liberando toda a memória alocada
    for (int i = 0; i < numPoderes; i++) {
        free(nomes[i]);
    }
    free(nomes);
    free(poderes);

    printf("Programa finalizado.\n");
    pausarExecucao();

    return 0;
}