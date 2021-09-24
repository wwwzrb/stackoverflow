#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // for close
#include <string.h>

/*
 * 用于创建大量TCP连接
 * 服务器端：创建一个监听socket
 */
int main(int argc, char *argv[]) {

	int server_sockfd;		// 服务器的套接字	
	struct sockaddr_in server_addr;		// 服务器的地址

	server_addr.sin_family=AF_INET; 	//设置为IP通信
	server_addr.sin_addr.s_addr=INADDR_ANY;		//服务器IP地址--允许连接到所有本地地址上
	server_addr.sin_port=htons(8000); 		//服务器端口号

	/*创建服务器端套接字--IPv4协议，面向连接通信，TCP协议*/
	if((server_sockfd=socket(PF_INET,SOCK_STREAM,0))<0) {  
					perror("socket error");
					return 1;
	}

	/*将套接字绑定到服务器的网络地址上*/
	if(bind(server_sockfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr))<0) {
					perror("bind error");
					return 1;
	}


	/*监听连接请求--监听队列长度为20*/
	if(listen(server_sockfd,20)<0) {
					perror("listen error");
					return 1;
	}
	printf("listen success.\n");

	char recv_buf[65536];
	memset(recv_buf, '\0', sizeof(recv_buf));

	while (1) {
		struct sockaddr_in client_addr;
		socklen_t length = sizeof(client_addr);
		//进程阻塞在accept上，成功返回非负描述字，出错返回-1
		int conn = accept(server_sockfd, (struct sockaddr*)&client_addr,&length);
		if(conn<0) {
			perror("connect");
			return -1;
		}

		printf("new client accepted.\n");

		char client_ip[INET_ADDRSTRLEN] = "";
		inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);

		while(recv(conn, recv_buf, sizeof(recv_buf), 0) > 0 ){
			printf("recv %s from client(%s:%d). \n", recv_buf, client_ip, ntohs(client_addr.sin_port));
			memset(recv_buf, '\0', strlen(recv_buf));
			break;
		}
	}

	printf("closed. \n");
	close(server_sockfd);
	return 0;
}