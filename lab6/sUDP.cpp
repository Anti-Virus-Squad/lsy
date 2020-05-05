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

	SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sock == SOCKET_ERROR)

	{

		printf("create socket failed\n");

		return -1;

	}



	//Step3���󶨵�ַ��Ϣ  

	sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;		// Э���ΪIPV4��  

	serverAddr.sin_port = htons(PORT);		// �˿�  ��Ϊ������С��ģʽ�������Ǵ��ģʽ������htons�ѱ����ֽ���תΪ�����ֽ���  

	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);	// ip��ַ��INADDR_ANY��ʾ�󶨵�������������IP 



	//bind(socket�����֣� �󶨸�listenfd��Э���ַ����ַ����)

	bind(sock, (sockaddr*)&serverAddr, sizeof(sockaddr));

	//Step5����ͻ��˽���ͨ��

	char buf[512];

	while (TRUE)

	{

		memset(buf, 0, 512);

		// ����ڵ����Ϣ����������ͻ��˵�������Ϣ  

		sockaddr_in clientAddr;

		memset(&clientAddr, 0, sizeof(sockaddr_in));



		int clientAddrLen = sizeof(sockaddr);

		//���տͻ��˷��������� 

		//recvfrom������socket���ƣ��������ݵĻ���������������С����־λ�����ò�����ʽ����sockaddr�ṹ��ַ��sockaddr�ṹ��С��ַ

		//sockaddr��ַ������������﷢�����ͷ��͵�����ĵ�ַ��Ϣ

		int ret = recvfrom(sock, buf, 512, 0, (sockaddr*)&clientAddr, &clientAddrLen);
		if (strcmp(buf, "begin") == 0)
		{
			//inet_ntoa����ת��Ϊip��ntohs����ת��Ϊ�˿ں�

			printf("Recv msgfrom IP:[%s] Port:[%d]\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

			// ��һ�����ݰ����ظ��ͻ�  

			//sendto������socket���ƣ��������ݵĻ���������������С����־λ�����ò�����ʽ����sockaddr�ṹ��ַ��sockaddr�ṹ��С��ַ

			//sendto(sock, "Hello World!", strlen("Hello World!"), 0, (sockaddr*)&clientAddr, clientAddrLen);

			//printf("Send msg back to IP:[%s] Port:[%d]\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

			cout << "�����ļ�·��";
			char filepath[50];
			scanf("%s", filepath);
			FILE* fp = fopen(filepath, "wb");  //�Զ����Ʒ�ʽ�򿪣��������ļ�
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
