#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm.h"

void pgm_liberar(PGMImage *img) 
{
  if (img->pixels != NULL) 
  {
    for (int i = 0; i < img->altura; i++) 
    {
      free(img->pixels[i]);
    }
    free(img->pixels);
    img->pixels = NULL;
  }
}

int pgm_ler(const char *caminho_arquivo, PGMImage *img) 
{
  // abrimos com "rb" (read binary) para suportar ficheiros P5
  FILE *f = fopen(caminho_arquivo, "rb");
  if (f == NULL) 
  {
    fprintf(stderr, "Erro ao abrir a imagem: %s\n", caminho_arquivo);
    return 0;
  }

  // lê o cabeçalho
  fscanf(f, "%s", img->tipo);
  fscanf(f, "%d %d", &img->largura, &img->altura);
    
  int maxval;
  fscanf(f, "%d", &maxval);
  img->maxval = maxval;

  // precisamos consumir esse byte antes de ler os dados binários do P5.
  fgetc(f);

  // aloca a memória para a matriz de píxeis da imagem
  img->pixels = (unsigned char **)malloc(img->altura * sizeof(unsigned char *));
  for (int i = 0; i < img->altura; i++) 
  {
    img->pixels[i] = (unsigned char *)malloc(img->largura * sizeof(unsigned char));
  }

  // ascolhe a leitura com base no tipo
  if (strcmp(img->tipo, "P5") == 0) 
  {
    // leitura Binária (P5) lê a linha inteira de uma vez em bytes
    for (int i = 0; i < img->altura; i++) 
    {
      fread(img->pixels[i], sizeof(unsigned char), img->largura, f);
    }
  } 
  else if (strcmp(img->tipo, "P2") == 0) 
  {
    // leitura em Texto (P2)
    for (int i = 0; i < img->altura; i++) 
    {
      for (int j = 0; j < img->largura; j++) 
      {
        int valor_pixel;
        fscanf(f, "%d", &valor_pixel);
        img->pixels[i][j] = (unsigned char)valor_pixel;
      }
    }
  } 
  else 
  {
    fprintf(stderr, "Formato PGM nao suportado: %s\n", img->tipo);
    fclose(f);
    return 0;
  }

  fclose(f);
  return 1;
}
