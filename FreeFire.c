#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.
#define TAM_NOME 30
#define TAM_TIPO 20
#define MAX_ITENS 10


// Struct Item:
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO];
    int quantidade;
} Item;


// Protótipos das funções
void limparTela();
void inserirItem(Item mochila[], int *totalItens);
void removerItem(Item mochila[], int *totalItens);
void listarItens(const Item mochila[], int totalItens);




int main() {
    int totalItens = 0;
    Item mochila[MAX_ITENS];
    int opcao;         
    
    do{
    printf("\n==========================================\n");
    printf("MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA      \n");
    printf("==========================================\n");
    printf("Itens na mochila %d/10\n", totalItens);
    printf("1. Adicionar Item\n");
    printf("2. Remover Item\n");
    printf("3. Listar Itens na Mochila\n");
    printf("0. Sair\n");
    printf("-------------------------------------------\n");
    printf("Escolha uma Opcao: ");
    scanf("%d", &opcao);
    limparTela();

    switch (opcao)
    {
    case 1:
        inserirItem(mochila, &totalItens);
        break;
    case 2:
        removerItem(mochila, &totalItens);
        break;
    case 3:
        listarItens(mochila, totalItens);
        break;
    case 0:
        printf("\nSaindo do sistema de inventario...\n");
        break;
    default:
        printf("\nOpcao invalida! Tente novamente.\n");
    } 
    } while (opcao != 0);

    return 0;
}



// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.
void limparTela(){
    int c;
    while ( (c= getchar()) != '\n' && c != EOF);    
}

// Adiciona um novo componente à mochila se houver espaço.
void inserirItem(Item mochila[], int *totalItens){
    if (*totalItens >= MAX_ITENS) {
        printf("\n[ERRO] A mochila esta cheia! Capacidade maxima: %d itens.\n", MAX_ITENS);
        return;
    }

    Item novoItem;

    printf("\n--- CADASTRAR NOVO ITEM ---\n");

    printf("Nome do item: ");
    fgets(novoItem.nome, sizeof(novoItem.nome), stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = '\0'; // Remove o \n capturado pelo fgets

    printf("Tipo (ex: arma, municao, cura, protecao): ");
    fgets(novoItem.tipo, sizeof(novoItem.tipo), stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);
    limparTela();

    // Insere o novo item na posição atual do total de itens
    mochila[*totalItens] = novoItem;
    (*totalItens)++;

    printf("\n[SUCESSO] Item cadastrado com sucesso!\n");

    // Listagem automática conforme requisito
    listarItens(mochila, *totalItens);
}

// Permite remover um componente da mochila pelo nome.
void removerItem(Item mochila[], int *totalItens) {
    if (*totalItens == 0) {
        printf("\n[AVISO] A mochila esta vazia. Nenhum item para remover.\n");
        return;
    }

    char nomeBusca[TAM_NOME];
    printf("\n--- REMOVER ITEM ---\n");
    printf("Digite o nome do item a ser removido: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    int indiceEncontrado = -1;

    // Busca o item para remover
    for (int i = 0; i < *totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) { 
            indiceEncontrado = i;
            break;
        }
    }

    if (indiceEncontrado != -1) {
        // Shift/deslocamento dos elementos para preencher o espaço do item removido
        for (int i = indiceEncontrado; i < *totalItens - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        (*totalItens)--;
        printf("\n[SUCESSO] Item '%s' removido com sucesso!\n", nomeBusca);
    } else {
        printf("\n[ERRO] Item '%s' nao foi encontrado na mochila.\n", nomeBusca);
    }

    // Listagem automática conforme requisito
    listarItens(mochila, *totalItens);
}

// Exibe uma tabela formatada com todos os componentes presentes na mochila.
void listarItens(const Item mochila[], int totalItens) {
    printf("\n====================================\n");
    printf("         ITENS NA MOCHILA (%d/%d)   \n", totalItens, MAX_ITENS);
    printf("====================================\n");

    if (totalItens == 0) {
        printf("A mochila esta vazia.\n");
        return;
    }

    for (int i = 0; i < totalItens; i++) {
        printf("%d. Nome: %-20s | Tipo: %-12s | Qtd: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
// Exibe a quantidade de comparações feitas (análise de desempenho).

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
