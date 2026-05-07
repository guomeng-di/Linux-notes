#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;
const int HEADER_LEN=4;
void sendMsg(int fd,const string& msg){
    //1获取数据长度
    int len=msg.size();
    //2先发固定的4个字节长度(网络字节序)
    int netLen=htonl(len);
    send(fd,&netLen,HEADER_LEN,0);
    //3发数据
    send(fd,msg.c_str(),len,0);
    
    return ;
}
string recvMsg(int fd){
    //1读取长度头
    int netlen=0;
    int ret=recv(fd,&netlen,HEADER_LEN,0);
    if(ret<=0) return "";
    //2转回本机字节序
    int datalen=ntohl(ret);
    //3按需读取完整数据
    char buf[1024]={0};
    recv(fd,buf,datalen,0);
    
    return string(buf);
}
int main(){
    //1创建socket
    int listenFd=socket(AF_INET,SOCK_STREAM,0);
    //
    sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(8888);
    serAddr.sin_addr.s_addr = INADDR_ANY;
    //3 bind绑定
    bind(listenFd,(sockaddr*)&serAddr,sizeof(serAddr));
    //4 listen监听
    listen(listenFd, 5);  // 最多排队5个连接
    cout << "服务端启动，监听8888...\n";
    //5 accept等待客户端连接
    int connFd=accept(listenFd,NULL,NULL);

    while(1){
        string res=recvMsg(connFd);
        cout<<"收到客户端消息:"<<res<<endl;
        sendMsg(connFd, "已收到：" + res);
    }
    close(connFd);
    close(listenFd);
    return 0;
}