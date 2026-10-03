# Prog2
TRABALHO PRÁTICO: Descritor LBP (Local Binary Pattern)
Disciplina: CI1002 Programação 2 - 2026/2
============================================================

1. AUTORIA
----------
- Nome: María Victoria Aviles
- RA: GRR20250429

2. LISTA DE FICHEIROS E DIRETÓRIOS
----------------------------------
O trabalho está organizado da seguinte forma:

- LEIAME: Este documento descritivo do trabalho
- src/: Diretório contendo o código principal em linguagem C e o Makefile.
  - main.c: Módulo principal que coordena o fluxo de execução.
  - argumentos.c / argumentos.h: Tratamento, validação e armazenamento dos argumentos de linha de comando (argc/argv).
  - diretorio.c / diretorio.h: Gestão e localização de ficheiros PGM no diretório especificado.
  - pgm.c / pgm.h: Leitura, alocação dinâmica e gestão de memória para imagens PGM (suporte a P2 e P5).
  - lbp.c / lbp.h: Implementação do cálculo LBP (8,1 e 16,2) e geração dos histogramas.
  - saida.c / saida.h: Gravação do ficheiro de características textuais e das imagens LBP opcionais.
  - Makefile: Regras de compilação modular e automatizada.
- bin/: Diretório de destino onde o executável é instalado após a compilação[cite: 10, 11].

3. COMO COMPILAR O CÓDIGO-FONTE
-------------------------------
Para compilar o projeto de forma limpa e correta, utilize o Makefile localizado na pasta src/ com as seguintes regras obrigatórias:
- make tudo: Compila todos os módulos individualmente, gera o programa executável e instala-o no diretório bin/
- make limpa: Remove os ficheiros temporários de objeto (*.o) gerados durante a compilação.
- make faxina: Remove todos os ficheiros temporários, objetos e executáveis, limpando totalmente o ambiente de trabalho.

Para executar o programa após compilado, utilize por exemplo:
./bin/lbp -d ./dataset -n 8 -o resultado_8.txt -i ./lbp_images_8
./bin/lbp -d ./dataset -n 8 -o resultado_8.txt -i ./lbp_images_16

*Nota importante sobre a visualização das imagens geradas:* Como as imagens LBP são guardadas no formato binário (P5), os editores de texto (como o VS Code) costumam bloquear a abertura direta exibindo um aviso de ficheiro binário. Para inspecionar e visualizar corretamente as imagens geradas, a forma mais prática é abri-las diretamente pelo terminal usando o visualizador nativo com o comando:

`open ./nomedapasta/nomedaimagem` (substituindo pelo caminho da pasta e da imagem desejadas).

4. ALGORITMOS, ESTRUTURAS DE DADOS E ESCOLHAS TÉCNICAS
-------------------------------------------------------
- Algoritmos Utilizados: O programa implementa o descritor de textura LBP para 8 vizinhos com raio 1 (LBP(8,1)) e 16 vizinhos com raio 2 (LBP(16,2)). Os pixels de borda que não possuem todos os vizinhos necessários recebem o valor zero. No caso do LBP(16,2) para visualização de imagens, aplica-se a normalização dividindo por 257 para ajustar os valores para o intervalo [0,255].
- Estruturas de Dados: Foi utilizado uma estrutura modular para representar as imagens PGM (armazenando largura, altura, maxval e a matriz de píxeis alocada dinamicamente). Os histogramas são alocados dinamicamente com 256 posições para 8 vizinhos e 65.536 posições para 16 vizinhos.
- Alternativas e Dificuldades: A minha maior dificuldade no início foi a leitura das imagens. O meu código estava falhando porque tentava ler tudo como texto (P2), mas as imagens do dataset afinal eram binárias (P5). Tive de reajustar a função de leitura (`pgm_ler`) para verificar o cabeçalho e usar `fread` quando fosse binário. 

5. BUGS CONHECIDOS
------------------
Não foram identificados bugs ou falhas de segmentação (*Segmentation fault*) nos testes padrão realizados. O programa faz a validação rigorosa de argumentos inválidos e diretórios inexistentes.

6. OUTRAS INFORMAÇÕES
---------------------
O projeto cumpre integralmente os requisitos de modularização em C, boas práticas com warnings rigorosos ativados (`-Wall -Wextra -Wpedantic -std=c11`) e tratamento adequado de erros.