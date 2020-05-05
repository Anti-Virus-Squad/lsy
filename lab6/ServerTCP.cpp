#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include<string>
#include<iostream>
using namespace std;
#pragma warning(disable:4996) 
#pragma comment (lib, "ws2_32.lib")  //���� ws2_32.dll
#define BUF_SIZE 1024


int main() {

	/*�������ӵ�SOCKET*/
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);
	if (servSock < 0)
	{/*����ʧ�� */
		fprintf(stderr, "socker Error:%s\n", strerror(errno));
		exit(1);
	}
	/*��ʼ����������ַ*/
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(1234);

	if (bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) < 0)
	{/*��ʧ��*/
		fprintf(stderr, "Bind Error:%s\n", strerror(errno));
		exit(1);
	}

	/*�����ͻ�������*/
	if (listen(servSock, 20) < 0)
	{
		fprintf(stderr, "Listen Error:%s\n", strerror(errno));
		closesocket(servSock);
		exit(1);
	}

	cout << "�ȴ��ͻ��˽���..." << endl;

	/* �ȴ����տͻ���������*/
	SOCKADDR clntAddr;
	int nSize = sizeof(SOCKADDR);
	SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);

	if (clntSock <= 0)
	{
		fprintf(stderr, "Accept Error:%s\n", strerror(errno));
	}
	else
	{
		/*
		//�ȼ���ļ��Ƿ����
		const char* filename;  //�ļ���
		string filenames;
		cout << "������Ҫ�ϴ������������ļ�·����";
		cin >> filenames;
		filename = filenames.c_str();
		FILE* fp = fopen(filename, "rb");  //�Զ����Ʒ�ʽ���ļ�
		if (fp == NULL) {
			printf("Cannot open file, press any key to exit!\n");
			system("pause");
			exit(0);
		}
		//ѭ���������ݣ�ֱ���ļ���β
		char buffer[BUF_SIZE] = { 0 };  //������
		int nCount;
		while ((nCount = fread(buffer, 1, BUF_SIZE, fp)) > 0) {
			send(clntSock, buffer, nCount, 0);
		}
		cout << "�ļ���ȡ��ϣ��Ͽ����������ͻ��˷���FIN��" << endl;
		shutdown(clntSock, SD_SEND);  //�ļ���ȡ��ϣ��Ͽ����������ͻ��˷���FIN��
		recv(clntSock, buffer, BUF_SIZE, 0);  //�������ȴ��ͻ��˽������
		cout << "�ͻ��˽������" << endl;
		fclose(fp);
		*/

		//�������ļ��������ļ��Ƿ��ܴ����ɹ�
		char filename[100] = { 0 };  //�ļ���
		printf("����Ҫ�����ļ���·��������: ");
		gets_s(filename);
		FILE* fp = fopen(filename, "wb");  //�Զ����Ʒ�ʽ�򿪣��������ļ�
		if (fp == NULL) {
			printf("Cannot open file, press any key to exit!\n");
			system("pause");
			exit(0);
		}
		//ѭ���������ݣ�ֱ���ļ��������
		char buffer[BUF_SIZE] = { 0 };  //�ļ�������
		int nCount;
		while ((nCount = recv(clntSock, buffer, BUF_SIZE, 0)) > 0) {
			fwrite(buffer, nCount, 1, fp);
			strcpy(buffer, "ServerRecivedDATA.");
			send(clntSock, buffer, sizeof(buffer), 0);
		}
		puts("File transfer success!");

		//�ļ�������Ϻ�ֱ�ӹر��׽��֣��������shutdown()
		fclose(fp);

		// ��sockaddrǿ��ת��Ϊ sockaddr_in
		sockaddr_in cin;
		memcpy(&cin, &clntAddr, sizeof(cin));
		if (nCount < 0)
		{
			printf("IP��%s���� %s�ļ�������ʧȥ����\n", inet_ntoa(cin.sin_addr), filename);
			remove(filename);
		}

		closesocket(clntSock);

	}
	closesocket(servSock);
	WSACleanup();

	system("pause");
	return 0;
}
