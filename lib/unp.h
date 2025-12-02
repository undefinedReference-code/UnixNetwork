#pragma once

/* Standard headers commonly needed by UNP examples */
#include <stdio.h>
#include <stdlib.h>      /* getenv, atoi, malloc, free, EXIT_* */
#include <string.h>
#include <stddef.h>      /* NULL (usually covered by stdlib.h/stddef.h) */
#include <errno.h>       /* errno and error macros */
#include <unistd.h>      /* close, read, write, getopt, etc. */
#include <sys/types.h>   /* socklen_t, pid_t on some systems */
#include <sys/socket.h>  /* socket, bind, listen, accept, socklen_t */
#include <netinet/in.h>  /* sockaddr_in, htons, htonl, INADDR_ANY */
#include <arpa/inet.h>   /* inet_pton, inet_ntop */
#include <sys/time.h>    /* struct timeval */
#include <sys/select.h>  /* fd_set, FD_* macros, select() */

/* Miscellaneous constants */
#define	MAXLINE		4096	/* max text line length */
#define	BUFFSIZE	8192	/* buffer size for reads and writes */

/* Following could be derived from SOMAXCONN in <sys/socket.h>, but many
   kernels still #define it as 5, while actually supporting many more */
#define	LISTENQ		1024	/* 2nd argument to listen() */

/* Following shortens all the typecasts of pointer arguments: */
#define	SA	struct sockaddr
#ifdef __cplusplus
extern "C" {
#endif

/* wrapspck.c */
void Listen(int fd, int backlog);
void Bind(int fd, const struct sockaddr *sa, socklen_t salen);
void Connect(int fd, const struct sockaddr *sa, socklen_t salen);
int Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);
int Socket(int family, int type, int protocol);
ssize_t Recv(int sockfd, void *buf, size_t len, int flags);
ssize_t Send(int sockfd, const void *buf, size_t len, int flags);
/* end wrapspck.c */

/* wrapunix.c */
void Close(int fd);
void Write(int fd, void *ptr, size_t nbytes);
/* end wrapunix.c */

/* error.c */
void err_sys(const char *fmt, ...);
void err_quit(const char *fmt, ...);
void err_ret(const char *fmt, ...);
void err_dump(const char *fmt, ...);
void err_msg(const char *fmt, ...);
/* end error.c */

#ifdef __cplusplus
}
#endif