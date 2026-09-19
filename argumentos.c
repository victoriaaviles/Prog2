#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "argumentos.h"

int parse_argumentos(int argc, char **argv, Argumentos *args)
{
    args->diretorio = NULL;
    args->saida = NULL;
    args->vizinhos = 0;
    args->diretorio_imagens = NULL;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-d") == 0) 
        {
            if (i + 1 < argc) 
            {
                args->diretorio = argv[++i];
            }
            else 
            {
                fprintf(stderr, "Erro: A opção -d exige um argumento.\n");
                return 0;
            }
        }
        else if (strcmp(argv[i], "-o") == 0) 
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
        else if (strcmp(argv[i], "-n") == 0) 
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
        else if (strcmp(argv[i], "-i") == 0) 
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
            // Tratamento obrigatório para opções desconhecidas
            fprintf(stderr, "Erro: Opção desconhecida '%s'.\n", argv[i]);
            return 0;
        }
    }

    // Validação básica para garantir que os parâmetros obrigatórios foram preenchidos
    if (!args->diretorio || !args->saida || args->vizinhos == 0) 
    {
        fprintf(stderr, "Uso: %s -d <diretorio> -o <saida> -n <8|16> [-i <dir_imagens>]\n", argv[0]);
        return 0;
    }

    return 1; // Sucesso
}

