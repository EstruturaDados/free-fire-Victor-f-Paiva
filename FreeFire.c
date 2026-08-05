#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define TAM_NOME 30
#define TAM_TIPO 20
#define MAX_ITENS 10

// ----------------------------------------------------------------------------
// ESTRUTURAS DE DADOS (STRUCTS)
// ----------------------------------------------------------------------------
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;
} Item;


// Struct No: Elemento dinamico da Lista Encadeada
typedef struct No {
    Item dados;
    struct No* proximo;
} No;


// ----------------------------------------------------------------------------
// PROTOTIPOS DAS FUNCOES - VETOR (LISTA SEQUENCIAL)
// ----------------------------------------------------------------------------
bool inserirItemVetor(Item vetor[], int *total, Item novoItem);
bool removerItemVetor(Item vetor[], int *total, const char *nome);
void listarVetor(const Item vetor[], int total);
int buscarSequencialVetor(const Item vetor[], int total, const char *nome, int *comparacoes);
void ordenarVetorSelectionSort(Item vetor[], int total, int *comparacoes);
int buscarBinariaVetor(const Item vetor[], int total, const char *nome, int *comparacoes);

// ----------------------------------------------------------------------------
// PROTOTIPOS DAS FUNCOES - LISTA ENCADEADA
// ----------------------------------------------------------------------------
bool inserirItemLista(No **cabeca, Item novoItem, int *total);
bool removerItemLista(No **cabeca, const char *nome, int *total);
void listarLista(No *cabeca);
int buscarSequencialLista(No *cabeca, const char *nome, int *comparacoes);
void liberarLista(No **cabeca);

// ----------------------------------------------------------------------------
// PROTOTIPOS DAS FUNCOES AUXILIARES E INTERFACE
// ----------------------------------------------------------------------------
void limparBuffer();
void executarBenchmarkComparativo(int totalVetor, int totalLista);

