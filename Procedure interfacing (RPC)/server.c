#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <inttypes.h>
#include <time.h>

typedef char byte;  // Since byte is valid data type in c we have to type-define it.
typedef enum { ADD = 0, SUB, MUL } OpType;
typedef struct { byte number_of_bytes;  byte id; OpType op; byte params[2]; } Request;
typedef struct { byte number_of_bytes; byte id; byte status; byte data; } Response;
//number_of_bytes is the number of bytes read make up a valid request/response to be processed.

int add(const Request* req, Response* res) {
  res->data = req->params[0] + req->params[1];
  return true;
}

int sub(const Request* req, Response* res) {
  res->data = req->params[0] - req->params[1];
  return true;
}

int mult(const Request* req, Response* res) {
  res->data = req->params[0] * req->params[1];
  return true;
}

int handleRequest(const Request* req, Response* res) {
  switch (req->op) {
    case ADD:
      return add(req, res);
    case SUB:
      return sub(req, res);
    case MUL:
      return mult(req, res);
    default:
      return false;
  }
}

int main() {
  printf("Waiting for request...\n");
  struct sockaddr_in servaddr;
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));   // Clear servaddr
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htons(INADDR_ANY);
  servaddr.sin_port = htons(22000);
  bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
  listen(listen_fd, 1);
  int comm_fd = accept(listen_fd, (struct sockaddr*)NULL, NULL);
  Request req  = {0};
  Response res = {0};
  
  if (read(comm_fd, (byte*)&req, sizeof(Request)));

  if (req.number_of_bytes == 0xA) {
	printf("\n	Request recieved\n");
	printf("-------------------------------\n");
	printf("Numbers recieved: %d and %d\n", req.params[0], req.params[1]);
    printf("Operation: %d\n", req.op);

    res.number_of_bytes = req.number_of_bytes;
    res.id = req.id;

    if (handleRequest(&req, &res)) {
      res.status = true;
    }
    else {
      res.status = false;
    }

	printf("\nServer calculating...............\n\n");
	sleep(5);
    printf("Response sent: %d\n", res.data );

    if (write(comm_fd, (byte*)&res, sizeof(Response)));
  }
  else {
    printf("Unrecognized number_of_bytes (%d). Failing here\n", req.number_of_bytes);
  }
  
  close(comm_fd);

  return 0;
}
