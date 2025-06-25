#ifndef ALGEN_H
#define ALGEN_H

#include <stdio.h>

typedef struct _no No;
typedef struct _lista Lista;
typedef struct _posicao Posicao;
typedef struct _matriz Matriz;

/**
 * @brief Cria um novo indivíduo com genótipo aleatório baseado no mapa.
 * @param matriz Ponteiro para a estrutura Matriz que contém o mapa.
 * @return Ponteiro para o indivíduo criado, ou NULL em caso de erro.
 */
No *cria_no(Matriz *matriz);

/**
 * @brief Obtém os vizinhos válidos para movimentação em uma posição do mapa.
 * @param matriz Ponteiro para a estrutura Matriz.
 * @param x Coordenada x atual.
 * @param y Coordenada y atual.
 * @param vizinhos Array para armazenar os vizinhos válidos encontrados.
 * @param max_vizinhos Tamanho máximo do array de vizinhos.
 * @return Número de vizinhos válidos encontrados.
 */
int obter_vizinhos_validos(Matriz *matriz, int x, int y, Posicao *vizinhos, int max_vizinhos);

/**
 * @brief Escolhe o próximo estado na cadeia de Markov para um dado ponto no mapa.
 * @param matriz Ponteiro para a estrutura Matriz.
 * @param x_atual Coordenada x atual.
 * @param y_atual Coordenada y atual.
 * @return Próxima posição escolhida.
 */
Posicao proximo_estado_markov(Matriz *matriz, int x_atual, int y_atual);

/**
 * @brief Gera o genótipo do indivíduo baseado em um passeio aleatório pelo mapa.
 * @param matriz Ponteiro para a estrutura Matriz.
 * @param no Ponteiro para o indivíduo onde o genótipo será armazenado.
 */
void cria_genotipo(Matriz *matriz, No *no);

/**
 * @brief Carrega o mapa a partir de um arquivo texto.
 * @param arquivo Nome do arquivo contendo o mapa.
 * @return Ponteiro para a matriz carregada, ou NULL em caso de erro.
 */
Matriz *carregar_mapa(char *arquivo);

/**
 * @brief Imprime os indivíduos de uma lista na saída padrão.
 * @param lista Ponteiro para a lista a ser impressa.
 */
void imprimir(Lista *lista);

/**
 * @brief Cria uma lista vazia.
 * @return Ponteiro para a nova lista, ou NULL em caso de erro.
 */
Lista *cria_lista(void);

/**
 * @brief Insere um número especificado de indivíduos na lista com base no mapa.
 * @param lista Ponteiro para a lista onde os indivíduos serão inseridos.
 * @param matriz Ponteiro para a matriz do mapa.
 * @param num_elementos Número de indivíduos a serem criados e inseridos.
 */
void insere_elementos(Lista *lista, Matriz *matriz, int num_elementos);

/**
 * @brief Calcula o fitness de um indivíduo com base no mapa e seu genótipo.
 * @param matriz Ponteiro para a matriz do mapa.
 * @param no Ponteiro para o indivíduo cujo fitness será calculado.
 * @return Valor do fitness calculado.
 */
float calcula_fitness(Matriz *matriz, No *no);

/**
 * @brief Aplica mutação no genótipo de um indivíduo com uma taxa dada.
 * @param individuo Ponteiro para o indivíduo a sofrer mutação.
 * @param taxa_mutacao Probabilidade de mutação para cada gene.
 */
void aplicar_mutacao(No *individuo, float taxa_mutacao);

/**
 * @brief Copia os n melhores indivíduos da lista origem para a lista destino (elitismo).
 * @param origem Ponteiro para a lista original.
 * @param destino Ponteiro para a lista destino.
 * @param n Número de indivíduos a copiar.
 */
void aplicar_elitismo(Lista *origem, Lista *destino, int n);

/**
 * @brief Ordena a lista de indivíduos em ordem decrescente de fitness.
 * @param lista Ponteiro para a lista a ser ordenada.
 */
void ordenar_por_fitness(Lista *lista);

/**
 * @brief Cria uma nova lista de indivíduos a partir da atual usando crossover e mutação.
 * @param lista Ponteiro para a lista da geração atual.
 * @param matriz Ponteiro para a matriz do mapa.
 * @return Ponteiro para a nova lista (nova geração), ou NULL em caso de erro.
 */
Lista* criar_lista_com_crossover(Lista* lista, Matriz* matriz);

/**
 * @brief Gera uma nova geração a partir da geração atual (seleção, crossover, mutação).
 * @param geracao_atual Ponteiro para a lista da geração atual.
 * @param matriz Ponteiro para a matriz do mapa.
 * @return Ponteiro para a nova geração.
 */
Lista* nova_geracao(Lista *geracao_atual, Matriz *matriz);

/**
 * @brief Libera a memória ocupada por uma lista de indivíduos, incluindo seus genótipos.
 * @param lista Ponteiro para a lista a ser liberada.
 */
void liberar_lista(Lista *lista);

/**
 * @brief Libera a memória ocupada pela matriz do mapa.
 * @param matriz Ponteiro para a matriz a ser liberada.
 */
void liberar_matriz(Matriz *matriz);

/**
 * @brief Itera sobre a lista de indivíduos, executando uma função callback para cada um.
 * @param lista Ponteiro para a lista a ser iterada.
 * @param callback Função callback a ser executada para cada indivíduo.
 * @param contexto Contexto passado para a função callback.
 */
void iterar_lista(Lista *lista, void (*callback)(No *no, void *contexto), void *contexto);

/**
 * @brief Função auxiliar para salvar um indivíduo no formato CSV.
 * @param no Ponteiro para o indivíduo a ser salvo.
 * @param contexto Ponteiro para o arquivo onde será salvo.
 */
void salvar_individuo_csv(No *no, void *contexto);

/**
 * @brief Salva a lista de indivíduos em um arquivo CSV.
 * @param lista Ponteiro para a lista a ser salva.
 * @param nome_arquivo Nome do arquivo CSV onde os dados serão gravados.
 */
void salvar_lista_csv(Lista *lista, const char *nome_arquivo);

/**
 * @brief Obtém o próximo nó da lista encadeada.
 * @param no Ponteiro para o nó atual.
 * @return Ponteiro para o próximo nó, ou NULL se não houver próximo.
 */
No* get_proximo_no(No *no);

/**
 * @brief Obtém o nó inicial da lista.
 * @param lista Ponteiro para a lista.
 * @return Ponteiro para o primeiro nó da lista.
 */
No* get_inicio_lista(Lista *lista);

#endif // ALGEN_H
