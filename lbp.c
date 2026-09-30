#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "pgm.h"
#include "lbp.h"

// implementação do lbp(8,1) - 8 vizinhos com raio 1
uint16_t lbp8_pixel(const PGMImage *image, int x, int y) 
{
  unsigned char central = image->pixels[y][x];
  uint16_t codigo = 0;

  // Coordenadas relativas dos 8 vizinhos imediatos (Raio 1)
  // P0 P1 P2
  // P3 C  P4
  // P5 P6 P7
  int vizinhos_dx[8] = {-1,  0,  1, -1,  1, -1,  0,  1};
  int vizinhos_dy[8] = {-1, -1, -1,  0,  0,  1,  1,  1};

  for (int i = 0; i < 8; i++) 
  {
    int nx = x + vizinhos_dx[i];
    int ny = y + vizinhos_dy[i];

    // se o vizinho for maior ou igual ao central, atribui bit 1
    if (image->pixels[ny][nx] >= central) 
    {
      codigo |= (1 << (7 - i)); // ou acumulando conforme a ordem estipulada
    }
  }
  return codigo;
}

// implementação do LBP(16,2) - 16 vizinhos com raio 2
uint16_t lbp16_pixel(const PGMImage *image, int x, int y) 
{
  unsigned char central = image->pixels[y][x];
  uint16_t codigo = 0;

  // aqui entram as coordenadas para os 16 vizinhos com raio 2 (janela 5x5)
  int dx[16] = {-2, -2, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2};
  int dy[16] = {-1, -2, -2, -2, -2, -2, -1, 0, 1, 2, 2, 2, 2, 2, 1, 0};

  for (int i = 0; i < 16; i++) 
  {
    int nx = x + dx[i];
    int ny = y + dy[i];

    if (image->pixels[ny][nx] >= central) 
    {
      codigo |= (1 << (15 - i));
    }
  }
  return codigo;
}

// função que processa a imagem inteira
int calcular_lbp_imagem(const PGMImage *img_entrada, int vizinhos, unsigned int *histograma, PGMImage *img_lbp, int gerar_imagem) 
{
  LBPFunction lbp_func = NULL;
  int margem = 0;

  // seleção da função por ponteiro para função com base no número de vizinhos
  if (vizinhos == 8) 
  {
    lbp_func = lbp8_pixel;
    margem = 1; // lbp(8,1) precisa de 1 pixel de borda
  } 
  else if (vizinhos == 16) 
  {
    lbp_func = lbp16_pixel;
    margem = 2; // lbp(16,2) precisa de 2 pixels de borda
  } else 
  {
    return 0;
  }

  // inicializa o histograma com zeros
  int num_bins = (vizinhos == 8) ? 256 : 65536;
  for (int i = 0; i < num_bins; i++) 
  {
    histograma[i] = 0;
  }

  // prepara a imagem lbp de saída (se solicitado via -i)
  if (gerar_imagem) 
  {
    img_lbp->largura = img_entrada->largura;
    img_lbp->altura = img_entrada->altura;
    img_lbp->maxval = 255; // Para visualização
    strcpy(img_lbp->tipo, "P2");
        
    // alocação da matriz da imagem lbp de saída...
  }

  // percorre a imagem respeitando as margens das bordas
  for (int y = 0; y < img_entrada->altura; y++) 
  {
    for (int x = 0; x < img_entrada->largura; x++) 
    {
      uint16_t codigo = 0;

      // regra de bordas: pixels que não possuem todos os vizinhos recebem zero
      if (y < margem || y >= img_entrada->altura - margem || x < margem || x >= img_entrada->largura - margem) 
      {
        codigo = 0;
      } 
      else 
      {
        // chama a função correspondente através do ponteiro para função
        codigo = lbp_func(img_entrada, x, y);
        // incrementa o histograma
        histograma[codigo]++;
      }

      if (gerar_imagem) 
      {
        // se for lbp(16,2), normaliza dividindo por 257 para o intervalo 0-255 exclusivamente para visualização
        if (vizinhos == 16) 
        {
          img_lbp->pixels[y][x] = (unsigned char)(codigo / 257);
        } 
        else 
        {
          img_lbp->pixels[y][x] = (unsigned char)codigo;
        }
      }
    }
  }
  return 1;
}