// ============================================================================
// FUNCAO PRINCIPAL (MAIN)
// ============================================================================
int main() {
    // Declaracao do Vetor e controle
    Item mochilaVetor[MAX_ITENS];
    int totalVetor = 0;
    bool vetorOrdenado = false;

    // Declaracao da Lista Encadeada e controle
    No *mochilaLista = NULL;
    int totalLista = 0;

    int opcao;
    char nomeBusca[TAM_NOME];

    do {
        printf("\n==================================================\n");
        printf("   MOCHILA DE SOBREVIVENCIA - VETOR vs LISTA ENC. \n");
        printf("==================================================\n");
        printf(" Status Vetor: %d/%d itens | Ordenado: %s\n", 
               totalVetor, MAX_ITENS, vetorOrdenado ? "SIM" : "NAO");
        printf(" Status Lista: %d itens (alocacao dinamica)\n", totalLista);
        printf("--------------------------------------------------\n");
        printf(" 1. Inserir Item (em Ambas as Estruturas)\n");
        printf(" 2. Remover Item por Nome (em Ambas)\n");
        printf(" 3. Listar Itens (Vetor vs Lista Encadeada)\n");
        printf(" 4. Buscar Item (Busca Sequencial em Ambas)\n");
        printf(" 5. Ordenar Vetor (Selection Sort)\n");
        printf(" 6. Buscar Item no Vetor (Busca Binaria - Requer Ordenacao)\n");
        printf(" 7. Executar Benchmark Comparativo de Desempenho\n");
        printf(" 0. Sair\n");
        printf("==================================================\n");
        printf(" Escolha uma Opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            limparBuffer();
            opcao = -1;
        } else {
            limparBuffer();
        }

        switch (opcao) {
            case 1: {
                if (totalVetor >= MAX_ITENS) {
                    printf("\n[AVISO] Vetor esta cheio (%d/%d)! Nao e possivel adicionar mais no vetor.\n", totalVetor, MAX_ITENS);
                    break;
                }
                Item novo;
                printf("\n--- CADASTRAR NOVO ITEM ---\n");
                printf("Nome do Item: ");
                fgets(novo.nome, sizeof(novo.nome), stdin);
                novo.nome[strcspn(novo.nome, "\n")] = '\0';

                printf("Tipo (ex: arma, cura, municao): ");
                fgets(novo.tipo, sizeof(novo.tipo), stdin);
                novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

                printf("Quantidade: ");
                scanf("%d", &novo.quantidade);
                limparBuffer();

                // Insere simultaneamente nas duas estruturas
                bool insVetor = inserirItemVetor(mochilaVetor, &totalVetor, novo);
                bool insLista = inserirItemLista(&mochilaLista, novo, &totalLista);

                if (insVetor && insLista) {
                    printf("\n[SUCESSO] Item '%s' inserido com sucesso no Vetor e na Lista Encadeada!\n", novo.nome);
                    vetorOrdenado = false; // A insercao desordena o vetor
                } else {
                    printf("\n[ERRO] Falha ao inserir o item em uma das estruturas.\n");
                }
                break;
            }

            case 2: {
                if (totalVetor == 0 && totalLista == 0) {
                    printf("\n[AVISO] Ambas as mochilas estao vazias.\n");
                    break;
                }
                printf("\n--- REMOVER ITEM POR NOME ---\n");
                printf("Digite o nome do item a remover: ");
                fgets(nomeBusca, sizeof(nomeBusca), stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

                bool remVetor = removerItemVetor(mochilaVetor, &totalVetor, nomeBusca);
                bool remLista = removerItemLista(&mochilaLista, nomeBusca, &totalLista);

                printf("\nResultados da Remocao:\n");
                printf(" - Vetor: %s\n", remVetor ? "Item removido com sucesso." : "Item nao encontrado.");
                printf(" - Lista Encadeada: %s\n", remLista ? "Item removido com sucesso." : "Item nao encontrado.");
                break;
            }

            case 3: {
                printf("\n==================================================\n");
                printf("            EXIBINDO CONTEUDO DAS MOCHILAS        \n");
                printf("==================================================\n");
                printf("\n>>> 1. ESTRUTURA: VETOR (LISTA SEQUENCIAL) <<<\n");
                listarVetor(mochilaVetor, totalVetor);

                printf("\n>>> 2. ESTRUTURA: LISTA ENCADEADA (DINAMICA) <<<\n");
                listarLista(mochilaLista);
                break;
            }

            case 4: {
                if (totalVetor == 0 && totalLista == 0) {
                    printf("\n[AVISO] Mochilas vazias. Cadastre itens antes de buscar.\n");
                    break;
                }
                printf("\n--- BUSCA SEQUENCIAL COMPARATIVA ---\n");
                printf("Digite o nome do item para buscar: ");
                fgets(nomeBusca, sizeof(nomeBusca), stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

                int compVetor = 0, compLista = 0;

                clock_t tInicioV = clock();
                int idxV = buscarSequencialVetor(mochilaVetor, totalVetor, nomeBusca, &compVetor);
                clock_t tFimV = clock();
                double tempoV = ((double)(tFimV - tInicioV)) / CLOCKS_PER_SEC * 1000.0;

                clock_t tInicioL = clock();
                int idxL = buscarSequencialLista(mochilaLista, nomeBusca, &compLista);
                clock_t tFimL = clock();
                double tempoL = ((double)(tFimL - tInicioL)) / CLOCKS_PER_SEC * 1000.0;

                printf("\nRESULTADOS DA BUSCA SEQUENCIAL:\n");
                printf("+-----------------+-------------------+-------------------+\n");
                printf("| Metrica         | Vetor             | Lista Encadeada   |\n");
                printf("+-----------------+-------------------+-------------------+\n");
                printf("| Status          | %-17s | %-17s |\n", 
                       (idxV != -1) ? "ENCONTRADO" : "NAO ENCONTRADO",
                       (idxL != -1) ? "ENCONTRADO" : "NAO ENCONTRADO");
                printf("| Comparacoes     | %-17d | %-17d |\n", compVetor, compLista);
                printf("| Tempo Execucao  | %-14.4f ms | %-14.4f ms |\n", tempoV, tempoL);
                printf("+-----------------+-------------------+-------------------+\n");
                break;
            }

            case 5: {
                if (totalVetor < 2) {
                    printf("\n[AVISO] Sao necessarios pelo menos 2 itens para ordenar o vetor.\n");
                    break;
                }
                int compOrd = 0;
                clock_t tInicio = clock();
                ordenarVetorSelectionSort(mochilaVetor, totalVetor, &compOrd);
                clock_t tFim = clock();
                double tempoOrd = ((double)(tFim - tInicio)) / CLOCKS_PER_SEC * 1000.0;

                vetorOrdenado = true;
                printf("\n[SUCESSO] Vetor ordenado por NOME via Selection Sort!\n");
                printf(" - Comparacoes de chaves efetuadas: %d\n", compOrd);
                printf(" - Tempo de ordenacao: %.4f ms\n", tempoOrd);
                printf("\nEstado atual do Vetor Ordenado:\n");
                listarVetor(mochilaVetor, totalVetor);
                break;
            }

            case 6: {
                if (!vetorOrdenado) {
                    printf("\n[ERRO] O vetor precisa estar ORDENADO por NOME para realizar a Busca Binaria.\n");
                    printf("Execute a opcao 5 (Ordenar Vetor) primeiro.\n");
                    break;
                }
                printf("\n--- BUSCA BINARIA NO VETOR ---\n");
                printf("Digite o nome do item a buscar: ");
                fgets(nomeBusca, sizeof(nomeBusca), stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

                int compBin = 0, compSeq = 0;

                // Executa busca binaria
                clock_t tInicioBin = clock();
                int idxBin = buscarBinariaVetor(mochilaVetor, totalVetor, nomeBusca, &compBin);
                clock_t tFimBin = clock();
                double tempoBin = ((double)(tFimBin - tInicioBin)) / CLOCKS_PER_SEC * 1000.0;

                // Executa busca sequencial no vetor para comparacao
                buscarSequencialVetor(mochilaVetor, totalVetor, nomeBusca, &compSeq);

                printf("\n==================================================\n");
                printf("  COMPARATIVO: BUSCA SEQUENCIAL vs BUSCA BINARIA \n");
                printf("==================================================\n");
                if (idxBin != -1) {
                    printf(" Item Encontrado: %s (Qtd: %d | Tipo: %s)\n", 
                           mochilaVetor[idxBin].nome, mochilaVetor[idxBin].quantidade, mochilaVetor[idxBin].tipo);
                } else {
                    printf(" Item '%s' NAO encontrado no vetor.\n", nomeBusca);
                }
                printf("--------------------------------------------------\n");
                printf(" Algoritmo           | Comparacoes | Tempo (ms) | Complexidade\n");
                printf("--------------------------------------------------\n");
                printf(" Busca Sequencial    | %-11d | %-10.4f | O(n)\n", compSeq, 0.0);
                printf(" Busca Binaria       | %-11d | %-10.4f | O(log n)\n", compBin, tempoBin);
                printf("--------------------------------------------------\n");
                printf(" Eficiencia: A Busca Binaria reduziu as comparacoes em %.1f%%\n",
                       compSeq > 0 ? ((double)(compSeq - compBin) / compSeq) * 100.0 : 0.0);
                break;
            }

            case 7: {
                executarBenchmarkComparativo(totalVetor, totalLista);
                break;
            }

            case 0:
                printf("\nEncerrando o sistema de inventario e liberando memoria...\n");
                liberarLista(&mochilaLista);
                printf("Memoria liberada com sucesso. Ate a proxima fuga da ilha!\n");
                break;

            default:
                printf("\n[ERRO] Opcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// ----------------------------------------------------------------------------
// IMPLEMENTACAO DAS FUNCOES AUXILIARES
// ----------------------------------------------------------------------------
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ----------------------------------------------------------------------------
// IMPLEMENTACAO DAS OPERACOES - VETOR (LISTA SEQUENCIAL)
// ----------------------------------------------------------------------------

bool inserirItemVetor(Item vetor[], int *total, Item novoItem) {
    if (*total >= MAX_ITENS) {
        return false;
    }
    vetor[*total] = novoItem;
    (*total)++;
    return true;
}

bool removerItemVetor(Item vetor[], int *total, const char *nome) {
    if (*total == 0) return false;

    int indiceEncontrado = -1;
    for (int i = 0; i < *total; i++) {
        if (strcmp(vetor[i].nome, nome) == 0) {
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado == -1) return false;

    // Deslocamento a esquerda (shift) para manter contiguidade fisica
    for (int i = indiceEncontrado; i < *total - 1; i++) {
        vetor[i] = vetor[i + 1];
    }
    (*total)--;
    return true;
}

void listarVetor(const Item vetor[], int total) {
    if (total == 0) {
        printf(" [Vetor Vazio]\n");
        return;
    }
    for (int i = 0; i < total; i++) {
        printf(" Index [%d] -> Nome: %-18s | Tipo: %-10s | Qtd: %d\n",
               i, vetor[i].nome, vetor[i].tipo, vetor[i].quantidade);
    }
}

int buscarSequencialVetor(const Item vetor[], int total, const char *nome, int *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < total; i++) {
        (*comparacoes)++;
        if (strcmp(vetor[i].nome, nome) == 0) {
            return i; // Retorna o indice onde o item se encontra
        }
    }
    return -1; // Nao encontrado
}

void ordenarVetorSelectionSort(Item vetor[], int total, int *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < total - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < total; j++) {
            (*comparacoes)++;
            if (strcmp(vetor[j].nome, vetor[minIdx].nome) < 0) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            Item temp = vetor[i];
            vetor[i] = vetor[minIdx];
            vetor[minIdx] = temp;
        }
    }
}

int buscarBinariaVetor(const Item vetor[], int total, const char *nome, int *comparacoes) {
    *comparacoes = 0;
    int inicio = 0;
    int fim = total - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        (*comparacoes)++;

        int res = strcmp(vetor[meio].nome, nome);
        if (res == 0) {
            return meio; // Encontrado
        } else if (res < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    return -1; // Nao encontrado
}

// ----------------------------------------------------------------------------
// IMPLEMENTACAO DAS OPERACOES - LISTA ENCADEADA
// ----------------------------------------------------------------------------

bool inserirItemLista(No **cabeca, Item novoItem, int *total) {
    No *novoNo = (No*) malloc(sizeof(No));
    if (novoNo == NULL) {
        return false; // Falha de alocacao dinamica
    }
    novoNo->dados = novoItem;
    novoNo->proximo = NULL;

    // Insercao no final da lista
    if (*cabeca == NULL) {
        *cabeca = novoNo;
    } else {
        No *atual = *cabeca;
        while (atual->proximo != NULL) {
            atual = atual->proximo;
        }
        atual->proximo = novoNo;
    }
    (*total)++;
    return true;
}

bool removerItemLista(No **cabeca, const char *nome, int *total) {
    if (*cabeca == NULL) return false;

    No *atual = *cabeca;
    No *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->dados.nome, nome) == 0) {
            if (anterior == NULL) {
                // Removendo o primeiro no (cabeca)
                *cabeca = atual->proximo;
            } else {
                // Removendo um no do meio ou fim
                anterior->proximo = atual->proximo;
            }
            free(atual);
            (*total)--;
            return true;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    return false;
}

void listarLista(No *cabeca) {
    if (cabeca == NULL) {
        printf(" [Lista Encadeada Vazia]\n");
        return;
    }
    No *atual = cabeca;
    int pos = 0;
    while (atual != NULL) {
        printf(" No [%d] (%p) -> Nome: %-18s | Tipo: %-10s | Qtd: %d\n",
               pos++, (void*)atual, atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
        atual = atual->proximo;
    }
}

int buscarSequencialLista(No *cabeca, const char *nome, int *comparacoes) {
    *comparacoes = 0;
    No *atual = cabeca;
    int pos = 0;

    while (atual != NULL) {
        (*comparacoes)++;
        if (strcmp(atual->dados.nome, nome) == 0) {
            return pos; // Retorna a posicao do no
        }
        atual = atual->proximo;
        pos++;
    }
    return -1;
}

void liberarLista(No **cabeca) {
    No *atual = *cabeca;
    while (atual != NULL) {
        No *temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    *cabeca = NULL;
}

// ----------------------------------------------------------------------------
// BENCHMARK E ANALISE COMPARATIVA DETALHADA
// ----------------------------------------------------------------------------
void executarBenchmarkComparativo(int totalVetor, int totalLista) {
    printf("\n======================================================================\n");
    printf("   RELATORIO BENCHMARK COMPARATIVO: VETOR VS. LISTA ENCADEADA         \n");
    printf("======================================================================\n");
    printf(" ALOCACAO DE MEMORIA E CAPACIDADE:\n");
    printf("    - Vetor: Capacidade Fixa: %d slots.\n", MAX_ITENS);
    printf("      Tamanho ocupado: %lu bytes (%d elementos ativos).\n", 
           sizeof(Item) * MAX_ITENS, totalVetor);
    printf("    - Lista Encadeada: Capacidade: Limitada apenas pela RAM.\n");
    printf("      Tamanho por No: %lu bytes (Item: %lu + Ponteiro: %lu).\n", 
           sizeof(No), sizeof(Item), sizeof(struct No*));
    printf("      Tamanho ocupado total: %lu bytes (%d nos ativos).\n", 
           sizeof(No) * totalLista, totalLista);

