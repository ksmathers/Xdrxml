XML2CFLAGS=-I/usr/include/libxml2
CFLAGS=-I../lib ${XML2CFLAGS}

OBJS=set.o map.o xdrxml.o strbuf.o xdrxmlsb.o stream.o \
     player.o class.o armor.o weapon.o registry.o \
     item.o attr.o itemset.o location.o dice.o \
     money.o itemreg.o potions/heal.o monster.o race.o


libdr1.a: ${OBJS}
	ar rc libdr1.a ${OBJS}

clean:
	rm *.o 

.c.o:
	gcc -g -c ${CFLAGS} $< -o $@

