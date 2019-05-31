all: 
	g++ -c psin.cpp
	g++ -c arvore.cpp	
	g++ -c arvoreb.cpp
	g++ -c arvoreb2.cpp
	g++ -c analise.cpp
	g++ -c tp.cpp
	g++ psin.o arvore.o arvoreb.o arvoreb2.o analise.o tp.o -Wall -o tp

clean:
	rm -f *.o 
	rm tp

run:
	./tp 1 100 1 50
