#include "algen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

typedef struct _no {
    char *genotipo;
    int tam_genotipo;
    float fitness;
    int totalMov;
    int x, y;
    int colisoes;
    struct _no *proximo;
} No;

typedef struct _lista {
    No *inicio;
    No *fim;
} Lista;

typedef struct _posicao {
    int x;
    int y;
} Posicao;

typedef struct _matriz {
    unsigned int linhas, colunas;
    char **mapa;
    int x_inicial, y_inicial;
    int x_final, y_final;
} Matriz;

No *cria_no(Matriz *matriz) {
    No *no = malloc(sizeof(No));
    if (!no) return NULL;
    no->totalMov = 0;
    no->colisoes = 0;
    no->genotipo = NULL;
    no->tam_genotipo = 0;
    cria_genotipo(matriz, no);
    no->fitness = calcula_fitness(matriz, no);
    no->proximo = NULL;
    return no;
}

int obter_vizinhos_validos(Matriz *matriz, int x, int y, Posicao *vizinhos, int max_vizinhos) {
    int count = 0;
    if (x > 0 && count < max_vizinhos && matriz->mapa[x-1][y] != '#')
        vizinhos[count++] = (Posicao){x-1, y};
    if (x < (int)matriz->linhas - 1 && count < max_vizinhos && matriz->mapa[x+1][y] != '#')
        vizinhos[count++] = (Posicao){x+1, y};
    if (y > 0 && count < max_vizinhos && matriz->mapa[x][y-1] != '#')
        vizinhos[count++] = (Posicao){x, y-1};
    if (y < (int)matriz->colunas - 1 && count < max_vizinhos && matriz->mapa[x][y+1] != '#')
        vizinhos[count++] = (Posicao){x, y+1};
    return count;
}

Posicao proximo_estado_markov(Matriz *matriz, int x_atual, int y_atual) {
    Posicao vizinhos[4];
    int num_vizinhos = obter_vizinhos_validos(matriz, x_atual, y_atual, vizinhos, 4);
    if (num_vizinhos == 0)
        return (Posicao){x_atual, y_atual};
    int escolha = rand() % num_vizinhos;
    return vizinhos[escolha];
}

void cria_genotipo(Matriz *matriz, No *no) {
    int max_tam = 10 + rand() % 91;
    no->genotipo = malloc(sizeof(char) * max_tam);
    if (!no->genotipo) {
        printf("Erro ao alocar genotipo.\n");
        no->tam_genotipo = 0;
        return;
    }
    int x = matriz->x_inicial, y = matriz->y_inicial, totalMov = 0;
    for (int i = 0; i < max_tam; i++) {
        Posicao prox = proximo_estado_markov(matriz, x, y);
        if (prox.x == x && prox.y == y) break; 

        char direcao = 0;
        if (prox.x == x - 1)
            direcao = 'C'; 
        else if (prox.x == x + 1)
            direcao = 'B'; 
        else if (prox.y == y - 1)
            direcao = 'E'; 
        else if (prox.y == y + 1)
            direcao = 'D'; 
        else
            break;

        no->genotipo[totalMov++] = direcao;
        x = prox.x;
        y = prox.y;

        if (x == matriz->x_final && y == matriz->y_final)
            break;
    }
    if (totalMov == 0) totalMov = 1; 
    char *temp = realloc(no->genotipo, totalMov * sizeof(char));
    if (temp != NULL)
        no->genotipo = temp;
    no->totalMov = totalMov;
    no->tam_genotipo = totalMov;
    no->x = x;
    no->y = y;
    no->colisoes = 0;
}

