all: 
	mkdir bin
	g++ home/interpreter.cpp home/lexem.h -o bin/interpreter

clean:
	rm bin/interpreter
	rmdir bin