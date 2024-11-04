// main.c
#include "tp.h"
#include "grafo.h"

int main(int argc, char *argv[])
{
    char *arquivo_composicao = NULL;
    char *arquivo_configuracao = NULL;
    struct rusage memoria;
    struct timeval start, end;
    double tempo;
    int e = 0;
    gettimeofday(&start, NULL);
    FILE *saida = fopen("Saida", "w");
    config_opt(argc, argv, &arquivo_composicao, &arquivo_configuracao);

    Grafo g;
    Completude completude;
    inicializa_grafo(&g, MAX_BARRAS);

    // Lê o arquivo de configuração
    le_arquivo_configuracao(arquivo_configuracao, &g, &*saida);
    // Lê o arquivo de composição
    le_arquivo_composicao(arquivo_composicao, &g, &completude, &*saida);

    // Verifica a completude
    if (verificar_completude(&g, &completude) && e == 0)
    {
        printf("O kit está completo.\n");
    }
    else
    {
        printf("O kit está incompleto e não poderá ser montado\n");
        e++;
    }



    // Verifica o posicionamento
    if (verificar_posicionamento(&g, &*saida) && e != 0)
    {
        printf("O posicionamento das barras está correto e o Kit pode ser montado\n");
        free_all(&g);
        fprintf(saida, "Sim\n");
    }
    else fprintf(saida, "Não\n");
    imprime_uso(memoria, start, end, tempo);
    exit(1);
}