Matriz *carregar_mapa(char *arquivo) {
    FILE *mapa = fopen(arquivo, "r");
    if (!mapa) {
        printf("Falha ao abrir arquivo %s\n", arquivo);
        return NULL;
    }
    Matriz *matriz = malloc(sizeof(Matriz));
    if (!matriz) {
        fclose(mapa);
        return NULL;
    }
    if (fscanf(mapa, "%u %u", &matriz->linhas, &matriz->colunas) != 2) {
        printf("Erro ao ler dimensoes da matriz\n");
        fclose(mapa);
        free(matriz);
        return NULL;
    }
    matriz->mapa = malloc(matriz->linhas * sizeof(char *));
    if (!matriz->mapa) {
        printf("Erro na alocacao do mapa\n");
        fclose(mapa);
        free(matriz);
        return NULL;
    }
    for (unsigned int i = 0; i < matriz->linhas; ++i) {
        matriz->mapa[i] = malloc(matriz->colunas * sizeof(char));
        if (!matriz->mapa[i]) {
            printf("Erro na alocacao do mapa linha %u\n", i);
            for (unsigned int j = 0; j < i; j++)
                free(matriz->mapa[j]);
            free(matriz->mapa);
            free(matriz);
            fclose(mapa);
            return NULL;
        }
    }

    fgetc(mapa); 

    char aux[1001];
    matriz->x_inicial = matriz->y_inicial = -1;
    matriz->x_final = matriz->y_final = -1;
    for (unsigned int i = 0; i < matriz->linhas; ++i) {
        if (!fgets(aux, sizeof(aux), mapa)) {
            printf("Erro ao ler linha %u\n", i);
            for (unsigned int j = 0; j < matriz->linhas; j++)
                free(matriz->mapa[j]);
            free(matriz->mapa);
            free(matriz);
            fclose(mapa);
            return NULL;
        }
        for (unsigned int j = 0; j < matriz->colunas; ++j) {
            matriz->mapa[i][j] = aux[j];
            if (aux[j] == 'S') {
                matriz->x_inicial = i;
                matriz->y_inicial = j;
            } else if (aux[j] == 'E') {
                matriz->x_final = i;
                matriz->y_final = j;
            }
        }
    }
    if (matriz->x_inicial == -1 || matriz->y_inicial == -1 || matriz->x_final == -1 || matriz->y_final == -1) {
        printf("Pontos iniciais e/ou finais nao encontrados!\n");
        for (unsigned int i = 0; i < matriz->linhas; ++i)
            free(matriz->mapa[i]);
        free(matriz->mapa);
        free(matriz);
        fclose(mapa);
        return NULL;
    }
    fclose(mapa);
    return matriz;
}

void imprimir(Lista *lista) {
    if (!lista || !lista->inicio) {
        printf("Lista vazia.\n");
        return;
    }
    No *atual = lista->inicio;
    printf("Individuos:\n");
    while (atual) {
        printf("Genotipo: ");
        for (int i = 0; i < atual->tam_genotipo; i++)
            printf("%c", atual->genotipo[i]);
        printf("\nFitness: %.2f\nCoordenada final (%d, %d)\n", atual->fitness, atual->x, atual->y);
        printf("------------------------------\n");
        atual = atual->proximo;
    }
}

Lista *cria_lista() {
    Lista *lista = malloc(sizeof(Lista));
    if (!lista) return NULL;
    lista->inicio = NULL;
    lista->fim = NULL;
    return lista;
}

void insere_elementos(Lista *lista, Matriz *matriz, int num_elementos) {
    for (int i = 0; i < num_elementos; ++i) {
        No *node = cria_no(matriz);
        if (!node) {
            printf("Erro ao criar no\n");
            return;
        }
        if (!lista->inicio) {
            lista->inicio = node;
            lista->fim = node;
        } else {
            lista->fim->proximo = node;
            lista->fim = node;
        }
    }
}

float calcula_fitness(Matriz *matriz, No *no) {
    int **visitas = malloc(matriz->linhas * sizeof(int *));
    if (!visitas) {
        printf("Erro ao alocar visitas\n");
        return -INFINITY;
    }
    for (unsigned int i = 0; i < matriz->linhas; i++) {
        visitas[i] = calloc(matriz->colunas, sizeof(int));
        if (!visitas[i]) {
            printf("Erro ao alocar visitas linha %u\n", i);
            for (unsigned int j = 0; j < i; j++)
                free(visitas[j]);
            free(visitas);
            return -INFINITY;
        }
    }

    int x = matriz->x_inicial, y = matriz->y_inicial;
    visitas[x][y] = 1;
    int colisoes = 0;
    for (int i = 0; i < no->tam_genotipo; i++) {
        int novo_x = x, novo_y = y;
        switch (no->genotipo[i]) {
            case 'C':
                if (x > 0)
                    novo_x--;
                break;
            case 'B':
                if (x < (int)matriz->linhas - 1)
                    novo_x++;
                break;
            case 'E':
                if (y > 0)
                    novo_y--;
                break;
            case 'D':
                if (y < (int)matriz->colunas - 1)
                    novo_y++;
                break;
            default:
                break;
        }
        if (matriz->mapa[novo_x][novo_y] == '#') {
            colisoes++;
            break;
        } else {
            x = novo_x;
            y = novo_y;
            visitas[x][y]++;
        }
        if (x == matriz->x_final && y == matriz->y_final)
            break;
    }

    int penalidade_repeticao = 0;
    for (unsigned int i = 0; i < matriz->linhas; i++) {
        for (unsigned int j = 0; j < matriz->colunas; j++) {
            if (visitas[i][j] > 1)
                penalidade_repeticao += (visitas[i][j] - 1);
        }
        free(visitas[i]);
    }
    free(visitas);

    float distancia = fabsf(x - matriz->x_final) + fabsf(y - matriz->y_final);
    float penalidade_colisao = 5.0f * colisoes;
    float penalidade_repetida = 3.0f * penalidade_repeticao;
    float recompensa_comprimento = 10.0f * no->totalMov + 0.1f * (no->totalMov * no->totalMov);

    return 1000.0f - distancia - penalidade_colisao - penalidade_repetida + recompensa_comprimento;
}

