#ifndef ARGUMENTOS_H
#define ARGUMENTOS_H

typedef struct 
{
  char *diretorio;         // Diretório contendo as imagens PGM (-d)
  char *saida;             // Arquivo de saída para o histograma (-o)
  int vizinhos;            // Número de vizinhos: 8 ou 16 (-n)
  char *diretorio_imagens; // Diretório opcional para salvar imagens LBP (-i)
} Argumentos;

int parse_argumentos(int argc, char **argv, Argumentos *args);

#endif