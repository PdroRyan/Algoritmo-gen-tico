#include "algen.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARQUIVO_MAPA "C:\\Users\\Ryan\\Documents\\VSCODE\\Linguagem C\\TRABALHO_DE_ED_(Oficial)\\matriz.txt"

int main(void) {
    const char *arquivo_saida = "resultado.csv";
    const int num_individuos = 100;
    const int num_geracoes = 50;

    srand((unsigned int)time(NULL));

    Matriz *matriz = carregar_mapa(ARQUIVO_MAPA);
    if (!matriz) {
        printf("Erro ao carregar o mapa.\n");
        return EXIT_FAILURE;
    }

    Lista *populacao = cria_lista();
    if (!populacao) {
        printf("Erro ao criar lista de população.\n");
        liberar_matriz(matriz);
        return EXIT_FAILURE;
    }

    insere_elementos(populacao, matriz, num_individuos);

    for (int i = 0; i < num_geracoes; i++) {
        printf("Geração %d concluída.\n", i + 1);
        Lista *nova_populacao = nova_geracao(populacao, matriz);
        if (!nova_populacao) {
            printf("Erro ao gerar nova população na geração %d.\n", i + 1);
            liberar_lista(populacao);
            liberar_matriz(matriz);
            return EXIT_FAILURE;
        }
        populacao = nova_populacao;
    }

    salvar_lista_csv(populacao, arquivo_saida);
    printf("Resultado final salvo em: %s\n", arquivo_saida);

    liberar_lista(populacao);
    liberar_matriz(matriz);

    return EXIT_SUCCESS;
}
