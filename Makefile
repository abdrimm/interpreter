all: 
	mkdir bin
	g++ home/1_arithmetic.cpp home/lexem.h -o bin/1_arithmetic

clean:
	rm bin/1_arithmetic
	rmdir bin