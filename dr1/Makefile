
all: subdirs 

subdirs:
	cd gfx/24x35 ; make
	cd lib ; make
	cd sdlclient ; make
	cd server ; make
#	cd mapedit ; make

depend:
	makedepend -I. \
		-I/usr/lib/gcc-lib/i686-pc-linux-gnu/pgcc-2.91.66/include *.c

tags:
	ctags *.h *.c

clean:
	rm */*.o
