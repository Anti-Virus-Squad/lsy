#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include<string>
#include<iostream>
using namespace std;
#pragma comment(lib, "ws2_32.lib")//WSAStartup( )���ӿ⺯��
#pragma warning(disable:4996) 

#define BUF_SIZE 1024

int main() {

	/*�������ӵ�SOCKET */
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);//ʹ��2.2�汾��Socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock < 0)
	{/*����ʧ�� */
		fprintf(stderr, "socker Error:%s\n", strerror(errno));
		exit(1);
	}

	/* ��ʼ���ͻ��˵�ַ*/
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockAddr.sin_port = htons(1234);
	if (connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) < 0)	//��������
	{/*����ʧ�� */
		fprintf(stderr, "Connect Error:%s\n", strerror(errno));
		closesocket(sock);
		exit(1);
	}

	/*
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
	while ((nCount = recv(sock, buffer, BUF_SIZE, 0)) > 0) {
		fwrite(buffer, nCount, 1, fp);
	}
	puts("File transfer success!");

	//�ļ�������Ϻ�ֱ�ӹر��׽��֣��������shutdown()
	fclose(fp);
	*/

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
		send(sock, buffer, nCount, 0);
		if (recv(sock, buffer, BUF_SIZE, 0) <= 0)
		{
			cout << "������"<< inet_ntoa(sockAddr.sin_addr) <<"ʧȥ����" << endl;
			break;
		}
	}
	cout << "�ļ���ȡ��ϣ��Ͽ����������ͻ��˷���FIN��" << endl;
	shutdown(sock, SD_SEND);  //�ļ���ȡ��ϣ��Ͽ����������ͻ��˷���FIN��
	recv(sock, buffer, BUF_SIZE, 0);  //�������ȴ��ͻ��˽������
	cout << "�ͻ��˽������" << endl;
	fclose(fp);

	closesocket(sock);
	WSACleanup();
	system("pause");
	return 0;
}