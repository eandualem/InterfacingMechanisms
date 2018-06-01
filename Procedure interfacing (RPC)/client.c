#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef char byte;  // Since byte is valid data type in c we have to type-define it.
typedef enum { ADD = 0, SUB, MUL} OpType;
typedef struct { byte number_of_bytes;  byte id; OpType op; byte params[2]; } Request;
typedef struct { byte number_of_bytes; byte id; byte status; byte data; } Response;
//number_of_bytes is the number of bytes read make up a valid request/response to be processed.

int main() {

    struct sockaddr_in servaddr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof servaddr);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port   = htons(22000);

    inet_pton(AF_INET, "127.0.0.1", &(servaddr.sin_addr));

    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    Request req = {.number_of_bytes = 0xA, .id = 0, .op = MUL, .params = {20, 4} };

    Response res = {0};
	printf("\n	Sending Request\n");
	printf("---------------------------\n");
    printf("Numbers sent: %d and %d\n", req.params[0], req.params[1]);
    printf("Operation: %d\n\n", req.op);

	sleep(5);
    if (write(sockfd, (byte*)&req, sizeof(Request)));

    if (read(sockfd, (byte*)&res, sizeof(Response)));
    

    printf("\n	Response Recieved\n");
	printf("-------------------------\n");
    printf("Response number: %d\n\n", res.data );

    close(sockfd);
    
    return 0;
}
