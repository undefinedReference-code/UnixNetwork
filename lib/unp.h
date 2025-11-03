#include <sys/socket.h>
#include <netinet/in.h>    
#include <string.h>
#include <stdio.h>

/* Miscellaneous constants */
#define	MAXLINE		4096	/* max text line length */
#define	BUFFSIZE	8192	/* buffer size for reads and writes */

/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
   kernels still #define it as 5, while actually supporting many more */
#define	LISTENQ		1024	/* 2nd argument to listen() */

/* Following shortens all the typecasts of pointer arguments: */
#define	SA	struct sockaddr

/* wrapspck.c */
void Listen(int fd, int backlog);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Connect(int fd, const struct sockaddr *sa, socklen_t salen);
int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
int Socket(int family, int type, int protocol);
/* end wrapspck.c */
/* wrapunix.c */
void Close(int fd);
void Write(int fd, void *ptr, size_t nbytes);
/* end wrapunix.c */
