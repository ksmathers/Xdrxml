CFLAGS=-I..

libdr1.a: map.o xdrxml.o strbuf.o xdrxmlsb.o qio.o
	ar rc libdr1.a map.o xdrxml.o strbuf.o xdrxmlsb.o qio.o

clean:
	rm *.o 

.c.o:
	gcc -g -c ${CFLAGS} $<

