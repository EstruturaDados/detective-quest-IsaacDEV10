#include <stdio.h>
#include <stdlib.h> // Para malloc, free
#include <string.h> // Para strcpy, strcmp
#include <stdbool.h> // Para bool, true, false

// --- 1. ÁRVORE BINÁRIA (MAPA DA MANSÃO) ---

typedef struct Comodo {
    char nome[50];
    struct Comodo* esquerda; // Ex: Porta da esquerda
    struct Comodo* direita;  // Ex: Porta da direita
} Comodo;

/**
 * @brief Cria um novo nó (Comodo) para o mapa.
 */
Comodo* criarComodo(const char* nome) {
    Comodo* novoComodo = (Comodo*)malloc(sizeof(Comodo));
    strcpy(novoComodo->nome, nome);
    novoComodo->esquerda = NULL;
    novoComodo->direita = NULL;
    return novoComodo;
}

/**
 * @brief Exibe o mapa da mansão (travessia pre-ordem).
 * (Raiz -> Esquerda -> Direita)
 */
void exibirMapa(Comodo* raiz) {
    if (raiz == NULL) {
        return;
    }
    printf("  - Comodo: %s\n", raiz->nome);
    if (raiz->esquerda) {
        printf("    (Esquerda -> %s)\n", raiz->esquerda->nome);
    }
    if (raiz->direita) {
        printf("    (Direita  -> %s)\n", raiz->direita->nome);
    }
    exibirMapa(raiz->esquerda);
    exibirMapa(raiz->direita);
}

// --- 2. ÁRVORE DE BUSCA BINÁRIA (PISTAS) ---

typedef struct Pista {
    int id; // Chave da árvore de busca
    char descricao[100];
    char categoria[50];
    struct Pista* esquerda;
    struct Pista* direita;
} Pista;

/**
 * @brief Cria uma nova Pista.
 */
Pista* criarPista(int id, const char* desc, const char* cat) {
    Pista* novaPista = (Pista*)malloc(sizeof(Pista));
    novaPista->id = id;
    strcpy(novaPista->descricao, desc);
    strcpy(novaPista->categoria, cat);
    novaPista->esquerda = NULL;
    novaPista->direita = NULL;
    return novaPista;
}

/**
 * @brief Insere uma Pista na Árvore de Busca (ordenada por ID).
 */
Pista* inserirPista(Pista* raiz, Pista* novaPista) {
    if (raiz == NULL) {
        return novaPista; // Encontrou o local de inserção
    }
    
    // Condicionais para decidir onde inserir
    if (novaPista->id < raiz->id) {
        raiz->esquerda = inserirPista(raiz->esquerda, novaPista);
    } else if (novaPista->id > raiz->id) {
        raiz->direita = inserirPista(raiz->direita, novaPista);
    }
    
    return raiz;
}

/**
 * @brief Exibe as pistas em ordem de ID (travessia in-ordem).
 * (Esquerda -> Raiz -> Direita)
 */
void exibirPistasOrdenadas(Pista* raiz) {
    if (raiz == NULL) {
        return;
    }
    exibirPistasOrdenadas(raiz->esquerda);
    printf("  - [ID %d] %s (Categoria: %s)\n", raiz->id, raiz->descricao, raiz->categoria);
    exibirPistasOrdenadas(raiz->direita);
}

// --- 3. TABELA HASH (PISTA -> SUSPEITO) ---

#define TAMANHO_HASH 10 // Tamanho do vetor da tabela hash

/**
 * @brief Nó da lista encadeada para tratar colisoes.
 */
typedef struct Evidencia {
    int idPista; // Chave
    char nomeSuspeito[50]; // Valor
    struct Evidencia* proximo;
} Evidencia;

/**
 * @brief A estrutura da Tabela Hash (usa array de ponteiros).
 */
typedef struct {
    Evidencia* tabela[TAMANHO_HASH];
} TabelaHash;

/**
 * @brief Função Hash simples (operador módulo).
 */
int funcaoHash(int idPista) {
    return idPista % TAMANHO_HASH;
}

/**
 * @brief Inicializa a tabela hash com NULL.
 */
void inicializarTabelaHash(TabelaHash* th) {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        th->tabela[i] = NULL;
    }
}

/**
 * @brief Insere uma evidência (link Pista -> Suspeito) na tabela.
 */
void inserirEvidencia(TabelaHash* th, int idPista, const char* suspeito) {
    int indice = funcaoHash(idPista);
    
    // Cria o novo nó de evidência
    Evidencia* novaEvidencia = (Evidencia*)malloc(sizeof(Evidencia));
    novaEvidencia->idPista = idPista;
    strcpy(novaEvidencia->nomeSuspeito, suspeito);
    
    // Insere no início da lista encadeada (tratamento de colisão)
    novaEvidencia->proximo = th->tabela[indice];
    th->tabela[indice] = novaEvidencia;
    printf("  [Evidencia registrada: Pista %d -> %s]\n", idPista, suspeito);
}

