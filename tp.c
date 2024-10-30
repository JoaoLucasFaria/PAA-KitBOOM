// tp.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "getopt.h"
#include "tp.h"

int config_opt(int argc, char *argv[], char **arquivo_composicao, char **arquivo_configuracao){    
    
    int opt;

    while ((opt = getopt(argc, argv, "c:k:")) != -1) {
        switch (opt) {
            case 'c':
                *arquivo_composicao = optarg;
                break;
            case 'k':
                *arquivo_configuracao = optarg;
                break;
            default:
                fprintf(stderr, "Uso: %s -c arquivo_composicao -k arquivo_configuracao\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (arquivo_composicao == NULL || arquivo_configuracao == NULL) {
        fprintf(stderr, "Uso: %s -c arquivo_composicao -k arquivo_configuracao\n", argv[0]);
        exit(EXIT_FAILURE);
    }

}

int le_arquivo_composicao(const char *filename, Grafo *g, Completude *completude) {
    int max = 0;
    FILE *arquivo = fopen(filename, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo composição.txt\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (fscanf(arquivo, "%d %d%2s", &completude[i].quantidade, &completude[i].tamanho, completude[i].cor) != EOF) {
        //Debug printf
        //printf("Lido do arquivo de completude: quantidade=%d, tamanho=%d, cor=%s\n", completude[i].quantidade, completude[i].tamanho, completude[i].cor);
        max += completude[i].quantidade;
        i++;
    }
    completude->total_completude = max;
    fclose(arquivo);
    //Debug printf
    //printf("Total de barras esperadas: %d\n", completude->total_completude);
}

int le_arquivo_configuracao(const char *filename, Grafo *g) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        exit(EXIT_FAILURE);
    }

    BarraExplosivo barra;

    // Aloca dinamicamente a matriz para representar a caixa 6x6
    int **caixa = (int **)malloc(6 * sizeof(int *));
    for (int i = 0; i < 6; i++) {
        caixa[i] = (int *)calloc(6, sizeof(int));  // Inicializa com 0
    }

    while (fscanf(file, "%d %d %d %d %d %2s", &barra.x_inicio, &barra.y_inicio, &barra.x_fim, &barra.y_fim, &barra.tamanho, barra.cor) != EOF) {
        // Verifica se a barra está dentro dos limites permitidos
        if (barra.x_inicio < 1 || barra.x_inicio > 6 || barra.x_fim < 1 || barra.x_fim > 6 ||
            barra.y_inicio < 1 || barra.y_inicio > 6 || barra.y_fim < 1 || barra.y_fim > 6) {
            printf("Erro: Barra fora dos limites permitidos (6x6): [%d %d %d %d %d %s]\n",
                   barra.x_inicio, barra.y_inicio, barra.x_fim, barra.y_fim, barra.tamanho, barra.cor);
            exit(EXIT_FAILURE); // Interrompe o programa caso a barra seja inválida
        }

        // Marca as posições ocupadas na matriz da caixa
        for (int x = barra.x_inicio - 1; x <= barra.x_fim - 1; x++) {
            for (int y = barra.y_inicio - 1; y <= barra.y_fim - 1; y++) {
                if (caixa[y][x] == 1) {
                    printf("Erro: Sobreposição de barras na posição (%d,%d)\n", x + 1, y + 1);
                    exit(EXIT_FAILURE); // Erro de sobreposição
                }
                caixa[y][x] = 1;
            }
        }

        // Adiciona a barra ao grafo
        adiciona_barra(g, barra);

        // Verifica se excedeu o número máximo de barras permitidas
        if (g->num_barras >= MAX_BARRAS) {
            printf("Erro: Excedido o número máximo de barras permitidas.\n");
            exit(EXIT_FAILURE);
        }
    }
    fclose(file);

    // Verifica adjacência entre todas as barras e adiciona arestas se necessário
    for (int i = 0; i < g->num_barras; i++) {
        for (int j = i + 1; j < g->num_barras; j++) {
            if (sao_adjacentes(g->barras[i], g->barras[j])) {
                adiciona_aresta(g, i, j);
            }
        }
    }

    // Verifica se há espaços vazios na caixa 6x6
    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 6; x++) {
            if (caixa[y][x] == 0) {
                printf("Existem espaços vazios na caixa e as bombas podem sofrer movimentos bruscos. (Continue por sua conta e risco)\n");
                return 0; // Indica erro de espaço vazio
            }
        }
    }

    // Libera a memória alocada dinamicamente
    for (int i = 0; i < 6; i++) {
        free(caixa[i]);
    }
    free(caixa);

    return 1; // Sucesso
}

