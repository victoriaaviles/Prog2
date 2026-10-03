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

  // interpreta e valida a linha de comando
  int resultado = parse_argumentos(argc, argv, &args);
  if (resultado == 2)   // -h: a ajuda já foi mostrada, não é erro
  {
    return 0;
  }
  if (!resultado)   // argumentos inválidos: mensagem já foi impressa
  {
    return 1;
  }

  // processa todas as imagens .pgm do diretório
  if (!processar_diretorio(args.diretorio, &args))
  {
    return 1;
  }

  return 0;   // sucesso
}
