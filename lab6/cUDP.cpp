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

	//Step1����ʼ�����绷��  

	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)

	{

		printf("WSAStartup failed\n");

		return -1;

	}



	//Step2������һ��UDP��socket 

	//����socket������socket(Э����ָ��socket���ͣ�ָ��Э��)����TCPЭ�������������ͬ����ΪIPЭ���壩

	SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sockClient == INVALID_SOCKET)

	{

		printf("create socket failed\n");

		return -1;

	}



	// ����һ�������ַ��Ϣ�Ľṹ�壬����������ĵ�ַ��Ϣ  

	sockaddr_in addr = { 0 };

	addr.sin_family = AF_INET;		// Э���ΪIPV4��  

	addr.sin_port = htons(PORT);	// �˿�  ��Ϊ������С��ģʽ�������Ǵ��ģʽ������htons�ѱ����ֽ���תΪ�����ֽ���  

	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");	// ��������ip��ַ

		//Step3�������˽���ͨ��

	char buf[] = "begin";

	//��������  

	//sendto������socket���ƣ��������ݵĻ���������������С����־λ�����ò�����ʽ����sockaddr�ṹ��ַ��sockaddr�ṹ��С��ַ

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



	// ��������  

	//recvfrom������socket���ƣ��������ݵĻ���������������С����־λ�����ò�����ʽ����sockaddr�ṹ��ַ��sockaddr�ṹ��С��ַ

	//int dwRecv = recvfrom(sockClient, recvBuf, 512, 0, (SOCKADDR*)&addrSever, &nServerAddrLen);

	//printf("Recv msg from server : %s\n", recvBuf);
	cout << "�����ļ�·��";
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

	//Step4���ر�SOCKET����  

	closesocket(sockClient);



	//�������绷��  

	WSACleanup();

	system("pause");

	return 0;

}
