CFLAGS=-I..

libdr1.a: map.o xdrxml.o
	ar rc libdr1.a map.o xdrxml.o

.c.o:
	gcc -g -c ${CFLAGS} $<

