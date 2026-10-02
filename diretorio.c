#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include "argumentos.h"
#include "diretorio.h"
#include "pgm.h"
#include "lbp.h"
#include "saida.h"

int processar_diretorio(const char *caminho_dir, Argumentos *args) 
{
  // a linha (void)args; foi removida porque agora usaremos a variável args
  
  DIR *dir = opendir(caminho_dir);

  if (dir == NULL) 
  {
    fprintf(stderr, "Erro: Diretório inexistente ou sem permissão: %s\n", caminho_dir);
    return 0;
  }

  struct dirent *entrada;

  while ((entrada = readdir(dir)) != NULL)
  {
    if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0)
    {
      continue;
    }
  
    char *extensao = strrchr(entrada->d_name, '.');
    if (extensao != NULL && strcmp(extensao, ".pgm") == 0)
    {
      // monta o caminho completo do arquivo (ex: ./dataset/imagem01.pgm)
      char caminho_imagem[512];
      snprintf(caminho_imagem, sizeof(caminho_imagem), "%s/%s", caminho_dir, entrada->d_name);
      
      PGMImage img;
      
      // lê a imagem original PGM
      if (pgm_ler(caminho_imagem, &img)) 
      {
        // prepara o histograma (65536 posições suportam tanto LBP8 quanto LBP16)
        unsigned int *histograma = calloc(65536, sizeof(unsigned int));
        PGMImage img_lbp;
          
        // verifica se a opção -i foi informada para gerar as imagens de visualização
        int gerar_imagem = (args->diretorio_imagens != NULL) ? 1 : 0;
          
        // calcula o LBP e preenche o histograma
        if (calcular_lbp_imagem(&img, args->vizinhos, histograma, &img_lbp, gerar_imagem))
        {
          // grava os dados no arquivo de texto -o
          gravar_caracteristicas(args->saida, entrada->d_name, &img, args->vizinhos, histograma);
              
          // salva a nova imagem se a opção -i foi ativada
          if (gerar_imagem) 
          {
            salvar_imagem_lbp(args->diretorio_imagens, entrada->d_name, &img_lbp);
            pgm_liberar(&img_lbp); // libera a memória da imagem lbp recém-criada
          }
        }
          
        free(histograma);
        pgm_liberar(&img); // libera a memória da imagem original
      }
    }
  } 
  
  closedir(dir);
  return 1;
}
