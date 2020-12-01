#include <stdio.h>  
#include <strings.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
//#include <linux/in.h>  
#include <stdlib.h>  
#include <memory.h>  
#include <arpa/inet.h>  
#include <netinet/in.h>  
  
#include <signal.h> //添加信号处理  防止向已断开的连接�?�信  
  
/** 
  信号处理顺序说明：在Linux操作系统中某些状况发生时，系统会向相关进程发送信号， 
  信号处理方式是：1，系统首先调用用户在进程中注册的函数�?2，然后调用系统的默认 
  响应方式,此处我们可以注册自己的信号处理函数，在连接断�?时执�? 
  */  
  
  
#define PORT    11910  
#define Buflen  1024  
  
void process_conn_client(int s);  
void sig_pipe(int signo);    //用户注册的信号函�?,接收的是信号�?  
  
int s;  //全局变量 �? 存储套接字描述符  
  
int main(int argc,char *argv[])  
{  
  
    sockaddr_in server_addr;  
    int err;  
    sighandler_t ret;  
    char server_ip[50] = "";  
    /********************socket()*********************/  
    s= socket(AF_INET,SOCK_STREAM,0);  
    if(s<0)  
    {  
        printf("client : create socket error\n");  
        return 1;  
    }  
  
    //信号处理函数  SIGINT 是当用户按一�? Ctrl-C 建时发�?�的信号  
    ret = signal(SIGTSTP,sig_pipe);  
    if(SIG_ERR == ret)  
    {  
        printf("信号挂接失败\n");  
        return -1;  
    }  
    else  
        printf("信号挂接成功\n") ;  
  
  
    /*******************connect()*********************/  
    //设置服务器地�?结构，准备连接到服务�?  
    memset(&server_addr,0,sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_port = htons(PORT);  
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
  
    /*将用户数入对额字符串类型的IP格式转化为整型数�?*/  
    //inet_pton(AF_INET,argv[1],&server_addr.sin_addr.s_addr);  
    printf("please input server ip address : \n");  
    read(0,server_ip,50);  
    //err = inet_pton(AF_INET,server_ip,&server_addr.sin_addr.s_addr);  
    server_addr.sin_addr.s_addr = inet_addr(server_ip);  
  
    err = connect(s,(struct sockaddr *)&server_addr,sizeof(sockaddr));  
    if(err == 0)  
    {  
        printf("client : connect to server\n");  
    }  
    else  
    {  
        printf("client : connect error\n");  
        return -1;  
    }  
    //与服务器端进行�?�信  
    process_conn_client(s);  
    close(s);  
  
}  
void process_conn_client(int s)  
{  
  
    ssize_t size = 0;  
    char buffer[Buflen];  
  
    for(;;)  
    {  
        memset(buffer,'\0',Buflen);  
        /*从标准输入中读取数据放到缓冲区buffer�?*/  
        size = read(0,buffer,Buflen);   // 0，被默认的分配到标准输入  1，标准输�?  2，error  
        if(size >  0)  
        {  
            //当向服务器发�? “quit�? 命令时，服务器首先断�?连接  
            write(s,buffer,strlen(buffer)+1);   //向服务器端写  
  
            //等待读取到数�?  
            for(size = 0 ; size == 0 ; size = read(s,buffer,Buflen) );  
  
            write(1,buffer,strlen(buffer)+1);   //向标准输出写  
        }  
    }  
}  
  
void sig_pipe(int signo)    //传入套接字描述符  
{  
    printf("Catch a signal\n");  
    if(signo == SIGTSTP)  
    {  
  
        printf("接收�? SIGTSTP 信号\n");  
        int ret = close(s);  
        if(ret == 0)  
            printf("成功 : 关闭套接字\n");  
        else if(ret ==-1 )  
            printf("失败 : 未关闭套接字\n");  
        exit(1);  
    }  
} 
