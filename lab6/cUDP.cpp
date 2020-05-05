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

	SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sockClient == INVALID_SOCKET)

	{

		printf("create socket failed\n");

		return -1;

	}



	// 申明一个网络地址信息的结构体，保存服务器的地址信息  

	sockaddr_in addr = { 0 };

	addr.sin_family = AF_INET;		// 协议簇为IPV4的  

	addr.sin_port = htons(PORT);	// 端口  因为本机是小端模式，网络是大端模式，调用htons把本机字节序转为网络字节序  

	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");	// 服务器的ip地址

		//Step3：与服务端进行通信

	char buf[] = "begin";

	//发送数据  

	//sendto参数：socket名称，接收数据的缓冲区，缓冲区大小，标志位（调用操作方式），sockaddr结构地址，sockaddr结构大小地址

	int dwSent = sendto(sockClient, buf, strlen(buf), 0, (SOCKADDR*)&addr, sizeof(SOCKADDR));

	/*if (dwSent == 0)

	{

		printf("send %s failed\n", buf);

		return -1;

	}*/

	printf("send msg:%s\n", buf);



	char recvBuf[512];
	


	memset(recvBuf, 0, 512);



	sockaddr_in addrSever = { 0 };

	int nServerAddrLen = sizeof(sockaddr_in);



	// 接收数据  

	//recvfrom参数：socket名称，接收数据的缓冲区，缓冲区大小，标志位（调用操作方式），sockaddr结构地址，sockaddr结构大小地址

	//int dwRecv = recvfrom(sockClient, recvBuf, 512, 0, (SOCKADDR*)&addrSever, &nServerAddrLen);

	//printf("Recv msg from server : %s\n", recvBuf);
	cout << "输入文件路径";
	char filepath[50];
	scanf("%s", filepath);
	FILE* fp = fopen(filepath, "rb");
	if (fp == NULL) 
	{
		printf("Cannot open file, press any key to exit!\n");
		exit(0);
	}
	int nCount;
	while ((nCount = fread(recvBuf, 1,512, fp)) > 0) 
	{
		sendto(sockClient, recvBuf, nCount, 0, (SOCKADDR*)&addr, sizeof(SOCKADDR));
		
	}
	fclose(fp);

	//Step4：关闭SOCKET连接  

	closesocket(sockClient);



	//清理网络环境  

	WSACleanup();

	system("pause");

	return 0;

}
