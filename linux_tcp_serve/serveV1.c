#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

 
/* socket
 * bind
 * listen
 * accept
 * send/recv
 */
 
#define SERVER_PORT 5000
#define BACKLOG     10
 
int main(int argc, char **argv)
{
	int iSocketServer;
	int iSocketClient;
	struct sockaddr_in tSocketServerAddr;
	struct sockaddr_in tSocketClientAddr;
	int iRet;
	int iAddrLen;
 
	int iRecvLen;
	unsigned char ucRecvBuf[1000];
 
	int iClientNum = -1;
 
	signal(SIGCHLD,SIG_IGN);//处理僵尸进程的，可以看我那篇关于僵尸进程的博客
	
	/*
	*		int socket(int domain, int type,int protocol)
	*		domain:		说明我们网络程序所在的主机采用的通讯协族(AF_UNIX和AF_INET等). 
	*					AF_UNIX只能够用于单一的Unix 系统进程间通信,
	*					而AF_INET是针对Internet的,因而可以允许在远程 
	*
	*		type:		我们网络程序所采用的通讯协议(SOCK_STREAM,SOCK_DGRAM等) 
    *    				SOCK_STREAM表明我们用的是TCP 协议,这样会提供按顺序的,可靠,双向,面向连接的比特流. 
    *    				SOCK_DGRAM 表明我们用的是UDP协议,这样只会提供定长的,不可靠,无连接的通信.
	*
	*		protocol:	由于我们指定了type,所以这个地方我们一般只要用0来代替就可以了
	*/
	iSocketServer = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == iSocketServer)
	{
		printf("socket error!\n");
		return -1;
	}
 
	/*
	*		int bind(int sockfd, struct sockaddr *my_addr, int addrlen)
	*		把文件描述符和ip以及端口绑定起来
	*
	*		sockfd:是由socket调用返回的文件描述符.
	*		addrlen:是sockaddr结构的长度.
	*
	*		my_addr:是一个指向sockaddr的指针
	*
	*		struct sockaddr{
	*				unisgned short  as_family;
	*				char            sa_data[14];
	*		};
	*
	*		不过由于系统的兼容性,我们一般不用这个头文件,而使用另外一个结构(struct sockaddr_in) 来代替
	*			struct sockaddr_in{
	*					unsigned short          sin_family;     
	*					unsigned short int      sin_port;
	*					struct in_addr          sin_addr;
	*					unsigned char           sin_zero[8];
	*			}
	*		我们主要使用Internet所以
	*			sin_family一般为AF_INET,
	*			sin_addr设置为INADDR_ANY表示可以和任何的主机通信
	*			sin_port是我们要监听的端口号.
	*			sin_zero[8]是用来填充的，一般设置为01就行了
	*			bind将本地的端口同socket返回的文件描述符捆绑在一起.成功是返回0,失败的情况和socket一样
	*/
	tSocketServerAddr.sin_family      = AF_INET;
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* host to net, short */
 	tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
	memset(tSocketServerAddr.sin_zero, 0, 8);
	
	iRet = bind(iSocketServer, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));
	if (-1 == iRet)
	{
		printf("bind error!\n");
		return -1;
	}
 
	/*
	*		int listen(int sockfd,int backlog)
	*		sockfd:		是bind后的文件描述符.（不是指bind的返回值，而是指socket函数返回的文件句柄经过bind函数后）
	*		backlog:	设置请求排队的最大长度.当有多个客户端程序和服务端相连时, 使用这个表示可以介绍的排队长度. 这里我们设置为10
	*		listen函数将bind的文件描述符变为监听套接字.返回的情况和bind一样.
	*/
	iRet = listen(iSocketServer, BACKLOG);
	if (-1 == iRet)
	{
		printf("listen error!\n");
		return -1;
	}
 
	/*		这里的while(1)为父进程		*/
	while (1)
	{
		iAddrLen = sizeof(struct sockaddr);
		/*
		*		int accept(int sockfd, struct sockaddr *addr,int *addrlen)
		*		sockfd:是listen后的文件描述符.
		*		addr,addrlen是用来给客户端的程序填写的,服务器端只要传递指针就可以了。
		*		bind,listen和accept是服务器端用的函数,
		*		accept调用时,服务器端的程序会一直阻塞到有一个 客户程序发出了连接. 
		*		accept成功时返回最后的服务器端的文件描述符,
		*		这个时候服务器端可以向该描述符写信息了. 失败时返回-1
		*/
		iSocketClient = accept(iSocketServer, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
		
		/*		连接上了的话打印相应信息，并创建子进程对各个客户端发来的消息进行处理		*/
		if (-1 != iSocketClient)
		{
			iClientNum++;
			printf("Get connect from client %d : %s\n",  iClientNum, inet_ntoa(tSocketClientAddr.sin_addr));
			if (!fork())
			{
				/* 子进程的源码 */
				iRecvLen = recv(iSocketClient, ucRecvBuf, 999, 0);	//类似于read函数

				if (iRecvLen <= 0)
				{

					close(iSocketClient);
					return -1;
				}
				else
				{
					ucRecvBuf[iRecvLen] = '\0';
					printf("Get Msg From Client %d: %s\n", iClientNum, ucRecvBuf);
					while (1)
					{
					/* 接收客户端发来的数据并显示出来 */
					/*	ssize_t recv(int sockfd, void *buf, size_t len, int flags);
					*	flags设置为0就行了
					*/
        			printf("ser :# ");

					fflush(stdout);
					char buf[1024];
        				int count = read(0, buf, sizeof(buf)-1);
					buf[count] = '\0';
					if(strncasecmp(buf,"quit",4)==0)
					{
					break;
					}
       					buf[count] = '\0';
        				write(iSocketClient, buf, strlen(buf));

					}
				}				
			}
		}
		close(iSocketClient);
	}
	close(iSocketServer);
	
	return 0;
}


