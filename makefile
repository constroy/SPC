CXX=g++
CXXFLAGS=-std=c++14 -Wall
SRC=$(wildcard *.cpp)
HEAD=$(wildcard *.hpp)
OBJ=$(SRC:.cpp=.o)

spc: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

spc.o: parser.hpp analyser.hpp
error.o: error.hpp lexer.hpp
lexer.o: lexer.hpp error.hpp
parser.o: parser.hpp lexer.hpp error.hpp component.hpp
analyser.o: analyser.hpp component.hpp symtab.hpp
coder.o: coder.hpp lexer.hpp symtab.hpp optimizer.hpp
component.o: component.hpp lexer.hpp coder.hpp symtab.hpp
symtab.o: symtab.hpp error.hpp
optimizer.o: optimizer.hpp

clean:
	rm -f *.o *.gch $(EXEC)

.PHONY: clean