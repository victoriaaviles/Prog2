#ifndef LBP_H
#define LBP_H

#include <stdint.h>
#include "pgm.h"

typedef uint16_t (*LBPFunction)(const PGMImage *image, int x, int y);

// funções para calcular o código lbp de um pixel específico
uint16_t lbp8_pixel(const PGMImage *image, int x, int y);
uint16_t lbp16_pixel(const PGMImage *image, int x, int y);

// função que percorre a imagem, calcula o lbp e preenche o histograma
int calcular_lbp_imagem(const PGMImage *img_entrada, int vizinhos, unsigned int *histograma, PGMImage *img_lbp, int gerar_imagem);

#endif
