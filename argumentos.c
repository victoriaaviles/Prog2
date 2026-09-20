#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "argumentos.h"

int parse_argumentos(int argc, char **argv, Argumentos *args) //argc = quantidade total de argumentos   argv = vetor que guarda essas strings
{
  args->diretorio = NULL;
  args->saida = NULL;
  args->vizinhos = 0;
  args->diretorio_imagens = NULL;

  for (int i = 1; i < argc; i++)     //percorre todos os argumentos passados na linha     argv[0] guarda o executavel
  {
    if (strcmp(argv[i], "-d") == 0)     //compara a string atual com -d
    {
      if (i + 1 < argc)    //verifica se há um próximo argumento
      {
        args->diretorio = argv[++i];    
      }
      else 
      {
        fprintf(stderr, "Erro: A opção -d exige um argumento.\n");
        return 0;
      }
    }

    else if (strcmp(argv[i], "-o") == 0)    //compara a string atual com -o
    {
      if (i + 1 < argc) 
      {
        args->saida = argv[++i];
      } 
      else 
      {
        fprintf(stderr, "Erro: A opção -o exige um argumento.\n");
        return 0;
      }
    } 

    else if (strcmp(argv[i], "-n") == 0)      //compara a string atual com -n
    {
      if (i + 1 < argc) 
      {
        args->vizinhos = atoi(argv[++i]);
        if (args->vizinhos != 8 && args->vizinhos != 16) 
        {
          fprintf(stderr, "Erro: Número de vizinhos deve ser 8 ou 16.\n");
          return 0;
        }
      } 
      else 
      {
        fprintf(stderr, "Erro: A opção -n exige um argumento.\n");
        return 0;
      }
    } 

    else if (strcmp(argv[i], "-i") == 0)     //compara a string atual com -i
    {
      if (i + 1 < argc) 
      {
        args->diretorio_imagens = argv[++i];
      } 
      else 
      {
        fprintf(stderr, "Erro: A opção -i exige um argumento.\n");
        return 0;
      }
    } 

    else 
    {
      // para opções desconhecidas
      fprintf(stderr, "Erro: Opção desconhecida '%s'.\n", argv[i]);
      return 0;
    }
  }
  
  // validação para garantir que os parâmetros obrigatórios foram preenchidos
  if (!args->diretorio || !args->saida || args->vizinhos == 0) 
  {
    fprintf(stderr, "Uso: %s -d <diretorio> -o <saida> -n <8|16> [-i <dir_imagens>]\n", argv[0]);
    return 0;
  }

  return 1; // sucesso
}