all: directories basic_interpreter

directories: bin

bin:
	mkdir -p bin

basic_interpreter: src/main.cpp bin/arithmetic.o bin/boolean.o bin/command.o bin/interpreter.o
	g++ -g -Wall src/main.cpp bin/command.o bin/interpreter.o bin/arithmetic.o bin/boolean.o -o basic_interpreter

bin/arithmetic.o: lib/arithmetic.h src/arithmetic.cpp
	g++ -g -Wall -c -std=c++11 src/arithmetic.cpp -o bin/arithmetic.o

bin/boolean.o: lib/boolean.h src/boolean.cpp
	g++ -g -Wall -c -std=c++11 src/boolean.cpp -o bin/boolean.o

bin/command.o: lib/command.h src/command.cpp
	g++ -g -Wall -c -std=c++11 src/command.cpp -o bin/command.o

bin/interpreter.o: lib/interpreter.h src/interpreter.cpp
	g++ -g -Wall -c -std=c++11 src/interpreter.cpp -o bin/interpreter.o
