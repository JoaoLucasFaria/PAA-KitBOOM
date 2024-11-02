#include "getopt.h"
#include "grafo.h"
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <stdlib.h>


void inicializa_grafo(Grafo *g, int num_vertices)
{
    g->num_vertices = num_vertices;
    g->num_barras = 0;
    g->barras = NULL;

    // Aloca memória para a matriz de adjacência
    g->adj = (int **)malloc(num_vertices * sizeof(int *));
    if (g->adj == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para adj.\n");
        exit(1);
    }

    // Aloca cada linha da matriz de adjacência
    for (int i = 0; i < num_vertices; i++)
    {
        g->adj[i] = (int *)calloc(num_vertices, sizeof(int));
        if (g->adj[i] == NULL)
        {
            fprintf(stderr, "Erro ao alocar memória para adj[%d].\n", i);
            exit(1);
        }
    }
}

void adiciona_barra(Grafo *g, BarraExplosivo nova_barra)
{
    // Realoca memória para adicionar uma nova barra
    g->barras = (BarraExplosivo *)realloc(g->barras, (g->num_barras + 1) * sizeof(BarraExplosivo));
    if (g->barras == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para barras.\n");
        exit(1);
    }

    // Adiciona a nova barra e incrementa o contador
    g->barras[g->num_barras] = nova_barra;
    g->num_barras++;
}

void adiciona_aresta(Grafo *g, int u, int v)
{
    g->adj[u][v] = 1;
    g->adj[v][u] = 1;
}

int sao_adjacentes(BarraExplosivo a, BarraExplosivo b)
{
    if (a.y_inicio == b.y_inicio && a.y_fim == b.y_fim)
    {
        if ((a.x_fim + 1 == b.x_inicio) || (b.x_fim + 1 == a.x_inicio))
        {
            return 1;
        }
    }
    else if (a.x_inicio == b.x_inicio && a.x_fim == b.x_fim)
    {
        if ((a.y_fim + 1 == b.y_inicio) || (b.y_fim + 1 == a.y_inicio))
        {
            return 1;
        }
    }
    return 0;
}

int verificar_completude(Grafo *g, Completude *completude)
{
    int contagem_real[MAX_BARRAS] = {0};
    int e = 0;

    // Para cada barra no grafo, verificamos a correspondência com as barras esperadas
    for (int i = 0; i < g->num_barras; i++)
    {
        for (int j = 0; j < completude->total_completude; j++)
        {
            if (g->barras[i].tamanho == completude[j].tamanho && strcmp(g->barras[i].cor, completude[j].cor) == 0)
            {
                contagem_real[j]++;
                break; // Para evitar contagem duplicada
            }
        }
    }

    // Verifica se a contagem real corresponde à quantidade esperada de barras
    for (int i = 0; i < completude->total_completude; i++)
    {
        // Ignorar barras com quantidade esperada igual a zero
        if (completude[i].quantidade > 0 && completude[i].tamanho > 0 && completude[i].quantidade < MAX_BARRAS)
        {

            if (contagem_real[i] != completude[i].quantidade)
            {
                printf("Erro de completude: Esperado %d barras de %d%s, mas encontrado %d\n",
                       completude[i].quantidade, completude[i].tamanho, completude[i].cor, contagem_real[i]);
                e++;
            }
        }
    }
    return (e == 0) ? 1 : 0;
}

int verificar_posicionamento(Grafo *g, FILE *saida)
{
    for (int i = 0; i < g->num_barras; i++)
    {
        for (int j = i + 1; j < g->num_barras; j++)
        {
            if (strcmp(g->barras[i].cor, g->barras[j].cor) == 0 && sao_adjacentes(g->barras[i], g->barras[j]))
            {
                printf("Erro: Barras do mesmo tipo adjacentes encontradas. Barras %d e %d\n", i + 1, j + 1);
                return(0);
            }
        }
    }
    return 1; // Posicionamento correto
}

void liberar_matriz_adjacencia(Grafo *g)
{
    if (g->adj != NULL)
    {
        for (int i = 0; i < g->num_vertices; i++)
        {
            free(g->adj[i]); // Libera cada linha
        }
        free(g->adj); // Libera o ponteiro para o array de ponteiros
        g->adj = NULL;
    }
}

void liberar_barras(Grafo *g)
{
    if (g->barras != NULL)
    {
        free(g->barras);
        g->barras = NULL;
    }
}

void free_all(Grafo *g)
{
    liberar_matriz_adjacencia(g);
    liberar_barras(g);
}
