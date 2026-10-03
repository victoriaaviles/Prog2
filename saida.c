#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "saida.h"

// cria o arquivo de saída vazio (apaga o conteúdo anterior, se existir).
// deve ser chamada uma vez antes de processar as imagens; assim cada execução
// gera exatamente uma linha por imagem, sem acumular resultados de execuções antigas.
int criar_arquivo_saida(const char *caminho_saida)
{
  FILE *f = fopen(caminho_saida, "w");   // "w" apaga o conteúdo anterior
  if (f == NULL)
  {
    fprintf(stderr, "Erro: Não foi possível criar o arquivo de saída: %s\n", caminho_saida);
    return 0;
  }
  fclose(f);
  return 1;
}

// acrescenta ao arquivo de saída UMA linha com as características da imagem:
// nome largura altura vizinhos h[0] h[1] ... h[N-1]
// N = 256 (LBP8) ou 65536 (LBP16).
int gravar_caracteristicas(const char *caminho_saida, const char *nome_imagem, const PGMImage *img, int vizinhos, const unsigned int *histograma)
{
  FILE *f = fopen(caminho_saida, "a");   // "a" = acrescenta no final
  if (f == NULL)
  {
    fprintf(stderr, "Erro: Não foi possível abrir o arquivo de saída: %s\n", caminho_saida);
    return 0;
  }

  // parte fixa da linha
  fprintf(f, "%s %d %d %d", nome_imagem, img->largura, img->altura, vizinhos);

  // histograma: um valor por posição, separados por espaço
  int num_bins = (vizinhos == 8) ? 256 : 65536;
  for (int i = 0; i < num_bins; i++)
  {
    fprintf(f, " %u", histograma[i]);
  }
  fprintf(f, "\n");   // fim da linha desta imagem

  fclose(f);
  return 1;
}

// salva a imagem lbp (opção -i) em <dir_imagens>/<nome_original>,
// no formato PGM P5 (binário) com maxval 255.
int salvar_imagem_lbp(const char *dir_imagens, const char *nome_original, const PGMImage *img_lbp)
{
  // monta o caminho: pasta + "/" + nome do arquivo
  char caminho_completo[512];
  snprintf(caminho_completo, sizeof(caminho_completo), "%s/%s", dir_imagens, nome_original);

  FILE *f = fopen(caminho_completo, "wb");   // "wb" = escrita binária
  if (f == NULL)
  {
    fprintf(stderr, "Erro: Não foi possível criar a imagem LBP em %s\n", caminho_completo);
    return 0;
  }

  // cabeçalho: "P5", depois "largura altura", depois maxval
  fprintf(f, "P5\n%d %d\n%d\n", img_lbp->largura, img_lbp->altura, img_lbp->maxval);

  // corpo: 1 byte por pixel (os valores já estão em 0..255)
  for (int i = 0; i < img_lbp->altura; i++)
  {
    for (int j = 0; j < img_lbp->largura; j++)
    {
      fputc((int)img_lbp->pixels[i][j], f);
    }
  }

  int erro = ferror(f);   // verifica se houve falha de escrita (ex.: disco cheio)
  fclose(f);
  if (erro)
  {
    fprintf(stderr, "Erro ao gravar a imagem LBP em %s\n", caminho_completo);
    return 0;
  }
  return 1;
}
