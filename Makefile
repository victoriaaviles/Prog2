# Compilador
CC = gcc

CFLAGS = -Wall -Wextra -Wpedantic -std=c11

# Lista de arquivos fonte e de objetos correspondentes
SRC = main.c argumentos.c diretorio.c pgm.c lbp.c saida.c
OBJ = $(SRC:.c=.o)
TARGET = lbp

# Regra TUDO: compila e instala
tudo: $(TARGET)
	@mkdir -p ../bin
	cp $(TARGET) ../bin/

# Regra de linkedição do executável principal
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

# Regra genérica para gerar os arquivos .o a partir dos .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra clean: limpa os arquivos temporários (.o)
clean:
	rm -f *.o

# Regra clean all: limpa todos os temporários e executáveis gerados
clean all: limpa
	rm -f $(TARGET) ../bin/$(TARGET)