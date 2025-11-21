CPPFLAGS=-g -pthread -I/sw/include/root

main: main.o
	g++ -o main main.o

main.o:
	g++ $(CPPFLAGS) -c main.cxx

