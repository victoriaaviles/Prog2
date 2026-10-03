#ifndef PGM_H
#define PGM_H
#include <stdint.h>

typedef struct 
{
  char tipo[3];      // "P2" ou "P5"
  int largura;       // número de col
  int altura;        // número de lin
  int maxval;        // maior valor de intensidade de pixel
  uint16_t **pixels; // matriz para armazenar os pixels da imagem
} PGMImage;

// lê um arquivo pgm (P2 ou P5) para 'img'.
// Retorna 1 em sucesso e 0 em erro (nesse caso nada fica alocado).
int pgm_ler(const char *caminho_arquivo, PGMImage *img);     

// libera a matriz de pixels de 'img' (seguro chamar com pixels == NULL).
void pgm_liberar(PGMImage *img);     
#endif