/**
 * @brief Busca um suspeito rapidamente usando o ID da pista.
 */
const char* buscarSuspeito(TabelaHash* th, int idPista) {
    int indice = funcaoHash(idPista);
    
    Evidencia* atual = th->tabela[indice];
    while (atual != NULL) {
        if (atual->idPista == idPista) {
            return atual->nomeSuspeito; // Encontrou!
        }
        atual = atual->proximo;
    }
    
    return "Ninguem"; // Não encontrou
}


// --- ESTRUTURA ANINHADA PRINCIPAL (O JOGO) ---

typedef struct {
    Comodo* mapaMansao;     // Raiz da Árvore Binária
    Pista* arvorePistas;    // Raiz da Árvore de Busca
    TabelaHash tabelaSuspeitos; // A Tabela Hash
    Comodo* localAtual;     // Onde o detetive está
} JogoDetetive;

// --- FUNÇÃO MAIN (SIMULAÇÃO DO JOGO) ---

int main() {
    // 1. Inicializa o jogo
    JogoDetetive jogo;
    jogo.mapaMansao = NULL;
    jogo.arvorePistas = NULL;
    inicializarTabelaHash(&jogo.tabelaSuspeitos);

    printf("--- DETECTIVE QUEST: Iniciando Simulacao ---\n\n");
    
    // 2. Monta o Mapa (Árvore Binária)
    printf("1. Construindo o mapa da mansao...\n");
    jogo.mapaMansao = criarComodo("Salao Principal");
    jogo.mapaMansao->esquerda = criarComodo("Biblioteca");
    jogo.mapaMansao->direita = criarComodo("Sala de Jantar");
    jogo.mapaMansao->esquerda->esquerda = criarComodo("Escritorio");
    jogo.mapaMansao->direita->esquerda = criarComodo("Cozinha");

    // Exibe o mapa
    exibirMapa(jogo.mapaMansao);
    

    // 3. Cataloga as Pistas (Árvore de Busca Binária)
    printf("\n2. Detetive catalogando pistas (BST)...\n");
    jogo.arvorePistas = inserirPista(jogo.arvorePistas, criarPista(202, "Faca de Cozinha", "Arma do Crime"));
    jogo.arvorePistas = inserirPista(jogo.arvorePistas, criarPista(101, "Pegada na Lama", "Evidencia Externa"));
    jogo.arvorePistas = inserirPista(jogo.arvorePistas, criarPista(305, "Carta Ameacadora", "Motivacao"));
    jogo.arvorePistas = inserirPista(jogo.arvorePistas, criarPista(201, "Botao de Casaco", "Evidencia Fisica"));

    // Exibe as pistas (note que sairão em ordem de ID 101, 201, 202, 305)
    exibirPistasOrdenadas(jogo.arvorePistas);
    

    // 4. Vincula Pistas a Suspeitos (Tabela Hash)
    printf("\n3. Ligando evidencias aos suspeitos (Tabela Hash)...\n");
    inserirEvidencia(&jogo.tabelaSuspeitos, 202, "Mordomo"); // Faca
    inserirEvidencia(&jogo.tabelaSuspeitos, 101, "Jardineiro"); // Pegada
    inserirEvidencia(&jogo.tabelaSuspeitos, 305, "Sobrinho"); // Carta
    inserirEvidencia(&jogo.tabelaSuspeitos, 201, "Mordomo"); // Botao
    

    // 5. Resolvendo o Mistério (Busca na Tabela Hash)
    printf("\n4. SOLUCIONANDO O MISTERIO:\n");
    printf("   Quem esta ligado a 'Arma do Crime' (Pista ID 202)?\n");
    
    const char* culpado = buscarSuspeito(&jogo.tabelaSuspeitos, 202);
    
    printf("\n   >>> O suspeito e: %s!\n", culpado);
    
    printf("\n   Quem esta ligado ao 'Botao de Casaco' (Pista ID 201)?\n");
    culpado = buscarSuspeito(&jogo.tabelaSuspeitos, 201);
    printf("   >>> O suspeito e: %s!\n", culpado);

    printf("\n   Quem esta ligado a 'Pegada na Lama' (Pista ID 101)?\n");
    culpado = buscarSuspeito(&jogo.tabelaSuspeitos, 101);
    printf("   >>> O suspeito e: %s!\n", culpado);

    printf("\n--- Simulacao Concluida ---\n");

    // (Em um jogo real, teríamos funções para liberar (free) toda a memória alocada)
    return 0;
}