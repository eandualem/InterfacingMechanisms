#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main(){
	int fd[2];
	int xy[2];
	/*create pipe*/
	pipe(fd);
	pipe(xy);

	if (fork() != 0) { /* This is parent process */
		char *parent_message = "Am parent.\n" ;
		char buf2[1024];
		close(fd[0]);
		close(xy[1]);
		write(fd[1], parent_message, strlen (parent_message) + 1);

		while(1){
			read(xy[0], buf2, 1024);
			if(buf2){
				printf("Parent process recieved a message.");
				printf("\nMessage is : %s\n\n", buf2);
				break;
			}
		}
	}

	else { /*Child process */
		char *child_message = "Am child.\n" ;
		char buf[1024];
		close(fd[1]);
		close(xy[0]);
		while(1){
			read(fd[0], buf, 1024);
			if(buf){
				printf("Child process recieved a message.");
				printf("\nMessage is: %s\n", buf);
				break;
			}
		}
		write(xy[1], child_message, strlen (child_message) + 1);

	}
}

