CFLAGS=-I..

libdr1.a: map.o xdrxml.o strbuf.o xdrxmlsb.o stream.o
	ar rc libdr1.a map.o xdrxml.o strbuf.o xdrxmlsb.o stream.o

clean:
	rm *.o 

.c.o:
	gcc -g -c ${CFLAGS} $<

