#ifndef SAIDA_H
#define SAIDA_H

#include "pgm.h"
#include "argumentos.h"

// grava as características (histograma) da imagem processada no arquivo de saída (-o)
int gravar_caracteristicas(const char *caminho_saida, const char *nome_imagem, const PGMImage *img, int vizinhos, const unsigned int *histograma);

// salva a imagem lbp gerada no diretório opcional (-i)
int salvar_imagem_lbp(const char *dir_imagens, const char *nome_original, const PGMImage *img_lbp);

#endif
