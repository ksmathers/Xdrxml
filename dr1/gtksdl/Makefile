
EXECNAME	=	gnome_app

OBJS		=	gnome_app.o gtksdl.o

OPTIMIZATION	=	-O2
#DEBUG		=	-g -ggdb

DEBUGLIB	=	-lgnomeui -lart_lgpl -lSM -lICE -lgtk_g -lgdk -lgmodule -lXi -lXext -lX11 -lgnome -lgnomesupport -lesd -laudiofile -lm -ldb -lglib -ldl `sdl-config --libs`

LIB		=	`gnome-config --libs gtk gnome gnomeui` `sdl-config --libs`

CFLAGS=-c -Wall $(DEBUG) $(OPTIMIZATION) `gnome-config --cflags gtk gnome gnomeui` `sdl-config --cflags`
LDFLAGS=-Wall $(DEBUG)  $(LIB)

CC = gcc
RM = rm -f

all: $(OBJS)

	$(CC) $(OBJS) $(LDFLAGS) -o $(EXECNAME)

gnome_app.o : gnome_app.c gtksdl.h

	$(CC) $< $(CFLAGS)

gtksdl.o : gtksdl.c gtksdl.h

	$(CC) $< $(CFLAGS)

clean:
	$(RM) $(OBJS) core $(EXECNAME) $(SESSION)

depend:
	echo "No dependences"