void aplicar_mutacao(No *individuo, float taxa_mutacao) {
    const char direcoes[] = {'C', 'B', 'D', 'E'};
    for (int i = 0; i < individuo->tam_genotipo; i++) {
        if ((rand() / (float)RAND_MAX) < taxa_mutacao) {
            individuo->genotipo[i] = direcoes[rand() % 4];
        }
    }
}

void aplicar_elitismo(Lista *origem, Lista *destino, int n) {
    No *atual = origem->inicio;
    for (int i = 0; i < n && atual != NULL; i++) {
        No *copia = malloc(sizeof(No));
        if (copia == NULL) return;

        copia->genotipo = malloc(atual->tam_genotipo * sizeof(char));
        if (copia->genotipo == NULL) {
            free(copia);
            return;
        }

        copia->tam_genotipo = atual->tam_genotipo;
        copia->totalMov = atual->totalMov;
        copia->fitness = atual->fitness;
        copia->x = atual->x;
        copia->y = atual->y;
        copia->colisoes = atual->colisoes;
        copia->proximo = NULL;

        memcpy(copia->genotipo, atual->genotipo, atual->tam_genotipo * sizeof(char));

        if (destino->inicio == NULL) {
            destino->inicio = copia;
            destino->fim = copia;
        } else {
            destino->fim->proximo = copia;
            destino->fim = copia;
        }

        atual = atual->proximo;
    }
}

void ordenar_por_fitness(Lista *lista) {
    if (lista == NULL || lista->inicio == NULL) return;

    int trocou;
    do {
        trocou = 0;
        No *atual = lista->inicio;
        No *anterior = NULL;

        while (atual != NULL && atual->proximo != NULL) {
            No *prox = atual->proximo;

            if (atual->fitness < prox->fitness) {
                if (anterior == NULL) {
                    lista->inicio = prox;
                } else {
                    anterior->proximo = prox;
                }

                atual->proximo = prox->proximo;
                prox->proximo = atual;

                trocou = 1;
                anterior = prox;
            } else {
                anterior = atual;
                atual = atual->proximo;
            }
        }
    } while (trocou);

    No *aux = lista->inicio;
    while (aux && aux->proximo != NULL) {
        aux = aux->proximo;
    }
    lista->fim = aux;
}

Lista* nova_geracao(Lista *geracao_atual, Matriz *matriz) {
    if (geracao_atual == NULL || matriz == NULL) return NULL;

    ordenar_por_fitness(geracao_atual);

    Lista *nova = criar_lista_com_crossover(geracao_atual, matriz);

    liberar_lista(geracao_atual);

    return nova;
}

