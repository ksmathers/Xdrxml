#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define PORT 2300

static struct {
    FILE *fp;
} env[FD_SETSIZE];

void initplayer( cs) {
    env[cs].fp = fdopen( cs, "r+");
}

int runplayer( cs) {
    char buf[80]; 
    buf[0] = 0;
    fgets(buf, sizeof(buf), env[cs].fp);
    if (feof(env[cs].fp)) return -1;
    printf("%d Got: %s\n", cs, buf);
    fprintf(env[cs].fp, "Got %s\n", buf);
    fflush(env[cs].fp);
    return 0;
}

int finitplayer( cs) {
    fclose(env[cs].fp);
    return 0;
}

int main( int argc, char **argv) {
    int ss, cs, maxsock;
    struct sockaddr_in sin;
    struct sockaddr csin;
    fd_set r_set, w_set, e_set;
    fd_set tr_set, tw_set, te_set;
    int err;
    struct timeval ttv, tv = { /* tv_sec */ 10, /* tv_usec */ 500000 };

    ss = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sin.sin_family = AF_INET;
    sin.sin_port = htons( PORT);
    sin.sin_addr.s_addr = INADDR_ANY;
    if (bind( ss, &sin, sizeof(sin))) { perror("bind"); abort(); }
    if (listen( ss, 10)) { perror("listen"); abort(); }

    /* setup for select */
    maxsock = ss;
    FD_ZERO( &r_set);
    FD_ZERO( &w_set);
    FD_ZERO( &e_set);
    FD_SET( ss, &r_set);
    FD_SET( ss, &e_set);

    printf("Server ready... %d\n", ss);
    for (;;) {
        int i;
	int readerr;
	int sa_len;
        tr_set = r_set; tw_set = w_set; te_set = e_set;
	ttv = tv;

        
        err = select( maxsock + 1, &tr_set, &tw_set, &te_set, &ttv);
	if (err == EINTR) continue;
	if (err < 0) { perror("select"); abort(); }
	printf("%d sockets ready\n", err);

	printf("socket event\n");

        for (i=0; i<=maxsock; i++) {
	    readerr=0;
	    printf("checking %d\n", i);

	    if (FD_ISSET( i, &tr_set)) {
	        printf("readable %d\n", i);
		/* readable */
	        if (i == ss) {
		    cs = accept( ss, &csin, &sa_len);
		    printf("Client connect on %d cs=%d\n", PORT, cs);
		    if (cs < 0) {
			perror("accept");
		    } else {
		        initplayer( cs);
			if (cs > maxsock) maxsock = cs;
			FD_SET( cs, &r_set);
			FD_SET( cs, &e_set);
		    } /* if */
		} else {
		    readerr = runplayer( i);
		}
	    } /* if */

	    if (FD_ISSET( i, &tw_set)) {
		/* writable */
	    } /* if */

	    if (FD_ISSET( i, &te_set) || readerr) {
		/* errors */
		printf("Error on socket %d\n", i);
		perror("select");
		finitplayer(i);
		close(i);
		FD_CLR( i, &r_set);
		FD_CLR( i, &w_set);
		FD_CLR( i, &e_set);
	    } /* if */
	} /* for */
    }
}
