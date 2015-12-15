CXX=g++
CXXFLAGS=-std=c++14
SRC=$(wildcard *.cxx)
HEAD=$(wildcard *.hxx)
OBJ=$(SRC:.cxx=.o)
EXEC=spc

$(EXEC): $(OBJ)
	$(CXX) $(CXXFLAGS) $(LIBS) -o $(EXEC) $(OBJ)

$(OBJ): $(HEAD) $(SRC)
	$(CXX) -c $(CXXFLAGS) $(SRC)

clean:
	rm -f *.o *.gch $(EXEC)
