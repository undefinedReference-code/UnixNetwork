#include "unp.h"
int main(int argc, char** argv) 
{
    int sockfd, n;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;
    if (argc != 2) 
        err_quit("usage:a.out <IPaddress>");
    
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        err_quit("inet_pton error for %s", argv[1]);

    if (connect(sockfd, (SA*) &servaddr, sizeof(servaddr)) < 0)
        err_sys("connect error");

    while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;	/* null terminate */
        // we didn't save it, only output it.
		if (fputs(recvline, stdout) == EOF) //这里输出recvline之后就可以把其中的内容丢弃，不需要存储下来
			err_sys("fputs error");
	}
	if (n < 0)
		err_sys("read error");

	exit(0);   
}