# Nome do arquivo executável
EXECUTABLE = main

# Compilador
CC = g++

# Flags de compilação
CFLAGS = -std=c++11 -Wall -O

# Flags do linker
LDFLAGS = -lsndfile -lm -lsfml-audio -lsfml-system -lsfml-graphics -lsfml-window -pthread -lfftw3

# Arquivos fonte
SOURCES = main.cpp filters.cpp inputController.cpp outputController.cpp espectroBarras.cpp

# Arquivos objeto gerados
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
