
CC = gcc

CFLAGS=`gtk-config --cflags`  `gnome-config gnome --cflags` `libglade-config gnome --cflags`
LIBS=`libglade-config gnome --libs`

all: tglade

tglade: tglade.o
	$(CC) -o tglade tglade.o $(LIBS)

tglade.o: tglade.c

helloworld: helloworld.c 
	$(CC) `gtk-config --cflags`  helloworld.c -o helloworld `gtk-config --libs`

clean: 
	rm -f *.o helloworld
