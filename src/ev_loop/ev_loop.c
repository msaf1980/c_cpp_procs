#include <errno.h>
#include <stdlib.h>

#if defined(__linux)
#define __EPOLL__ 1
#include <sys/epoll.h>

#include "netutils/epollutils.h"

#elif defined(__FreeBSD__)
#include <sys/event.h>
#endif

#include "fileutils.h"
#include "netutils/netutils.h"
#include "ev_loop/ev_loop.h"

#if defined(__EPOLL__)
int loop_queue_epool(int srv_fd, size_t queue_size, int poll_timeout, short *running, int *err) {
    int ec = 0;
    int epoll_fd;
    int cli_fd;
    SA_IN cli_addr;
    socklen_t cli_addr_len;

    int n_events, i; /* for epoll */
    struct epoll_event event, *events;

    events = malloc(sizeof(struct epoll_event) * queue_size);
    if (events == NULL) {
        *err = errno;
        ec = ERR_CREATE;
        return ec;
    }

    *err = 0;
    if ((epoll_fd = epoll_create1(EPOLL_CLOEXEC)) == -1) {
        *err = errno;
        ec = ERR_CREATE;
        goto EXIT;
    }

    /* EPOLL_EVENT_SET(event, srv_fd, EPOLLIN | EPOLLET); */
    EPOLL_EVENT_SET(event, srv_fd, EPOLLIN);
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, srv_fd, &event) == -1) {
        *err = errno;
        ec = ERR_ADD_LISTEN;
        goto EXIT;
    }
    // event.events |= EPOLLRDHUP;

    while (running) {
        n_events = epoll_wait(epoll_fd, events, queue_size, poll_timeout);
        if (n_events == -1) {
            if (errno != EINTR) {
                *err = errno;
                ec = ERR_WAIT;
                break;
            }
        }

        for (i = 0; i < n_events; i++) {
            if (events[i].data.fd == srv_fd) {
                if (events[i].events & (EPOLLHUP | EPOLLERR)) {
                    *err = EIO;
                    ec = ERR_WAIT_LISTEN;
                    break;
                }

                cli_fd = accept(srv_fd, (SA *) &cli_addr, &cli_addr_len);
                if (cli_fd < 0) {

                    // LOG_ERROR_ERRNO(root_logger, errno, "accept");
                    // run queue with accept error ???
                } else {
                    set_nonblock(cli_fd);
                    // queue client_register(cli_fd, &cli_addr); ???
                    EPOLL_EVENT_SET(event, cli_fd,
                                    EPOLLET | EPOLLIN | EPOLLRDHUP);
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, cli_fd, &event) ==
                        -1) {
                        // queue err ERR_ADD, errno ?? close socket
                    }
                }
            } else {
                if (events[i].events & (EPOLLHUP | EPOLLRDHUP))
                    queue_io(events[i].data.fd, Q_CLOSE, 1);
                else if (events[i].events & EPOLLERR)
                    queue_io(events[i].data.fd, Q_ERR, 1);
                else if (events[i].events & EPOLLIN) {
                    if (queue_io(events[i].data.fd, Q_READ, 0))
                        queue_io(events[i].data.fd, Q_SHUTDOWN, 1);
                } else if (events[i].events & EPOLLOUT) {
                    if (queue_io(events[i].data.fd, Q_SEND, 0))
                        queue_io(events[i].data.fd, Q_SHUTDOWN, 1);
                }
            }
        }
    }

EXIT:
    if (epoll_fd >= 0)
        close(epoll_fd);
    if (srv_fd >= 0)
        close(srv_fd);
    free(events);
    return ec;
}
/* end epoll */

#elif defined(__KQUEUE__)
int loop_queue_kqueue(int srv_fd) {
    int ec = 0;
    int res = 0;
    int i;

    int cli_fd;

    socklen_t cli_addrlen;
    SA_IN cli_addr;

    struct kevent event, events[QUEUE];

    ssize_t r;

    if ((kq = kqueue()) == -1) {
        ec = errno;
        _LOG_ERROR_ERNNO(root_logger, ec, "kqueue");
        goto EXIT;
    }

    memset(&event, 0, sizeof(event));
    EV_SET(&event, srv_fd, EVFILT_READ, EV_ADD, 0, 0, 0);
    if (kevent(kq, &event, 1, NULL, 0, NULL) == -1) {
        ec = errno;
        _LOG_ERROR_ERRNO(root_logger, ec, "kevent set register listen socket");
        goto EXIT;
    }
    if (event.flags & EV_ERROR) {
        ec = 1;
        _LOG_ERROR_ERRNO(root_logger, event.data, "event error");
        goto EXIT;
    }

    while (running) {
        if ((r = kevent(kq, NULL, 0, events, QUEUE, NULL)) == -1) {
            ec = errno;
            _LOG_ERROR_ERRNO(root_logger, ec, "kevent read");
            goto EXIT;
        }

        for (i = 0; i < r; i++) { /* event process loop */
            if (events[i].ident == srv_fd) {
                cli_addrlen = sizeof(cli_addr);
                cli_fd = accept(srv_fd, (SA *) &cli_addr, &cli_addrlen);
                if (cli_fd < 0) {
                    _LOG_ERROR_ERRNO(root_logger, errno, "accept");
                } else {
                    set_nonblock(cli_fd);
                    client_register(cli_fd, &cli_addr);
                    memset(&event, 0, sizeof(event));
                    EV_SET(&event, cli_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                    if (kevent(kq, &event, 1, NULL, 0, NULL) < 0) {
                        _LOG_ERROR_ERRNO(root_logger, errno, "kevent set");
                    }
                }
            } else {
                if (events[i].flags & EV_EOF)
                    queue_io(events[i].ident, Q_CLOSE, 1);
                else if (events[i].filter == EVFILT_READ) {
                    if (queue_io(events[i].ident, READ, 0))
                        queue_io(events[i].ident, Q_SHUTDOWN, 1);
                } else if (events[i].filter == EVFILT_WRITE) {
                    sock_item *si;
                    if ((si = find_socket(&cli_socks, events[i].ident)) ==
                        NULL) {
                        _LOG_ERROR(root_logger, "socket %d not in table",
                                   events[i].ident);
                        close_socket(events[i].ident, 1);
                    } else if (si->process == 0) {
                        if (queue_io(events[i].ident, SEND, 0))
                            queue_io(events[i].ident, Q_SHUTDOWN, 1);
                    }
                }
            }
        } /* event process loop */
    }

EXIT:
    return ec;
}
#endif /* kqueue */

