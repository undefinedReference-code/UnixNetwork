#include	"unp.h"
#include	<time.h>

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE];

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr, 0, sizeof(servaddr));

	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(7);	/* daytime server */

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for ( ; ; ) {
		connfd = Accept(listenfd, (SA *) NULL, NULL);

        while(int n = Recv(connfd, buff, sizeof(buff), 0)) {
            Send(connfd, buff, n, 0);
        }

		Close(connfd);
	}
}