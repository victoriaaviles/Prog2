#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>   // isspace
#include <stdint.h>
#include "pgm.h"

// libera a matriz de pixels: primeiro cada linha, depois o vetor de ponteiros.
void pgm_liberar(PGMImage *img)
{
  if (img->pixels != NULL)
  {
    for (int i = 0; i < img->altura; i++)
    {
      free(img->pixels[i]);  // free(NULL) não faz nada, então é seguro mesmo
    }                        // se alguma linha não chegou a ser alocada
    free(img->pixels);
    img->pixels = NULL;      // evita liberar duas vezes por engano
  }
}

// avança no arquivo ignorando espaços em branco e comentários.
// comentário = do '#' até o fim da linha.
// retorna 1 se parou em um caractere útil (devolvido ao arquivo com ungetc), ou 0 se o arquivo acabou.
static int pular_espacos_e_comentarios(FILE *f)
{
  int c;
  while ((c = fgetc(f)) != EOF)
  {
    if (c == '#')
    {
      // descarta tudo até o fim da linha
      while ((c = fgetc(f)) != EOF && c != '\n')
      {
      }
    }
    else if (!isspace(c))
    {
      ungetc(c, f);  // devolve o caractere para ser lido pelo fscanf
      return 1;
    }
  }
  return 0;
}

// lê um inteiro do cabeçalho (largura, altura ou maxval), 
// ignorando antes espaços e comentários. Retorna 1 em sucesso.
static int ler_inteiro_cabecalho(FILE *f, int *valor)
{
  if (!pular_espacos_e_comentarios(f))
  {
    return 0;
  }
  return fscanf(f, "%d", valor) == 1;
}

// lê o corpo de um P5 (binário).
// maxval < 256  -> 1 byte por pixel
// maxval >= 256 -> 2 bytes por pixel, byte mais significativo primeiro
// retorna 1 em sucesso e 0 se o arquivo estiver truncado ou com valor inválido.
static int ler_corpo_p5(FILE *f, PGMImage *img)
{
  for (int i = 0; i < img->altura; i++)
  {
    for (int j = 0; j < img->largura; j++)
    {
      int c1 = fgetc(f);      // primeiro byte (ou único)
      if (c1 == EOF)
      {
        return 0;        // arquivo acabou antes do esperado
      }
      int valor = c1;
      if (img->maxval > 255)
      {
        int c2 = fgetc(f);        // segundo byte
        if (c2 == EOF)
        {
          return 0;
        }
        valor = (c1 << 8) | c2;   // junta: byte alto << 8 | byte baixo
      }
      if (valor > img->maxval)    // o enunciado exige valor em [0, maxval]
      {
        return 0;
      }
      img->pixels[i][j] = (uint16_t)valor;
    }
  }
  return 1;
}

// lê o corpo de um P2 (texto): números decimais separados por espaço/quebra de linha.
// o fscanf("%d") já pula qualquer espaço em branco entre os números.
static int ler_corpo_p2(FILE *f, PGMImage *img)
{
  for (int i = 0; i < img->altura; i++)
  {
    for (int j = 0; j < img->largura; j++)
    {
      int valor;
      // falha se não conseguiu ler um número ou se ele está fora de [0, maxval]
      if (fscanf(f, "%d", &valor) != 1 || valor < 0 || valor > img->maxval)
      {
        return 0;
      }
      img->pixels[i][j] = (uint16_t)valor;
    }
  }
  return 1;
}

// lê o arquivo pgm inteiro. Passos:
// 1) abre o arquivo  2) lê e valida o cabeçalho  3) aloca a matriz  4) lê o corpo
// se qualquer passo falhar, libera o que já alocou e retorna 0.
int pgm_ler(const char *caminho_arquivo, PGMImage *img)
{
  // estado inicial seguro: se falhar cedo, a struct não contém lixo
  img->pixels = NULL;
  img->largura = 0;
  img->altura = 0;
  img->maxval = 0;
  img->tipo[0] = '\0';

  // "rb" = leitura binária (necessário para o P5)
  FILE *f = fopen(caminho_arquivo, "rb");
  if (f == NULL)
  {
    fprintf(stderr, "Erro ao abrir a imagem: %s\n", caminho_arquivo);
    return 0;
  }

  // 1) número mágico ("P2" ou "P5"); %2s lê no máximo 2 caracteres (cabe em tipo[3])
  if (!pular_espacos_e_comentarios(f) || fscanf(f, "%2s", img->tipo) != 1)
  {
    fprintf(stderr, "Arquivo nao e um PGM valido: %s\n", caminho_arquivo);
    fclose(f);
    return 0;
  }

  if (strcmp(img->tipo, "P2") != 0 && strcmp(img->tipo, "P5") != 0)
  {
    fprintf(stderr, "Formato PGM desconhecido (%s): %s\n", img->tipo, caminho_arquivo);
    fclose(f);
    return 0;
  }

  // 2) largura, altura e maxval (comentários permitidos entre eles)
  int largura, altura, maxval;
  if (!ler_inteiro_cabecalho(f, &largura) ||
      !ler_inteiro_cabecalho(f, &altura) ||
      !ler_inteiro_cabecalho(f, &maxval))
  {
    fprintf(stderr, "Cabecalho PGM invalido: %s\n", caminho_arquivo);
    fclose(f);
    return 0;
  }

  // validação antes de alocar (evita malloc com tamanho absurdo/negativo)
  if (largura <= 0 || altura <= 0 || maxval <= 0 || maxval > 65535)
  {
    fprintf(stderr, "Dimensoes ou MAXVAL invalidos: %s\n", caminho_arquivo);
    fclose(f);
    return 0;
  }

  // o cabeçalho termina com um caractere de espaço/quebra de linha após o maxval.
  // precisa ser consumido para que o primeiro byte lido do P5 seja o primeiro pixel.
  fgetc(f);

  img->largura = largura;
  img->altura = altura;
  img->maxval = maxval;

  // 3) alocação da matriz altura x largura
  // calloc zera os ponteiros (NULL); assim, se uma linha falhar no meio,
  // pgm_liberar consegue limpar o que já foi alocado sem acessar lixo.
  img->pixels = (uint16_t **)calloc((size_t)altura, sizeof(uint16_t *));
  if (img->pixels == NULL)
  {
    fprintf(stderr, "Erro de alocacao de memoria\n");
    fclose(f);
    return 0;
  }
  for (int i = 0; i < altura; i++)
  {
    img->pixels[i] = (uint16_t *)malloc((size_t)largura * sizeof(uint16_t));
    if (img->pixels[i] == NULL)
    {
      fprintf(stderr, "Erro de alocacao de memoria\n");
      pgm_liberar(img);   // libera as linhas já alocadas
      fclose(f);
      return 0;
    }
  }

  // 4) leitura do corpo conforme o tipo
  int ok = (strcmp(img->tipo, "P5") == 0) ? ler_corpo_p5(f, img) : ler_corpo_p2(f, img);
  fclose(f);

  if (!ok)
  {
    fprintf(stderr, "Corpo do PGM invalido ou truncado: %s\n", caminho_arquivo);
    pgm_liberar(img);
    return 0;
  }

  return 1;
}
