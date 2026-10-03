#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "pgm.h"
#include "lbp.h"

// lbp(8,1): 8 vizinhos imediatos, raio 1.
//
//   P0 P1 P2        cada vizinho Pi vale bit i do código:
//   P3 C  P4        b_i = 1 se Pi >= C, senão 0
//   P5 P6 P7        LBP = soma de b_i * 2^i   (P0 = bit 0, P7 = bit 7)

// só chamar para pixels que têm os 8 vizinhos dentro da imagem.

uint16_t lbp8_pixel(const PGMImage *image, int x, int y)
{
  // deslocamentos (coluna, linha) de P0..P7 em relação ao centro
  static const int dx[8] = {-1,  0,  1, -1,  1, -1,  0,  1};
  static const int dy[8] = {-1, -1, -1,  0,  0,  1,  1,  1};

  uint16_t central = image->pixels[y][x];  // valor do pixel central C
  uint16_t codigo = 0;

  for (int i = 0; i < 8; i++)
  {
    // pixels[linha][coluna] = pixels[y + dy][x + dx]
    if (image->pixels[y + dy[i]][x + dx[i]] >= central)
    {
      codigo |= (uint16_t)(1u << i);  // liga o bit i (vale 2^i)
    }
  }
  return codigo;  // de 0 a 255
}

// LBP(16,2): 16 vizinhos na borda de uma janela 5x5 (raio 2).
//
//   P0  P1  P2  P3  P4
//   P5  .   .   .   P6
//   P7  .   C   .   P8        (o bloco interno 3x3 NÃO é usado)
//   P9  .   .   .   P10
//   P11 P12 P13 P14 P15
//
// Mesma regra: b_i = 1 se Pi >= C, e LBP = soma de b_i * 2^i (16 bits).

uint16_t lbp16_pixel(const PGMImage *image, int x, int y)
{
  static const int dx[16] = {-2, -1, 0, 1, 2, -2, 2, -2, 2, -2, 2, -2, -1, 0, 1, 2};
  static const int dy[16] = {-2, -2, -2, -2, -2, -1, -1, 0, 0, 1, 1, 2, 2, 2, 2, 2};

  uint16_t central = image->pixels[y][x];
  uint16_t codigo = 0;

  for (int i = 0; i < 16; i++)
  {
    if (image->pixels[y + dy[i]][x + dx[i]] >= central)
    {
      codigo |= (uint16_t)(1u << i);
    }
  }
  return codigo;  // de 0 a 65535
}

// Calcula o lbp da imagem inteira e preenche o histograma.
//
// img_entrada : imagem PGM original
// vizinhos    : 8 ou 16
// histograma  : vetor já alocado (>= 256 ou 65536 posições); é zerado aqui
// img_lbp     : recebe a imagem lbp (só usada se gerar_imagem != 0)
// gerar_imagem: 1 se a opção -i foi usada
//
// retorna 1 em sucesso e 0 em erro (nesse caso img_lbp não fica alocada).

int calcular_lbp_imagem(const PGMImage *img_entrada, int vizinhos, unsigned int *histograma, PGMImage *img_lbp, int gerar_imagem)
{
  LBPFunction lbp_func = NULL;  // ponteiro para função: aponta para lbp8 ou lbp16
  int margem = 0;               // quantos pixels da borda não podem ser processados

  // escolhe a função e a margem conforme o número de vizinhos
  if (vizinhos == 8)
  {
    lbp_func = lbp8_pixel;
    margem = 1;   // raio 1 -> 1 pixel de borda sem vizinhos completos
  }
  else if (vizinhos == 16)
  {
    lbp_func = lbp16_pixel;
    margem = 2;   // raio 2 -> 2 pixels de borda
  }
  else
  {
    return 0;     // valor inválido (já validado em argumentos.c)
  }

  // zera o histograma: 256 posições para LBP8, 65536 para LBP16
  int num_bins = (vizinhos == 8) ? 256 : 65536;
  for (int i = 0; i < num_bins; i++)
  {
    histograma[i] = 0;
  }

  // se -i foi usado, aloca a matriz da imagem LBP (mesmo tamanho da original)
  if (gerar_imagem)
  {
    img_lbp->largura = img_entrada->largura;
    img_lbp->altura = img_entrada->altura;
    img_lbp->maxval = 255;          // a imagem LBP é salva com valores de 0 a 255
    strcpy(img_lbp->tipo, "P2");

    // calloc: ponteiros começam NULL, então pgm_liberar é seguro se falhar no meio
    img_lbp->pixels = (uint16_t **)calloc((size_t)img_lbp->altura, sizeof(uint16_t *));
    if (img_lbp->pixels == NULL)
    {
      fprintf(stderr, "Erro de alocação de memória para a imagem LBP\n");
      return 0;
    }
    for (int i = 0; i < img_lbp->altura; i++)
    {
      img_lbp->pixels[i] = (uint16_t *)malloc((size_t)img_lbp->largura * sizeof(uint16_t));
      if (img_lbp->pixels[i] == NULL)
      {
        fprintf(stderr, "Erro de alocação de memória para a imagem LBP\n");
        pgm_liberar(img_lbp);       // libera as linhas já alocadas
        return 0;
      }
    }
  }

  // percorre todos os pixels da imagem
  for (int y = 0; y < img_entrada->altura; y++)
  {
    for (int x = 0; x < img_entrada->largura; x++)
    {
      uint16_t codigo = 0;  // pixels de borda ficam com 0, como manda o enunciado

      // só calcula se o pixel tem todos os vizinhos dentro da imagem
      if (y >= margem && y < img_entrada->altura - margem &&
          x >= margem && x < img_entrada->largura - margem)
      {
        codigo = lbp_func(img_entrada, x, y);  // chama lbp8_pixel ou lbp16_pixel
        histograma[codigo]++;                  // conta mais uma ocorrência desse código
      }

      if (gerar_imagem)
      {
        // LBP16 vai até 65535: divide por 257 (65535 / 257 = 255) só para visualizar.
        // o histograma usa o 'codigo' original, então não é afetado.
        img_lbp->pixels[y][x] = (vizinhos == 16) ? (uint16_t)(codigo / 257) : codigo;
      }
    }
  }
  return 1;
}
