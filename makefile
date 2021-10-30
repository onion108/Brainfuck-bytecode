VMDIR=./src/vm
COMPILERDIR=./src/compiler
clean:
	rm -rf ./bin/*
vm:
	g++ -o ./bin/bfvm -std=c++11 -lstdc++ $(VMDIR)/main.cpp
compiler:
	g++ -o ./bin/bfc -std=c++11 -lstdc++ $(COMPILERDIR)/main.cpp
build: clean vm compiler

