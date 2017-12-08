IDIR = .
CXX = g++
CFLAGS=-I $(IDIR)

ODIR=obj

CXXFLAGS += -g -Wall -Wextra -pthread -Weffc++

MAIN = run

DEPS = commom.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = btree.o main.o nome.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: run

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CFLAGS)


run: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(MAIN) $^ $(CFLAGS)
