auto: test.c test.h auto.c
	gcc auto.c test.c -I.. ../xdrxml.c -lxml -oauto

test.c: test.x
	../xtc test.x

clean:
	rm auto test.c test.h
