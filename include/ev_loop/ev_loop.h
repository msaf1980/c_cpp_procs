#ifndef _EV_LOOP_H_
#define _EV_LOOP_H_

#define ERR_CREATE      -1
#define ERR_ADD_LISTEN  -2
#define ERR_WAIT_LISTEN -3
#define ERR_WAIT        -4
#define ERR_ADD         -5
#define ERR_ACCEPT      -6

#define EV_NONE          0
#define EV_READ          1
#define EV_SEND          2
#define EV_SHUTDOWN      3
#define EV_CLOSE         4
#define EV_ERROR         5

#define EV_BUFPAGE 4096

struct ev_fd_t {
    int fd;
    ev_buf *buf; 

struct ev_event_t {
	int fd;
	char event;
};

#endif /* _EV_LOOP_H_ */
