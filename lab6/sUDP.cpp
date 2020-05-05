#include<iostream>
#include <stdio.h>  

#include <winsock2.h>  

#include <Windows.h>  
using namespace std;
#pragma warning(disable:4996) 

#pragma comment(lib,"ws2_32.lib")  

#define  PORT 6000  



int main(int argc, char* argv[])

{

	//Step1：初始化网络环境  

	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)

	{

		printf("WSAStartup failed\n");

		return -1;

	}



	//Step2：建立一个UDP的socket  

	//建立socket参数：socket(协议域，指定socket类型，指定协议)（和TCP协议后两个参数不同，都为IP协议族）

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sock == SOCKET_ERROR)

	{

		printf("create socket failed\n");

		return -1;

	}



	//Step3：绑定地址信息  

	sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;		// 协议簇为IPV4的  

	serverAddr.sin_port = htons(PORT);		// 端口  因为本机是小端模式，网络是大端模式，调用htons把本机字节序转为网络字节序  

	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);	// ip地址，INADDR_ANY表示绑定电脑上所有网卡IP 



	//bind(socket描述字， 绑定给listenfd的协议地址，地址长度)

	bind(sock, (sockaddr*)&serverAddr, sizeof(sockaddr));

	//Step5：与客户端进行通信

	char buf[512];

	while (TRUE)

	{

		memset(buf, 0, 512);

		// 网络节点的信息，用来保存客户端的网络信息  

		sockaddr_in clientAddr;

		memset(&clientAddr, 0, sizeof(sockaddr_in));



		int clientAddrLen = sizeof(sockaddr);

		//接收客户端发来的数据 

		//recvfrom参数：socket名称，接收数据的缓冲区，缓冲区大小，标志位（调用操作方式），sockaddr结构地址，sockaddr结构大小地址

		//sockaddr地址用来保存从哪里发来，和发送到哪里的地址信息

		int ret = recvfrom(sock, buf, 512, 0, (sockaddr*)&clientAddr, &clientAddrLen);
		if (strcmp(buf, "begin") == 0)
		{
			//inet_ntoa函数转化为ip，ntohs函数转化为端口号

			printf("Recv msgfrom IP:[%s] Port:[%d]\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

			// 发一个数据包返回给客户  

			//sendto参数：socket名称，发送数据的缓冲区，缓冲区大小，标志位（调用操作方式），sockaddr结构地址，sockaddr结构大小地址

			//sendto(sock, "Hello World!", strlen("Hello World!"), 0, (sockaddr*)&clientAddr, clientAddrLen);

			//printf("Send msg back to IP:[%s] Port:[%d]\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

			cout << "输入文件路径";
			char filepath[50];
			scanf("%s", filepath);
			FILE* fp = fopen(filepath, "wb");  //以二进制方式打开（创建）文件
			int nCount = recvfrom(sock, buf, sizeof(buf), 0, (sockaddr*)&clientAddr, &clientAddrLen);
			while (nCount > 0)
			{
				fwrite(buf, nCount, 1, fp);
				nCount = recvfrom(sock, buf, sizeof(buf), 0, (sockaddr*)&clientAddr, &clientAddrLen);
			}
			fclose(fp);
			cout << "success";
			
		}
	}

	return 0;

}
