#include "ech.h"


int main()
{
	int sockfd;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		EXIT("socket");

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8990);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	int on = 1;

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		EXIT("setsockopt");

	if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		EXIT("bind");

	if (listen(sockfd, SOMAXCONN) < 0)
		EXIT("listen");

	int conn;

	struct sockaddr_in peeraddr;
	socklen_t preelen = sizeof(peeraddr);

	if ((conn = accept(sockfd, (struct sockaddr*)&peeraddr, &preelen)) < 0)
		EXIT("accept");

	printf("IP address : %s on port %d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

	pid_t pid;

	pid = fork();

	if (pid == 1)
		EXIT("fork");

	if (pid == 0)
	{
		signal(SIGUSR1, handler);
		char sendbuffer[1024] = {0};

		while (fgets(sendbuffer, sizeof(sendbuffer), stdin) != NULL)
		{
			write(conn, sendbuffer, strlen(sendbuffer));
			memset(sendbuffer, 0, sizeof(sendbuffer));
		}
		printf("send process close\n");
		exit(EXIT_SUCCESS);
	}
	else
	{
		char recvbuffer[1024];

		while (1)
		{
			memset(recvbuffer, 0, sizeof(recvbuffer));
			int ret = read(conn, recvbuffer, sizeof(recvbuffer));
			if (ret == -1)
				EXIT("read");
			if (ret == 0)
			{
				printf("client close\n");
				break;
			}
			fputs(recvbuffer, stdout);
		}
		printf("recv process close\n");
		kill(pid, SIGUSR1);
		exit(EXIT_SUCCESS);
	}

	return 0;
}