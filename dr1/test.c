#include <stdio.h>

struct {
    int i;
    char *s;
    char buf[80];
    int st;
    int g;
} _a, _b;

int a() {
    char *s;
    enum { INIT, LOOP, DONE, BEGN };
    
    switch( _a.st) {
	case INIT: goto INIT;
	case LOOP: goto LOOP;
	case DONE: goto DONE;
    }

    INIT:
    printf("init\n");
    _a.s = _a.buf;

    for (_a.i = 0; _a.i < 10; _a.i++) {
        printf("a-what? ");
	LOOP: _a.st=LOOP;
	if (_a.g++ % 4 != 0) return 0;
	gets( _a.s);
    }
    DONE: _a.st=DONE;
    printf("done");
    return -1;
}


int b() {
    char *s;
    enum { INIT, LOOP, DONE, BEGN };
    
    switch( _b.st) {
	case INIT: goto INIT;
	case LOOP: goto LOOP;
	case DONE: goto DONE;
    }

    INIT:
    printf("init\n");
    _b.s = _b.buf;

    for (_b.i = 0; _b.i < 10; _b.i++) {
        printf("b-what? ");
	LOOP: _b.st=LOOP;
	if (_b.g++ % 3 != 0) return 0;
	gets( _b.s);
    }
    DONE: _b.st=DONE;
    printf("done");
    return -1;
}

int main(int argc, char **argv) {
    while (!a() && !b());
}