Lista* criar_lista_com_crossover(Lista* lista, Matriz* matriz) {
    if (lista == NULL || lista->inicio == NULL || matriz == NULL) return NULL;

    const int TOTAL_INDIVIDUOS = 100;
    const int ELITE = 5;
    const float TAXA_MUTACAO = 0.05f;

    Lista* nova_lista = cria_lista();
    if (nova_lista == NULL) return NULL;

    aplicar_elitismo(lista, nova_lista, ELITE);

    No* atual = lista->inicio;
    int criados = 0;

    while (atual != NULL && atual->proximo != NULL && criados < 45) {
        No* proximo = atual->proximo;

        No* filho = malloc(sizeof(No));
        if (filho == NULL) {
            liberar_lista(nova_lista);
            return NULL;
        }

        filho->genotipo = malloc(100 * sizeof(char));
        if (filho->genotipo == NULL) {
            free(filho);
            liberar_lista(nova_lista);
            return NULL;
        }

        filho->totalMov = 100;
        filho->colisoes = 0;
        filho->tam_genotipo = 100;
        filho->proximo = NULL;

        const char direcoes[] = {'C', 'B', 'D', 'E'};

        int tam1 = atual->tam_genotipo < 50 ? atual->tam_genotipo : 50;

        int tam2 = proximo->tam_genotipo;
        int inicio2 = tam2 > 50 ? tam2 - 50 : 0;
        int tam2_real = tam2 - inicio2 < 50 ? tam2 - inicio2 : 50;

        for (int i = 0; i < 50; i++) {
            filho->genotipo[i] = (i < tam1) ? atual->genotipo[i] : direcoes[rand() % 4];
        }

        for (int i = 0; i < 50; i++) {
            int idx = inicio2 + i;
            filho->genotipo[50 + i] = (i < tam2_real) ? proximo->genotipo[idx] : direcoes[rand() % 4];
        }

        aplicar_mutacao(filho, TAXA_MUTACAO);

        int x = matriz->x_inicial;
        int y = matriz->y_inicial;
        int colisoes = 0;
        int mov_validos = 0;

        for (int i = 0; i < 100; i++) {
            int novo_x = x, novo_y = y;

            switch (filho->genotipo[i]) {
                case 'C': 
                    if (x > 0) novo_x--;
                    break;
                case 'B': 
                    if (x < (int)matriz->linhas - 1) novo_x++;
                    break;
                case 'E': 
                    if (y > 0) novo_y--;
                    break;
                case 'D': 
                    if (y < (int)matriz->colunas - 1) novo_y++;
                    break;
                default:
                    break;
            }

            if (novo_x < 0 || novo_x >= (int)matriz->linhas || novo_y < 0 || novo_y >= (int)matriz->colunas) {
                colisoes++;
                break;
            }

            if (matriz->mapa[novo_x][novo_y] == '#') {
                colisoes++;
                break;
            }

            x = novo_x;
            y = novo_y;
            mov_validos++;

            if (x == matriz->x_final && y == matriz->y_final) break;
        }

        filho->x = x;
        filho->y = y;
        filho->colisoes = colisoes;
        filho->tam_genotipo = mov_validos;
        filho->totalMov = mov_validos;
        filho->fitness = calcula_fitness(matriz, filho);

        if (nova_lista->inicio == NULL) {
            nova_lista->inicio = filho;
            nova_lista->fim = filho;
        } else {
            nova_lista->fim->proximo = filho;
            nova_lista->fim = filho;
        }

        atual = proximo->proximo;
        criados++;
    }

    int faltam = TOTAL_INDIVIDUOS - ELITE - criados;
    if (faltam > 0) {
        insere_elementos(nova_lista, matriz, faltam);
    }

    return nova_lista;
}

void liberar_lista(Lista *lista) {
    if (!lista) return;

    No *atual = lista->inicio;
    while (atual) {
        No *prox = atual->proximo;
        if (atual->genotipo) {
            free(atual->genotipo);
        }
        free(atual);
        atual = prox;
    }
    free(lista);
}

void liberar_matriz(Matriz *matriz) {
    if (!matriz) return;

    for (unsigned int i = 0; i < matriz->linhas; i++) {
        free(matriz->mapa[i]);
    }
    free(matriz->mapa);
    free(matriz);
}

void iterar_lista(Lista *lista, void (*callback)(No *no, void *contexto), void *contexto) {
    if (!lista || !callback) return;
    No *atual = get_inicio_lista(lista);  // se quiser, internamente acessa lista->inicio
    while (atual != NULL) {
        callback(atual, contexto);
        atual = get_proximo_no(atual);
    }
}

void salvar_individuo_csv(No *no, void *contexto) {
    FILE *arquivo = (FILE *)contexto;
    if (!arquivo || !no) return;

    fprintf(arquivo, "\"");  // Início do campo genotipo (entre aspas)
    for (int i = 0; i < no->tam_genotipo; i++) {
        fputc(no->genotipo[i], arquivo);
    }
    fprintf(arquivo, "\",");

    fprintf(arquivo, "%.2f,%d,%d,%d,%d\n", no->fitness, no->totalMov, no->colisoes, no->x, no->y);
}

void salvar_lista_csv(Lista *lista, const char *nome_arquivo) {
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo CSV para escrita.\n");
        return;
    }

    // Cabeçalho CSV
    fprintf(arquivo, "genotipo,fitness,totalMov,colisoes,x,y\n");

    // Itera sobre a lista e grava cada indivíduo
    iterar_lista(lista, salvar_individuo_csv, arquivo);

    fclose(arquivo);
}

No* get_proximo_no(No *no) {
    if (!no) return NULL;
    return no->proximo;
}

No* get_inicio_lista(Lista *lista) {
    if (!lista) return NULL;
    return lista->inicio;
}


