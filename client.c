#include "ech.h"


int main()
{
	int sock;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		EXIT("socket");

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8909);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		EXIT("connect");

	pid_t pid;

	pid = fork();

	if (pid == -1)
		EXIT("fork");

	if (pid == 0)
	{
		char recvbuffer[1024];

		while (1)
		{
			memset(recvbuffer, 0, sizeof(recvbuffer));
			int ret = read(sock, recvbuffer, sizeof(recvbuffer));

			if (ret == -1)
			{
				EXIT("read");
			}
			else if (ret == 0)
			{
				printf("server close\n");
				break;
			}
			fputs(recvbuffer, stdout);
		}
		close(sock);
		kill(getppid(), SIGUSR1);
	}
	else
	{
		signal(SIGUSR1, handler);
		char sendbuffer[1024] = {0};

		while (fgets(sendbuffer, sizeof(sendbuffer), stdin) != NULL)
		{
			write(sock, sendbuffer, strlen(sendbuffer));
			memset(sendbuffer, 0, sizeof(sendbuffer));
		}
		close(sock);
	}

	return 0;
}