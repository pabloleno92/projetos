#include <stdio.h>
#include <locale.h>  // Para setlocale()
#include <stdlib.h>  // Para malloc(), realloc(), free()
#include <string.h>  // Para strcpy(), strcmp(), strcspn()

// Estrutura para Poder
typedef struct {
    char nome[50];
    int nivel;
} Poder;

// Estrutura para Personagem
typedef struct {
    char nome[50];
    char tipo[50];
    Poder *poderes;
    int numPoderes;  // Máximo 2, mas usamos dinâmico para flexibilidade
} Personagem;

// Função para pausar a execução
void pausarExecucao() {
    printf("Aperte Enter para continuar...");
    getchar();
}

// Função para limpar o buffer de entrada
void limparBuffer() {
    while (getchar() != '\n');
}

// Função para adicionar um personagem
Personagem *adicionarPersonagem(Personagem *personagens, int *numPersonagens) {
    (*numPersonagens)++;
    Personagem *temp = (Personagem *)realloc(personagens, (*numPersonagens) * sizeof(Personagem));
    if (temp == NULL) {
        printf("Erro ao realocar memória para personagens!\n");
        (*numPersonagens)--;
        return personagens;
    }
    personagens = temp;

    // Inicializar o novo personagem
    Personagem *novo = &personagens[(*numPersonagens) - 1];
    novo->poderes = NULL;
    novo->numPoderes = 0;

    printf("Criando Personagem %d:\n", *numPersonagens);
    printf("Digite o nome do personagem: ");
    fgets(novo->nome, 50, stdin);
    novo->nome[strcspn(novo->nome, "\n")] = '\0';

    printf("Digite o tipo do personagem (ex: Fogo, Agua, Pedra): ");
    fgets(novo->tipo, 50, stdin);
    novo->tipo[strcspn(novo->tipo, "\n")] = '\0';

    // Adicionar até 2 poderes
    int i = 0;
    while (i < 2 && novo->numPoderes < 2) {
        char resposta[50];
        int validInput = 0;
        char *trimmed;
        do {
            printf("Deseja adicionar um poder %d? (s/n): ", i + 1);
            fgets(resposta, 50, stdin);
            resposta[strcspn(resposta, "\n")] = '\0';
            // Remover espaços em branco
            trimmed = resposta;
            while (*trimmed == ' ') trimmed++;
            if (strlen(trimmed) == 0) {
                printf("Entrada inválida! Digite apenas 's' ou 'n'.\n");
                continue;
            }
            if (strlen(trimmed) == 1 && (trimmed[0] == 's' || trimmed[0] == 'S' || trimmed[0] == 'n' || trimmed[0] == 'N')) {
                validInput = 1;
            } else {
                printf("Entrada inválida! Digite apenas 's' ou 'n'.\n");
            }
        } while (!validInput);

        if (validInput && (trimmed[0] == 'n' || trimmed[0] == 'N')) {
            break; // Sai do loop de adicionar poderes
        }

        novo->numPoderes++;
        Poder *tempPoderes = (Poder *)realloc(novo->poderes, novo->numPoderes * sizeof(Poder));
        if (tempPoderes == NULL) {
            printf("Erro ao realocar memória para poderes!\n");
            novo->numPoderes--;
            continue;
        }
        novo->poderes = tempPoderes;

        Poder *poderNovo = &novo->poderes[novo->numPoderes - 1];
        printf("Digite o nome do poder: ");
        fgets(poderNovo->nome, 50, stdin);
        poderNovo->nome[strcspn(poderNovo->nome, "\n")] = '\0';

        printf("Digite o nível do poder: ");
        scanf("%d", &poderNovo->nivel);
        limparBuffer();

        i++; // Avança para o próximo poder apenas após adicionar um com sucesso
    }

    printf("Personagem '%s' (Personagem %d) adicionado.\n", novo->nome, *numPersonagens);
    return personagens;
}

// Função para deletar um personagem
Personagem *deletarPersonagem(Personagem *personagens, int *numPersonagens, int indice) {
    if (indice < 0 || indice >= *numPersonagens) {
        printf("Índice inválido!\n");
        return personagens;
    }

    // Liberar poderes do personagem
    free(personagens[indice].poderes);

    // Mover os elementos à direita
    for (int i = indice; i < *numPersonagens - 1; i++) {
        personagens[i] = personagens[i + 1];
    }

    (*numPersonagens)--;
    Personagem *temp = (Personagem *)realloc(personagens, (*numPersonagens) * sizeof(Personagem));
    if (temp != NULL || *numPersonagens == 0) {
        personagens = temp;
    }

    printf("Personagem deletado com sucesso.\n");
    return personagens;
}

