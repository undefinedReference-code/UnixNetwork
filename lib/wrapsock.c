#include "unp.h"

void Listen(int fd, int backlog)
{
	char	*ptr;

	/*4can override 2nd argument with environment variable */
	if ( (ptr = getenv("LISTENQ")) != NULL)
		backlog = atoi(ptr);
    
    /* The backlog parameter specifies the maximum length of the completed connection queue:
    In Linux: Typically refers to the number of connections that have completed the three-way handshake
    Common values: 5, 10, 128, 1024, etc.
    Tuning: High-concurrency servers require a larger backlog value */    
	if (listen(fd, backlog) < 0)
		err_sys("listen error");
}

void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (bind(fd, sa, salen) < 0)
		err_sys("bind error");
}

void Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (connect(fd, sa, salen) < 0)
		err_sys("connect error");
}

int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
	int	n;

	if ((n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
		err_sys("select error");
	return(n);		/* can return 0 on timeout */
}

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	int	n;

again:
	if ( (n = accept(fd, sa, salenptr)) < 0) {
#ifdef	EPROTO
		if (errno == EPROTO || errno == ECONNABORTED)
#else
		if (errno == ECONNABORTED)
#endif
			goto again;
		else
			err_sys("accept error");
	}
	return(n);
}

int Socket(int family, int type, int protocol)
{
	int	n;

	if ( (n = socket(family, type, protocol)) < 0)
		err_sys("socket error");
	return(n);
}