#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#pragma comment(lib, "WS2_32")
#include <iostream>
#include <stdio.h>
#include<time.h>
#define	BUF_SIZE 1024
using namespace std;
int udp_client(){
	WSADATA wsadata;
	int iRet = 0;

	// 初始化套接字动态库
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0){
		iRet = WSAGetLastError();
		printf("WSAStartup failed !\n");
		return -1;
	}
	if (LOBYTE(wsadata.wVersion) != 2 || HIBYTE(wsadata.wVersion) != 2)
	{
		printf("Wrong version!\n");
		WSACleanup();
		return 0;
	}
	SOCKADDR_IN servAddr;
	SOCKET sockClient = socket(AF_INET, 
		SOCK_DGRAM, 
		IPPROTO_IP
		);

	if (sockClient == INVALID_SOCKET) {
		cout << "Socket 创建失败，Exit!";
		return 0;
	}

	// 设置服务器地址
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = inet_addr("39.105.16.186");
	servAddr.sin_port = htons(6555);

	char strSend[BUF_SIZE];
	char strRecv[BUF_SIZE];
	sprintf(strSend, "Hello World! This is client");
	memset(strRecv,0,sizeof(strRecv));
	int nServAddLen = sizeof(servAddr);

	// 向服务器发送数据
	iRet = sendto(sockClient, strSend, BUF_SIZE, 0, (sockaddr *)&servAddr, nServAddLen);
	if (iRet == SOCKET_ERROR){
		printf("sendto() failed:%d\n", WSAGetLastError());
		closesocket(sockClient);
		WSACleanup();
		return -1;
	}
	printf("msg has been sent!\n");

	// 从服务器接收数据
	iRet = recvfrom(sockClient, strRecv, BUF_SIZE, 0, (sockaddr *)&servAddr, &nServAddLen);
	if (iRet == SOCKET_ERROR)
	{
		printf("recvfrom failed:%d\n", WSAGetLastError());
		closesocket(sockClient);
		WSACleanup();
		return -1;
	}
	printf("Recv From Server:%s\n", strRecv);
	
	if (!closesocket(sockClient))
	{
		WSAGetLastError();
		return 0;
	}
	if (!WSACleanup())
	{
		WSAGetLastError();
		return 0;
	}
	cout << "客户端关闭" << endl;
	return 0;
}
int main(){
	//tcp_client();
	udp_client();
}