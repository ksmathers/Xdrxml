typedef struct {
} dr1StreamManager;

dr1StreamManager_listen( int port);

dr1StreamManager_fdset( read_fds, write_fds, err_fds);

dr1StreamManager_onSelect( read_fds, write_fds, err_fds);

#if 0

dr1StreamManager *sm1 = dr1StreamManager_create( portnum);
dr1StreamManager *sm2 = dr1StreamManager_create( portnum2);

FDSET_CLR( readfds);
FDSET_CLR( writefds);
FDSET_CLR( errfds);
dr1StreamManager_fdset( sm1, readfds, writefds, errfds);
dr1StreamManager_fdset( sm2, readfds, writefds, errfds);
select( readfds, writefds, errfds);
dr1StreamManager_onSelect( sm1, read_fds, write_fds, err_fds);
dr1StreamManager_onSelect( sm2, read_fds, write_fds, err_fds);