// Função para editar um personagem
void editarPersonagem(Personagem *personagem) {
    int opcao;
    do {
        printf("\nEditando '%s':\n", personagem->nome);
        printf("1. Alterar nome\n");
        printf("2. Alterar tipo\n");
        printf("3. Gerenciar poderes\n");
        printf("4. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            printf("Novo nome: ");
            fgets(personagem->nome, 50, stdin);
            personagem->nome[strcspn(personagem->nome, "\n")] = '\0';
        } else if (opcao == 2) {
            printf("Novo tipo: ");
            fgets(personagem->tipo, 50, stdin);
            personagem->tipo[strcspn(personagem->tipo, "\n")] = '\0';
        } else if (opcao == 3) {
            int subOpcao;
            do {
                printf("\nGerenciando poderes de '%s':\n", personagem->nome);
                for (int i = 0; i < personagem->numPoderes; i++) {
                    printf("%d. %s (nível: %d)\n", i, personagem->poderes[i].nome, personagem->poderes[i].nivel);
                }
                printf("1. Adicionar poder (máx 2)\n");
                printf("2. Editar poder\n");
                printf("3. Deletar poder\n");
                printf("4. Voltar\n");
                printf("Escolha: ");
                scanf("%d", &subOpcao);
                limparBuffer();

                if (subOpcao == 1 && personagem->numPoderes < 2) {
                    personagem->numPoderes++;
                    Poder *temp = (Poder *)realloc(personagem->poderes, personagem->numPoderes * sizeof(Poder));
                    if (temp == NULL) {
                        printf("Erro ao adicionar poder!\n");
                        personagem->numPoderes--;
                        continue;
                    }
                    personagem->poderes = temp;
                    Poder *novoPoder = &personagem->poderes[personagem->numPoderes - 1];
                    printf("Nome do novo poder: ");
                    fgets(novoPoder->nome, 50, stdin);
                    novoPoder->nome[strcspn(novoPoder->nome, "\n")] = '\0';
                    printf("Nível do novo poder: ");
                    scanf("%d", &novoPoder->nivel);
                    limparBuffer();
                } else if (subOpcao == 2) {
                    int idx;
                    printf("Índice do poder a editar: ");
                    scanf("%d", &idx);
                    limparBuffer();
                    if (idx >= 0 && idx < personagem->numPoderes) {
                        printf("Novo nome: ");
                        fgets(personagem->poderes[idx].nome, 50, stdin);
                        personagem->poderes[idx].nome[strcspn(personagem->poderes[idx].nome, "\n")] = '\0';
                        printf("Novo nível: ");
                        scanf("%d", &personagem->poderes[idx].nivel);
                        limparBuffer();
                    }
                } else if (subOpcao == 3) {
                    int idx;
                    printf("Índice do poder a deletar: ");
                    scanf("%d", &idx);
                    limparBuffer();
                    if (idx >= 0 && idx < personagem->numPoderes) {
                        for (int j = idx; j < personagem->numPoderes - 1; j++) {
                            personagem->poderes[j] = personagem->poderes[j + 1];
                        }
                        personagem->numPoderes--;
                        Poder *temp = (Poder *)realloc(personagem->poderes, personagem->numPoderes * sizeof(Poder));
                        if (temp != NULL || personagem->numPoderes == 0) {
                            personagem->poderes = temp;
                        }
                    }
                }
            } while (subOpcao != 4);
        }
    } while (opcao != 4);
}

// Função para calcular força total (soma de níveis de poderes)
int calcularForca(Personagem *personagem) {
    int forca = 0;
    for (int i = 0; i < personagem->numPoderes; i++) {
        forca += personagem->poderes[i].nivel;
    }
    return forca;
}

// Função para batalha
void batalha(Personagem *personagens, int numPersonagens) {
    int idx1, idx2, poder1, poder2;
    printf("Escolha o primeiro personagem (0 a %d): ", numPersonagens - 1);
    scanf("%d", &idx1);
    limparBuffer();
    if (idx1 < 0 || idx1 >= numPersonagens) {
        printf("Inválido!\n");
        return;
    }

    printf("Escolha o segundo personagem (0 a %d): ", numPersonagens - 1);
    scanf("%d", &idx2);
    limparBuffer();
    if (idx2 < 0 || idx2 >= numPersonagens || idx2 == idx1) {
        printf("Inválido!\n");
        return;
    }

    Personagem *p1 = &personagens[idx1];
    Personagem *p2 = &personagens[idx2];

    printf("Poderes de %s:\n", p1->nome);
    for (int i = 0; i < p1->numPoderes; i++) {
        printf("%d. %s (nível: %d)\n", i, p1->poderes[i].nome, p1->poderes[i].nivel);
    }
    printf("Escolha o poder para %s: ", p1->nome);
    scanf("%d", &poder1);
    limparBuffer();
    if (poder1 < 0 || poder1 >= p1->numPoderes) {
        printf("Inválido!\n");
        return;
    }

    printf("Poderes de %s:\n", p2->nome);
    for (int i = 0; i < p2->numPoderes; i++) {
        printf("%d. %s (nível: %d)\n", i, p2->poderes[i].nome, p2->poderes[i].nivel);
    }
    printf("Escolha o poder para %s: ", p2->nome);
    scanf("%d", &poder2);
    limparBuffer();
    if (poder2 < 0 || poder2 >= p2->numPoderes) {
        printf("Inválido!\n");
        return;
    }

    int nivel1 = p1->poderes[poder1].nivel;
    int nivel2 = p2->poderes[poder2].nivel;

    printf("\nBatalha: %s (%s: %d) vs %s (%s: %d)\n", p1->nome, p1->poderes[poder1].nome, nivel1, p2->nome, p2->poderes[poder2].nome, nivel2);

    if (nivel1 > nivel2) {
        printf("%s vence!\n", p1->nome);
    } else if (nivel2 > nivel1) {
        printf("%s vence!\n", p2->nome);
    } else {
        printf("Empate!\n");
    }
}

