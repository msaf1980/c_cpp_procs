/**
 * Macroses for poll
 */

/**
 * Example:
 * for (i = 0 .. n - 1)
 *   POLLFD_SET(poll_set[i], sock_fd[i], POLLIN);
 */
#define POLLFD_SET(poll_fd, sockfd, event)                                     \
  do {                                                                         \
    (poll_fd).fd = sockfd;                                                     \
    (poll_fd).events = event;                                                  \
  } while (0)
