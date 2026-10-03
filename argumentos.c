#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "argumentos.h"

// mostra como usar o programa. 'saida' é stdout (para -h) ou stderr (para erros).
static void imprimir_uso(const char *programa, FILE *saida)
{
  fprintf(saida, "Uso: %s -d <diretorio> -o <saida> -n <8|16> [-i <dir_imagens>] [-h]\n", programa);
  fprintf(saida, "  -d  diretório contendo as imagens PGM\n");
  fprintf(saida, "  -n  número de vizinhos do LBP (8 ou 16)\n");
  fprintf(saida, "  -o  arquivo texto onde serão armazenadas as características\n");
  fprintf(saida, "  -i  diretório para geração opcional das imagens LBP\n");
  fprintf(saida, "  -h  apresenta esta ajuda\n");
}

// interpreta a linha de comando.
// argc = quantidade de argumentos; argv = vetor de strings (argv[0] é o executável).
// preenche 'args'. A ordem das opções é livre.
// retorno: 1 = sucesso, 0 = erro, 2 = usuário pediu ajuda (-h).
// observação: os campos de 'args' apontam para strings do próprio argv (nada é alocado aqui, então não há nada para liberar).

int parse_argumentos(int argc, char **argv, Argumentos *args)
{
  // valores iniciais: NULL / 0 significam "não informado"
  args->diretorio = NULL;
  args->saida = NULL;
  args->vizinhos = 0;
  args->diretorio_imagens = NULL;

  for (int i = 1; i < argc; i++)   // começa em 1 para pular o nome do executável
  {
    if (strcmp(argv[i], "-h") == 0)
    {
      imprimir_uso(argv[0], stdout);
      return 2;
    }
    else if (strcmp(argv[i], "-d") == 0)
    {
      // a opção exige um valor logo depois dela
      if (i + 1 >= argc)
      {
        fprintf(stderr, "Erro: A opção -d exige um argumento.\n");
        return 0;
      }
      args->diretorio = argv[++i];   // ++i: avança para o valor e já o consome
    }
    else if (strcmp(argv[i], "-o") == 0)
    {
      if (i + 1 >= argc)
      {
        fprintf(stderr, "Erro: A opção -o exige um argumento.\n");
        return 0;
      }
      args->saida = argv[++i];
    }
    else if (strcmp(argv[i], "-n") == 0)
    {
      if (i + 1 >= argc)
      {
        fprintf(stderr, "Erro: A opção -n exige um argumento.\n");
        return 0;
      }
      // strtol converte texto em número e diz onde parou ('fim').
      // Se *fim != '\0', sobrou texto inválido (ex.: "8abc"); o atoi aceitaria isso.
      char *fim;
      long valor = strtol(argv[++i], &fim, 10);
      if (*fim != '\0' || (valor != 8 && valor != 16))
      {
        fprintf(stderr, "Erro: valor inválido para -n. Valores permitidos: 8 ou 16.\n");
        return 0;
      }
      args->vizinhos = (int)valor;
    }
    else if (strcmp(argv[i], "-i") == 0)
    {
      if (i + 1 >= argc)
      {
        fprintf(stderr, "Erro: A opção -i exige um argumento.\n");
        return 0;
      }
      args->diretorio_imagens = argv[++i];
    }
    else
    {
      fprintf(stderr, "Erro: Opção desconhecida '%s'.\n", argv[i]);
      return 0;
    }
  }

  // -d, -o e -n são obrigatórios; -i é opcional
  if (!args->diretorio || !args->saida || args->vizinhos == 0)
  {
    imprimir_uso(argv[0], stderr);
    return 0;
  }

  return 1;
}
