#include	"unp.h"
#include    "string"
#include    "iostream"
using namespace std;

int main(int argc, char **argv) {
    if (argc != 2) {
        err_sys("using ./Client 127.0.0.1");
    }

    int					connfd;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE + 1];
	time_t				ticks;

	connfd = Socket(AF_INET, SOCK_STREAM, 0);
	memset(&servaddr, 0, sizeof(servaddr));

    char* ip_str = argv[1];

	servaddr.sin_family      = AF_INET;
	if (inet_pton(AF_INET, ip_str, &servaddr.sin_addr) != 1) {
        perror("inet_pton failed");
        exit(EXIT_FAILURE);
    }
	servaddr.sin_port        = htons(7);	/* daytime server */

	Connect(connfd, (SA*)&servaddr, sizeof(servaddr));

    for (;;) {
        int sendLength = 0;
        int recvLength = 0;
        string userInput;
        getline(cin, userInput);

        userInput += "\n";

        if (userInput == "exit" || userInput == "quit" || userInput == "q") {
            cout << "exit client..." << endl;
            break;
        }

        sendLength = Send(connfd, userInput.c_str(), userInput.size(), 0);

        int totalReceive = 0;
        bool messageComplete = false;

        while(!messageComplete) {
            int bytesRemaining = MAXLINE - totalReceive;
            
            if (bytesRemaining == 0) {
                buff[MAXLINE] = '\0'; 
                printf("%s", buff);   
                
                totalReceive = 0; 
                bytesRemaining = MAXLINE;
            }

            int n = Recv(connfd, buff + totalReceive,  bytesRemaining, 0);
            
            for (int i = 0; i < n; i++) {
                if (buff[totalReceive + i] == '\n') {
                    buff[totalReceive + i] = '\0';
                    messageComplete = true;
                    printf("Echo: %s\n", buff);
                    break;
                }
            }

            totalReceive += n;
        }
    }
    
    Close(connfd);
}