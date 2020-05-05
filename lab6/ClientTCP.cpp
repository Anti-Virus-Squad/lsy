#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include<string>
#include<iostream>
using namespace std;
#pragma comment(lib, "ws2_32.lib")//WSAStartup( )链接库函数
#pragma warning(disable:4996) 

#define BUF_SIZE 1024

int main() {

	/*创建连接的SOCKET */
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);//使用2.2版本的Socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0)
	{/*创建失败 */
		fprintf(stderr, "socker Error:%s\n", strerror(errno));
		exit(1);
	}

	/* 初始化客户端地址*/
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(1234);
	if (connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) < 0)	//请求连接
	{/*连接失败 */
		fprintf(stderr, "Connect Error:%s\n", strerror(errno));
		closesocket(sock);
		exit(1);
	}

	/*
	//先输入文件名，看文件是否能创建成功
	char filename[100] = { 0 };  //文件名
	printf("输入要保存文件的路径与名称: ");
	gets_s(filename);
	FILE* fp = fopen(filename, "wb");  //以二进制方式打开（创建）文件
	if (fp == NULL) {
		printf("Cannot open file, press any key to exit!\n");
		system("pause");
		exit(0);
	}
	//循环接收数据，直到文件传输完毕
	char buffer[BUF_SIZE] = { 0 };  //文件缓冲区
	int nCount;
	while ((nCount = recv(sock, buffer, BUF_SIZE, 0)) > 0) {
		fwrite(buffer, nCount, 1, fp);
	}
	puts("File transfer success!");

	//文件接收完毕后直接关闭套接字，无需调用shutdown()
	fclose(fp);
	*/

	//先检查文件是否存在
	const char* filename;  //文件名
	string filenames;
	cout << "请输入要上传到服务器的文件路径：";
	cin >> filenames;
	filename = filenames.c_str();
	FILE* fp = fopen(filename, "rb");  //以二进制方式打开文件
	if (fp == NULL) {
		printf("Cannot open file, press any key to exit!\n");
		system("pause");
		exit(0);
	}
	//循环发送数据，直到文件结尾
	char buffer[BUF_SIZE] = { 0 };  //缓冲区
	int nCount;
	while ((nCount = fread(buffer, 1, BUF_SIZE, fp)) > 0) {
		send(sock, buffer, nCount, 0);
		if (recv(sock, buffer, BUF_SIZE, 0) <= 0)
		{
			cout << "服务器"<< inet_ntoa(sockAddr.sin_addr) <<"失去连接" << endl;
			break;
		}
	}
	cout << "文件读取完毕，断开输出流，向客户端发送FIN包" << endl;
	shutdown(sock, SD_SEND);  //文件读取完毕，断开输出流，向客户端发送FIN包
	recv(sock, buffer, BUF_SIZE, 0);  //阻塞，等待客户端接收完毕
	cout << "客户端接收完毕" << endl;
	fclose(fp);

	closesocket(sock);
	WSACleanup();
	system("pause");
	return 0;
}