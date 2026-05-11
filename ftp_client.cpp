#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int main(){
    //1 socket
    int clientFd=socket(AF_INET,SOCK_STREAM,0);
    //2 connect
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(21);
    inet_pton(AF_INET,"127.0.0.1",&addr.sin_addr);
    connect(clientFd,(sockaddr*)&addr,sizeof(addr));

    char buf[1024]={0};
    //登录
    const char* login="LOGIN admin 123456";
    send(clientFd,login,strlen(login),0);
    recv(clientFd,buf,1024,0);
    cout<<"服务器回复:"<<buf<<endl;
    memset(buf,0,1024);
    //列文件
    const char* ls="LS";
    send(clientFd,ls,strlen(ls),0);
    recv(clientFd,buf,1024,0);
    cout<<"文件列表:"<<buf<<endl;
    memset(buf,0,1024);
    //下载文件
    const char* down="DOWN";
    send(clientFd,down,strlen(down),0);
    recv(clientFd,buf,1024,0);
    cout<<"文件内容:"<<buf<<endl;
    close(clientFd);
    return 0;
}
