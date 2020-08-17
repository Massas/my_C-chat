#ifndef _ECH_
#define _ECH_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<errno.h>
#include<signal.h>
#include<unistd.h> /* fork() read() write() */

#define EXIT(m) do {perror(m);exit(EXIT_FAILURE);} while(0);


void handler()
{
	printf("recv a exit signal.");
	exit(EXIT_SUCCESS);
}

#endif