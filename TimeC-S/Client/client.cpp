#include	"unp.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        err_sys("using ./Client 127.0.0.1");
    }

    int					connfd;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE];
	time_t				ticks;

	connfd = Socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr, 0, sizeof(servaddr));

    char* ip_str = argv[1];

	servaddr.sin_family      = AF_INET;
	if (inet_pton(AF_INET, ip_str, &servaddr.sin_addr) != 1) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }
	servaddr.sin_port        = htons(13);	/* daytime server */

	Connect(connfd, (SA*)&servaddr, sizeof(servaddr));

    
    while(int n = Recv(connfd, buff, sizeof(buff), NULL)) {
        if (n > 0) {
            buff[n] = '\0';
            printf("%s\n", buff);
        }
    }

    Close(connfd);
	
	
}