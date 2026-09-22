#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pgm.h"

// função para pular linhas de comentário iniciadas por #
static void pular_comentarios(FILE *f) 
{
  int c;
  while ((c = fgetc(f)) != EOF) 
  {
    if (c == '#') 
    {
      while ((c = fgetc(f)) != EOF && c != '\n');    // ignora tudo até o final da linha
    } 
    else if (c == ' ' || c == '\t' || c == '\n' || c == '\r')   // ignora espaços em branco antes de novos dados ou comentários
    {
      continue;
    } 
    else 
    {
    // encontrou um caractere normal, devolve para o fluxo do arquivo
      ungetc(c, f);
      break;
    }
  }
}

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
  FILE *f = fopen(caminho_arquivo, "r");
  if (f == NULL) 
  {
    fprintf(stderr, "Erro: Não foi possível abrir o arquivo %s\n", caminho_arquivo);
    return 0;
  }

  // Lê o tipo do arquivo (P2 ou P5)
  if (fscanf(f, "%2s", img->tipo) != 1) 
  {
    fprintf(stderr, "Erro: Arquivo PGM inválido (falha ao ler o tipo).\n");
    fclose(f);
    return 0;
  }

  // Valida se o formato corresponde ao padrão PGM suportado
  if (strcmp(img->tipo, "P2") != 0 && strcmp(img->tipo, "P5") != 0) 
  {
    fprintf(stderr, "Erro: Formato PGM desconhecido (%s). Permitidos: P2 ou P5.\n", img->tipo);
    fclose(f);
    return 0;
  }

  // Lê a largura, altura e maxval ignorando os comentários pelo caminho
  pular_comentarios(f);
  if (fscanf(f, "%d", &img->largura) != 1) 
  {
    fprintf(stderr, "Erro: Falha ao ler a largura da imagem.\n");
    fclose(f);
    return 0;
  }

  pular_comentarios(f);
  if (fscanf(f, "%d", &img->altura) != 1) 
  {
    fprintf(stderr, "Erro: Falha ao ler a altura da imagem.\n");
    fclose(f);
    return 0;
  }

  pular_comentarios(f);
  if (fscanf(f, "%d", &img->maxval) != 1) 
  {
    fprintf(stderr, "Erro: Falha ao ler o MAXVAL da imagem.\n");
    fclose(f);
    return 0;
  }

  // validações do maxval
  if (img->maxval <= 0 || img->maxval >= 65536) 
  {
    fprintf(stderr, "Erro: MAXVAL inválido (%d). Deve estar entre 1 e 65535.\n", img->maxval);
    fclose(f);
    return 0;
  }

  // consome o caractere de espaçamento após o maxval antes de ler os pixels
  fgetc(f);

  // Alocação dinâmica da matriz de pixels [altura][largura]
  img->pixels = (unsigned char **)malloc(img->altura * sizeof(unsigned char *));
  if (img->pixels == NULL) 
  {
    fprintf(stderr, "Erro: Falha de alocação de memória para as linhas da imagem.\n");
    fclose(f);
    return 0;
  }

  for (int i = 0; i < img->altura; i++) 
  {
    img->pixels[i] = (unsigned char *)malloc(img->largura * sizeof(unsigned char));
    if (img->pixels[i] == NULL) 
    {
      fprintf(stderr, "Erro: Falha de alocação de memória para as colunas da imagem.\n");

      // libera o que já foi alocado antes de sair
      for (int j = 0; j < i; j++) free(img->pixels[j]);
      free(img->pixels);
      fclose(f);
      return 0;
    }
  }

  // Leitura dos dados do corpo da imagem
  if (strcmp(img->tipo, "P2") == 0) 
  {
    // texto: lê valores decimais separados por espaços
    for (int i = 0; i < img->altura; i++) 
    {
      for (int j = 0; j < img->largura; j++) 
      {
        int pixel_val;
        if (fscanf(f, "%d", &pixel_val) != 1) 
        {
          fprintf(stderr, "Erro: Fim inesperado do arquivo P2.\n");
          pgm_liberar(img);
          fclose(f);
          return 0;
        }
        img->pixels[i][j] = (unsigned char)pixel_val;
      }
    }
  } 
  else 
  {
    // binário: fecha o arquivo em modo texto e reabre em binário se necessário, ou lê diretamente os bytes do buffer se a posição estiver alinhada.
    fclose(f);
    // abre novamente em modo binário para ler o bloco de pixels corretamente
    FILE *fb = fopen(caminho_arquivo, "rb");
  }

  fclose(f);
  return 1;
}