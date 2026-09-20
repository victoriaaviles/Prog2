#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include "argumentos.h"
#include "diretorio.h"

int processar_diretorio(const char *caminho_dir, Argumentos *args) 
{
  DIR *dir = opendir(caminho_dir);     //abre o diretorio

  if (dir == NULL) 
  {
    fprintf(stderr, "Erro: Diretório inexistente ou sem permissão: %s\n", caminho_dir);
    return 0;
  }

  struct dirent *entrada;

  while ((entrada = readdir(dir)) != NULL)    // percorre todos os arquivos e pastas encontrados dentro daquele diretorio
  {
    if (strcmp(entrada->d_name, ".") == 0 || strcmp(entrada->d_name, "..") == 0)   // ignora diretorios . e ..
    {
      continue;
    }
  
    // o arquivo termina com a extensão ".pgm"?
    char *extensao = strrchr(entrada->d_name, '.');      //encontra o "." no nome do arquivo
    if (extensao != NULL && strcmp(extensao, ".pgm") == 0)   // verifica se a extensao é .pgm
    {
      printf("Arquivo PGM encontrado: %s\n", entrada->d_name);    // nome do arquivo pgm encontrado
    }
  } 
  
  closedir(dir);
  return 1;
}