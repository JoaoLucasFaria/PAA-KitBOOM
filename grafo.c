#ifndef GRAFO_H
#define GRAFO_H

#define MAX_BARRAS 36
#include "tp.h"
#include "grafo.h"
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <stdlib.h>

typedef struct
{
    int x_inicio, y_inicio;
    int x_fim, y_fim;
    char cor[3];
    int tamanho;
    int quantidade;
} BarraExplosivo;

typedef struct
{
    int **adj;              // Ponteiro para uma matriz dinâmica
    BarraExplosivo *barras; // Ponteiro para um array dinâmico de barras
    int num_vertices;
    int num_barras;
} Grafo;

typedef struct
{
    int quantidade;       // Quantidade de bombas
    int tamanho;          // Tamanho da bomba (1, 2, 3, etc.)
    char cor[3];          // Cor da bomba (Az, Vm, etc.)
    int total_completude; // Total de barras esperadas
} Completude;

void inicializa_grafo(Grafo *g, int num_vertices);
void adiciona_barra(Grafo *g, BarraExplosivo barra);
void adiciona_aresta(Grafo *g, int u, int v);
int sao_adjacentes(BarraExplosivo a, BarraExplosivo b);
int verificar_completude(Grafo *g, Completude *completude);
int verificar_posicionamento(Grafo *g, FILE *saida);
void liberar_matriz_adjacencia(Grafo *g);
void liberar_barras(Grafo *g);
void free_all(Grafo *g);

#endif // GRAFO_H
