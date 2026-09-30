#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "saida.h"

// função para gravar o histograma e características no arquivo de saída
int gravar_caracteristicas(const char *caminho_saida, const char *nome_imagem, const PGMImage *img, int vizinhos, const unsigned int *histograma) {
  // abre o arquivo de saída (modo "a" para append/adicionar caso haja mais de uma imagem no diretório, ou "w" na primeira)
  // na primeira imagem abrir com "w" para limpar o arquivo anterior, e nas próximas com "a".
  FILE *f = fopen(caminho_saida, "a");
  if (f == NULL) 
  {
    fprintf(stderr, "Erro: Não foi possível criar ou abrir o arquivo de saída: %s\n", caminho_saida);
    return 0;
  }

  // formato exigido pela especificação:
  // nome largura altura vizinhos h[0] h[1] ... h[N-1]
  fprintf(f, "%s %d %d %d", nome_imagem, img->largura, img->altura, vizinhos);

  int num_bins = (vizinhos == 8) ? 256 : 65536;
  for (int i = 0; i < num_bins; i++) 
  {
    fprintf(f, " %u", histograma[i]);
  }
  fprintf(f, "\n");

  fclose(f);
  return 1;
}

// função opcional para salvar a imagem lbp gerada (-i)
int salvar_imagem_lbp(const char *dir_imagens, const char *nome_original, const PGMImage *img_lbp) 
{
  char caminho_completo[512];
  snprintf(caminho_completo, sizeof(caminho_completo), "%s/%s", dir_imagens, nome_original);

  FILE *f = fopen(caminho_completo, "w");
  if (f == NULL) 
  {
    fprintf(stderr, "Erro: Não foi possível criar a imagem LBP em %s\n", caminho_completo);
    return 0;
  }

  // escreve o cabeçalho pgm (P2)
  fprintf(f, "%s\n", img_lbp->tipo);
  fprintf(f, "%d %d\n", img_lbp->largura, img_lbp->altura);
  fprintf(f, "%d\n", img_lbp->maxval);

  // escreve os pixels da matriz lbp
  for (int i = 0; i < img_lbp->altura; i++) 
  {
    for (int j = 0; j < img_lbp->largura; j++) 
    {
      fprintf(f, "%d ", img_lbp->pixels[i][j]);
    }
      fprintf(f, "\n");
  }

  fclose(f);
  return 1;
}