VMDIR=./src/vm
COMPILERDIR=./src/compiler
ECOMPILERDIR=./src/e-compiler
clean:
	rm -rf ./bin/*
vm:
	g++ -o ./bin/bfvm -std=c++11 -lstdc++ $(VMDIR)/main.cpp
compiler:
	g++ -o ./bin/bfc -std=c++11 -lstdc++ $(COMPILERDIR)/main.cpp
e-compiler:
	g++ -o ./bin/ebfc -std=c++11 -lstdc++ $(ECOMPILERDIR)/main.cpp
build: clean vm compiler

