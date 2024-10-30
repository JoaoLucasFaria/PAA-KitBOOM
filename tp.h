// tp.h
#ifndef TP_H
#define TP_H

#include "grafo.h"

int le_arquivo_composicao(const char *filename, Grafo *g, Completude *completude);
int le_arquivo_configuracao(const char *filename, Grafo *g);
int config_opt(int argc, char *argv[], char **arquivo_composicao, char **arquivo_configuracao);

#endif // TP_H
