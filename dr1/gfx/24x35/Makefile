all: .buildall

clean:
	rm *bmp
	rm .buildall

.buildall:
	for i in *pcx ; do \
	    nn=`basename $$i .pcx`.bmp ; \
	    pcxtoppm $$i | ppmtobmp -windows -bpp=8 >$$nn ; \
	done
	touch .buildall
