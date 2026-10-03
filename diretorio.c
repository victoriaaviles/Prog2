#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>   // opendir, readdir, closedir
#include "argumentos.h"
#include "diretorio.h"
#include "pgm.h"
#include "lbp.h"
#include "saida.h"

#define TAM_HISTOGRAMA 65536  // tamanho máximo: serve para LBP8 (usa 256) e LBP16 (usa 65536)

// percorre o diretório e, para cada arquivo .pgm:
// lê a imagem -> calcula o LBP e o histograma -> grava a linha no arquivo de saída -> (se -i) 
// salva a imagem LBP -> libera a memória da imagem.
// retorna 1 em sucesso e 0 em erro.
int processar_diretorio(const char *caminho_dir, Argumentos *args)
{
  // abre o diretório; NULL se não existe ou não há permissão
  DIR *dir = opendir(caminho_dir);
  if (dir == NULL)
  {
    fprintf(stderr, "Erro: Diretório inexistente ou sem permissão: %s\n", caminho_dir);
    return 0;
  }

  // cria/esvazia o arquivo de saída (garante uma linha por imagem)
  if (!criar_arquivo_saida(args->saida))
  {
    closedir(dir);
    return 0;
  }

  // o histograma é alocado dinamicamente uma vez e reaproveitado para todas as
  // imagens (o calcular_lbp_imagem zera o conteúdo a cada chamada).
  unsigned int *histograma = malloc(TAM_HISTOGRAMA * sizeof(unsigned int));
  if (histograma == NULL)
  {
    fprintf(stderr, "Erro: Sem memória para o histograma.\n");
    closedir(dir);
    return 0;
  }

  // 1 se a opção -i foi informada (gerar imagens lbp), 0 caso contrário
  int gerar_imagem = (args->diretorio_imagens != NULL) ? 1 : 0;
  struct dirent *entrada;

  // readdir devolve uma entrada por chamada e NULL quando acabam
  while ((entrada = readdir(dir)) != NULL)
  {
    // ignora as entradas especiais "." e ".."
    if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0)
    {
      continue;
    }

    // só interessam arquivos terminados em ".pgm"
    char *extensao = strrchr(entrada->d_name, '.');   // última ocorrência de '.'
    if (extensao == NULL || strcmp(extensao, ".pgm") != 0)
    {
      continue;
    }

    // monta o caminho completo (ex.: ./imagens/foto01.pgm)
    char caminho_imagem[512];
    snprintf(caminho_imagem, sizeof(caminho_imagem), "%s/%s", caminho_dir, entrada->d_name);

    PGMImage img;       // imagem original
    PGMImage img_lbp;   // imagem LBP (só usada com -i)
    memset(&img_lbp, 0, sizeof(img_lbp));  // zera tudo: nenhum campo fica com lixo

    // se a leitura falhar, pgm_ler já imprimiu o erro e não deixou nada alocado
    if (!pgm_ler(caminho_imagem, &img))
    {
      continue;   // segue para a próxima imagem
    }

    // calcula o lbp; se der certo, grava os resultados
    if (calcular_lbp_imagem(&img, args->vizinhos, histograma, &img_lbp, gerar_imagem))
    {
      // acrescenta a linha desta imagem no arquivo de saída (-o)
      gravar_caracteristicas(args->saida, entrada->d_name, &img, args->vizinhos, histograma);

      // se -i foi usado, salva a imagem lbp e libera a memória dela
      if (gerar_imagem)
      {
        salvar_imagem_lbp(args->diretorio_imagens, entrada->d_name, &img_lbp);
        pgm_liberar(&img_lbp);
      }
    }

    pgm_liberar(&img);   // libera a imagem original
  }

  free(histograma);
  closedir(dir);
  return 1;
}
