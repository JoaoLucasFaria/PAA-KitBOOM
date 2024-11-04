// tp.c
#include "tp.h"
#include "grafo.h"

int config_opt(int argc, char *argv[], char **arquivo_composicao, char **arquivo_configuracao)
{

    int opt;

    while ((opt = getopt(argc, argv, "c:k:")) != -1)
    {
        switch (opt)
        {
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

    if (arquivo_composicao == NULL || arquivo_configuracao == NULL)
    {
        fprintf(stderr, "Uso: %s -c arquivo_composicao -k arquivo_configuracao\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

int le_arquivo_composicao(const char *filename, Grafo *g, Completude *completude, FILE *saida)
{
    int max = 0;
    FILE *arquivo = fopen(filename, "r");
    if (arquivo == NULL)
    {
        printf("\nErro ao abrir o arquivo composição. Certifique-se de que o comando esteja na forma:\n ./tp -c arquivo_composição.txt -k arquivo_configuração.txt\n\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    while (fscanf(arquivo, "%d %d%2s", &completude[i].quantidade, &completude[i].tamanho, completude[i].cor) != EOF)
    {
        max += completude[i].quantidade;
        i++;
    }
    completude->total_completude = max;
    fclose(arquivo);
}

int le_arquivo_configuracao(const char *filename, Grafo *g, FILE *saida)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("\nErro ao abrir o arquivo configuração. Certifique-se de que o comando esteja na forma:\n./tp -c arquivo_composição.txt -k arquivo_configuração.txt\n\n");
        exit(EXIT_FAILURE);
    }

    BarraExplosivo barra;

    // Aloca dinamicamente a matriz para representar a caixa 6x6
    int **caixa = (int **)malloc(6 * sizeof(int *));
    for (int i = 0; i < 6; i++)
    {
        caixa[i] = (int *)calloc(6, sizeof(int)); // Inicializa com 0
    }

    while (fscanf(file, "%d %d %d %d %d %2s", &barra.x_inicio, &barra.y_inicio, &barra.x_fim, &barra.y_fim, &barra.tamanho, barra.cor) != EOF)
    {
        // Verifica se a barra está dentro dos limites permitidos
        if (barra.x_inicio < 1 || barra.x_inicio > 6 || barra.x_fim < 1 || barra.x_fim > 6 ||
            barra.y_inicio < 1 || barra.y_inicio > 6 || barra.y_fim < 1 || barra.y_fim > 6)
        {
            printf("Erro: Barra fora dos limites permitidos (6x6): [%d %d %d %d %d %s]\n",
                   barra.x_inicio, barra.y_inicio, barra.x_fim, barra.y_fim, barra.tamanho, barra.cor);
            return 0; // Interrompe o programa caso a barra seja inválida
        }

        // Marca as posições ocupadas na matriz da caixa
        for (int x = barra.x_inicio - 1; x <= barra.x_fim - 1; x++)
        {
            for (int y = barra.y_inicio - 1; y <= barra.y_fim - 1; y++)
            {
                if (caixa[y][x] == 1)
                {
                    printf("Erro: Sobreposição de barras na posição (%d,%d)\n", x + 1, y + 1);
                    return 0; // Erro de sobreposição
                }
                caixa[y][x] = 1;
            }
        }

        // Adiciona a barra ao grafo
        adiciona_barra(g, barra);

        // Verifica se excedeu o número máximo de barras permitidas
        if (g->num_barras > MAX_BARRAS)
        {
            printf("Erro: Excedido o número máximo de barras permitidas.\n");
            //fprintf(saida, "Nao\n");
            return 0;
        }
    }
    fclose(file);

    // Verifica adjacência entre todas as barras e adiciona arestas se necessário
    for (int i = 0; i < g->num_barras; i++)
    {
        for (int j = i + 1; j < g->num_barras; j++)
        {
            if (sao_adjacentes(g->barras[i], g->barras[j]))
            {
                adiciona_aresta(g, i, j);
            }
        }
    }

    // Verifica se há espaços vazios na caixa 6x6
    for (int y = 0; y < 6; y++)
    {
        for (int x = 0; x < 6; x++)
        {
            if (caixa[y][x] == 0)
            {
                printf("Existem espaços vazios na caixa e as bombas podem sofrer movimentos bruscos. (Continue por sua conta e risco)\n");
                return 0; // Indica erro de espaço vazio
            }
        }
    }

    // Libera a memória alocada dinamicamente
    for (int i = 0; i < 6; i++)
    {
        free(caixa[i]);
    }
    free(caixa);

    return 1; // Sucesso
}

int imprime_uso(struct rusage memoria, struct timeval start, struct timeval end, double tempo){
    getrusage(RUSAGE_SELF, &memoria);
    gettimeofday(&end, NULL);
    tempo =
      (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

    printf("\nUso de Memória: %ld Kbytes\n", memoria.ru_maxrss);
    printf("Tempo decorrido: %.6f segundos\n", tempo);
    printf("Tempo de usuário: %.6f segundos\n",
         memoria.ru_utime.tv_sec + memoria.ru_utime.tv_usec / 1000000.0);
    printf("Tempo de sistema: %.6f segundos\n",
         memoria.ru_stime.tv_sec + memoria.ru_stime.tv_usec / 1000000.0);
}
