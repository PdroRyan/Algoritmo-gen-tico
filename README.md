Algoritmo Genético para Resolução de Labirintos

Descrição:
Este projeto implementa um algoritmo genético para encontrar caminhos em labirintos. O algoritmo evolui uma população de soluções candidatas (indivíduos) através de gerações, aplicando operadores genéticos como seleção, crossover e mutação para melhorar progressivamente as soluções.

Funcionalidades Principais:
Carregamento de labirintos a partir de arquivos

Geração aleatória de indivíduos com movimentos válidos

Avaliação de fitness considerando distância ao objetivo, colisões e repetição de caminhos

Operadores genéticos: elitismo, crossover e mutação

Geração de novas populações com evolução progressiva

Exportação de resultados para arquivos CSV

Estrutura do Código:
Estruturas de Dados Principais
No: Representa um indivíduo com seu genótipo (sequência de movimentos) e métricas

Lista: Armazena a população de indivíduos

Matriz: Representa o labirinto com paredes, posição inicial e final

Funções Chave
Inicialização:

carregar_mapa(): Lê o labirinto de um arquivo

cria_no(): Cria um novo indivíduo aleatório

cria_lista(): Inicializa uma população

Avaliação:

calcula_fitness(): Avalia a qualidade de cada solução

ordenar_por_fitness(): Ordena a população por qualidade

Operadores Genéticos:

aplicar_elitismo(): Preserva os melhores indivíduos

criar_lista_com_crossover(): Combina genes dos pais para criar filhos

aplicar_mutacao(): Introduz variações aleatórias

Evolução:

nova_geracao(): Gera uma nova população evoluída

cria_genotipo(): Gera sequências de movimentos válidos

Utilidades:

imprimir(): Exibe informações da população

salvar_lista_csv(): Exporta resultados para CSV

liberar_lista(), liberar_matriz(): Gerenciamento de memória

Métricas de Avaliação:
O fitness de cada solução considera:

Distância ao objetivo final

Número de colisões com paredes

Penalização por caminhos repetidos

Recompensa por comprimento do caminho

Como Usar:
Prepare um arquivo de labirinto no formato especificado

Inicialize a população com cria_lista() e insere_elementos()

Itere chamando nova_geracao() para evoluir a população

Acompanhe os melhores indivíduos com imprimir() ou salvar_lista_csv()

Configurações:
Tamanho da população: 100 indivíduos

Tamanho da elite: 5 indivíduos

Taxa de mutação: 5%

Comprimento máximo do genótipo: 100 movimentos

Requisitos:
Compilador C compatível com C11

Bibliotecas padrão: stdio.h, stdlib.h, string.h, math.h, time.h

Formato do Arquivo de Labirinto:
N M
<mapa com N linhas e M colunas>
Onde:

S marca a posição inicial

E marca a posição final

# representa paredes

. representa caminhos livres

Exemplo de Saída:
text
Genotipo: CBDEBCDE...
Fitness: 956.72
Coordenada final (5, 7)
------------------------------
