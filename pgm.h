#ifndef PGM_H
#define PGM_H
#include <stdint.h>

typedef struct 
{
  char tipo[3];      // "P2" ou "P5"
  int largura;       // número de col
  int altura;        // número de lin
  int maxval;        // maior valor de intensidade de pixel
  unsigned char **pixels; // matriz para armazenar os pixels da imagem
} PGMImage;

int pgm_ler(const char *caminho_arquivo, PGMImage *img);     // função de leitura do arquivo pgm

void pgm_liberar(PGMImage *img);     // função para liberar a memória alocada da matriz de pixels

#endif