// Função para mostrar todos os personagens
void mostrarPersonagens(Personagem *personagens, int numPersonagens) {
    printf("\nLista de Personagens:\n");
    if (numPersonagens == 0) {
        printf("Nenhum personagem criado.\n");
        return;
    }
    for (int i = 0; i < numPersonagens; i++) {
        printf("%d. %s (Tipo: %s, Força Total: %d)\n", i, personagens[i].nome, personagens[i].tipo, calcularForca(&personagens[i]));
        for (int j = 0; j < personagens[i].numPoderes; j++) {
            printf("   - %s (nível: %d)\n", personagens[i].poderes[j].nome, personagens[i].poderes[j].nivel);
        }
    }
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");

    int numPersonagens = 0;
    Personagem *personagens = NULL;

    // Criar personagens iniciais com base na escolha do usuário
    printf("=== Criação de Personagens ===\n");
    while (1) {
        char resposta[50];
        int validInput = 0;
        char *trimmed;
        do {
            printf("Deseja criar um personagem? (s/n): ");
            fgets(resposta, 50, stdin);
            resposta[strcspn(resposta, "\n")] = '\0';
            trimmed = resposta;
            while (*trimmed == ' ') trimmed++;
            if (strlen(trimmed) == 0) {
                printf("Entrada inválida! Digite apenas 's' ou 'n'.\n");
                continue;
            }
            if (strlen(trimmed) == 1 && (trimmed[0] == 's' || trimmed[0] == 'S' || trimmed[0] == 'n' || trimmed[0] == 'N')) {
                validInput = 1;
            } else {
                printf("Entrada inválida! Digite apenas 's' ou 'n'.\n");
            }
        } while (!validInput);

        if (trimmed[0] == 'n' || trimmed[0] == 'N') {
            break;
        }

        printf("\n=== Criando Personagem %d ===\n", numPersonagens + 1);
        personagens = adicionarPersonagem(personagens, &numPersonagens);
    }

    int opcao;
    do {
        printf("\n=== Batalha das Lendas ===\n");
        printf("1. Mostrar personagens\n");
        printf("2. Adicionar personagem\n");
        printf("3. Editar personagem\n");
        printf("4. Deletar personagem\n");
        printf("5. Batalha\n");
        printf("6. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        if (opcao == 1) {
            mostrarPersonagens(personagens, numPersonagens);
            pausarExecucao();
        } else if (opcao == 2) {
            char resposta[50];
            int validInput = 0;
            char *trimmed;
            do {
                printf("Deseja criar um personagem? (s/n): ");
                fgets(resposta, 50, stdin);
                resposta[strcspn(resposta, "\n")] = '\0';
                trimmed = resposta;
                while (*trimmed == ' ') trimmed++;
                if (strlen(trimmed) == 0) {
                    printf("Entrada inválida! Digite apenas 's' ou 'n'.\n");
                    continue;
                }
                if (strlen(trimmed) == 1 && (trimmed[0] == 's' || trimmed[0] == 'S' || trimmed[0] == 'n' || trimmed[0] == 'N')) {
                    validInput = 1;
                } else {
                    printf("Entrada inválida! Digite apenas 's' ou 'n'.\n");
                }
            } while (!validInput);

            if (trimmed[0] == 's' || trimmed[0] == 'S') {
                printf("\n=== Criando Personagem %d ===\n", numPersonagens + 1);
                personagens = adicionarPersonagem(personagens, &numPersonagens);
            }
            pausarExecucao();
        } else if (opcao == 3) {
            int idx;
            mostrarPersonagens(personagens, numPersonagens);
            printf("Índice do personagem a editar: ");
            scanf("%d", &idx);
            limparBuffer();
            if (idx >= 0 && idx < numPersonagens) {
                editarPersonagem(&personagens[idx]);
            } else {
                printf("Índice inválido!\n");
            }
            pausarExecucao();
        } else if (opcao == 4) {
            int idx;
            mostrarPersonagens(personagens, numPersonagens);
            printf("Índice do personagem a deletar: ");
            scanf("%d", &idx);
            limparBuffer();
            personagens = deletarPersonagem(personagens, &numPersonagens, idx);
            pausarExecucao();
        } else if (opcao == 5) {
            if (numPersonagens < 2) {
                printf("É necessário pelo menos 2 personagens para uma batalha!\n");
            } else {
                batalha(personagens, numPersonagens);
            }
            pausarExecucao();
        }
    } while (opcao != 6);

    // Liberar memória
    for (int i = 0; i < numPersonagens; i++) {
        free(personagens[i].poderes);
    }
    free(personagens);

    printf("Programa finalizado.\n");
    return 0;
}