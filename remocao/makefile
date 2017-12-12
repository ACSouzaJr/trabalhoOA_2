IDIR = .
CXX = g++
CFLAGS=-I $(IDIR)

ODIR=obj

CXXFLAGS += -g -Wall -Wextra -pthread -Weffc++ -std=c++11

MAIN = run

DEPS = commom.h printer.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = btree.o main.o nome.o operacoesregistro.o printer.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: run

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CFLAGS)


run: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(MAIN) $^ $(CFLAGS)

clean:
	-rm ./obj/*.o
	-rm run	
