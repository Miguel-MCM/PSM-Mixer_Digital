# Nome do arquivo executável
EXECUTABLE = main

# Compilador
CC = g++

# Flags de compilação
CFLAGS = -std=c++11 -Wall

# Flags do linker
LDFLAGS = -lsndfile -lm -lsfml-audio -lsfml-system -pthread

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
