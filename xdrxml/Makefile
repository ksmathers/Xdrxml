all: xtc

CFLAGS=-DYYDEBUG -g -Wall

clean:
	rm -f lex.yy.c
	rm -f y.tab.c
	rm -f y.tab.h
	rm -f *.o

xtc: xtcc.o y.tab.o lex.yy.o nametable.o
	gcc -g xtcc.o y.tab.o lex.yy.o nametable.o -lfl -oxtc

xtcc.o: y.tab.h

y.tab.h: y.tab.c

y.tab.o: y.tab.c y.tab.h

lex.yy.o: lex.yy.c y.tab.h

y.tab.c: xtc.y
	yacc -d xtc.y

lex.yy.c: xtc.l
	lex xtc.l
	
