all:
	mkdir bin -p
	g++ -c src/const.cpp -I./includes -o bin/const.o
	g++ -c src/lexem.cpp -I./includes -o bin/lexem.o
	g++ -c src/lexical.cpp -I./includes -o bin/lexical.o
	g++ -c src/syntax.cpp -I./includes -o bin/syntax.o
	g++ -c src/semantic.cpp -I./includes -o bin/semantic.o
	g++ src/main.cpp bin/const.o bin/lexem.o bin/lexical.o bin/syntax.o bin/semantic.o -I./includes -o bin/main