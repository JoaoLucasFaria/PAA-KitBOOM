// tp.h
#ifndef TP_H
#define TP_H

#include "grafo.h"
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <stdlib.h>

int le_arquivo_composicao(const char *filename, Grafo *g, Completude *completude, FILE *saida);
int le_arquivo_configuracao(const char *filename, Grafo *g, FILE *saida);
int config_opt(int argc, char *argv[], char **arquivo_composicao, char **arquivo_configuracao);
int imprime_uso(struct rusage memoria, struct timeval start, struct timeval end, double tempo);

#endif // TP_H
