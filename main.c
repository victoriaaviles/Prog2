#include <stdio.h>
#include <stdlib.h>
#include "argumentos.h"
#include "diretorio.h"
#include "pgm.h"
#include "lbp.h"
#include "saida.h"

int main(int argc, char **argv) 
{
  Argumentos args;

  // interpreta e valida os parâmetros do terminal
  if (!parse_argumentos(argc, argv, &args)) 
  {
    return 1;
  }

  // processa o diretório de imagens e executa o fluxo principal 
  // (a lógica de varredura chamará a leitura pgm e o cálculo lbp para cada arquivo)

  return 0;
}