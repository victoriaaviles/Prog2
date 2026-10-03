# Compilador e opções (as mesmas exigidas no enunciado)
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11

# Lista de arquivos fonte e de objetos correspondentes
SRC = main.c argumentos.c diretorio.c pgm.c lbp.c saida.c
OBJ = $(SRC:.c=.o)
TARGET = lbp

# Todos os cabeçalhos: se algum .h mudar, os módulos são recompilados
HEADERS = $(wildcard *.h)

# Regras que não geram arquivos com esse nome
.PHONY: tudo limpa faxina

# Regra TUDO: compila e instala (copia o executável para ../bin)
tudo: $(TARGET)
	@mkdir -p ../bin
	cp $(TARGET) ../bin/

# Regra de linkedição do executável principal
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

# Regra genérica para gerar os arquivos .o a partir dos .c (e dos .h)
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra LIMPA: remove os arquivos temporários (.o)
limpa:
	rm -f *.o

# Regra FAXINA: remove os temporários e os arquivos gerados (executável e cópia em ../bin)
faxina: limpa
	rm -f