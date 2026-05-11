#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

int main(){
    //1 socket
    int serveFd=socket(AF_INET,SOCK_STREAM,0);
    //2 connect
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(21);
    addr.sin_addr.s_addr=INADDR_ANY;
    //3 bind
    bind(serveFd,(sockaddr*)&addr,sizeof(addr));
    //4 listen
    listen(serveFd,5);
    cout<<"FTP服务器启动,端口21...\n";
    //5 accept
    int connFd=accept(serveFd,NULL,NULL);
    cout<<"客户端已连接!\n";

    char buf[1024]={0};
    while(1){
        int n=recv(connFd,buf,1024,0);
        if(n<=0) break;

        string cmd=buf;
        cout<<"收到命令:"<<cmd<<endl;

        if(cmd.find("LOGIN")!=string::npos){
            send(connFd,"登录成功",8,0);
        }else if(cmd=="LS"){
            send(connFd,"1.txt  2.jpg  视频.mp4",20,0);
        }else if(cmd=="DOWN"){
            send(connFd,"文件内容:hello tcp+ftp",22,0);
        }
        memset(buf,0,1024);
    }
    close(connFd);
    close(serveFd);
    return 0;
}