all: newplayer oldplayer

newplayer: newplayer.c
	gcc newplayer.c -onewplayer -I.. ../xdrxml.o -lxml2

oldplayer: oldplayer.c
	gcc oldplayer.c -ooldplayer -I.. ../xdrxml.o -lxml2

test:
	@echo "**New to old"; \
	newplayer write; \
	oldplayer read; \
	echo "**Old to new"; \
	oldplayer write; \
	newplayer read
