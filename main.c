// main.c
#include <stdio.h>
#include <stdlib.h>
#include "getopt.h"
#include "tp.h"
#include "grafo.h"

int main(int argc, char *argv[]) {
    char *arquivo_composicao = NULL;
    char *arquivo_configuracao = NULL;
    config_opt(argc, argv, &arquivo_composicao, &arquivo_configuracao);

    Grafo g;
    Completude completude;
    inicializa_grafo(&g, MAX_BARRAS);

    // Lê o arquivo de configuração
    le_arquivo_configuracao(arquivo_configuracao, &g);
    // Lê o arquivo de composição
    le_arquivo_composicao(arquivo_composicao, &g, &completude);

    // Verifica a completude
    if (verificar_completude(&g, &completude)) {
        printf("O kit está completo.\n");
    } else {
        printf("O kit está incompleto e não poderá ser montado\n");
        exit(EXIT_FAILURE);
    }

    // Verifica o posicionamento
    if (verificar_posicionamento(&g)) {
        printf("O posicionamento das barras está correto e o Kit pode ser montado\n");
        free_all(&g);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
