#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;
const int HEADER_LEN=4;
void sendMsg(int fd,string& msg){
    //1获取数据长度
    int len=msg.size();
    //2先发固定的4个字节长度(网络字节序)
    int netLen=htonl(len);
    send(fd, &netLen, HEADER_LEN, 0);
    //3发数据
    send(fd,msg.c_str(),len,0);

    return;
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
    int clientFd=socket(AF_INET,SOCK_STREAM,0);
    //2连接服务端 127.0.0.1:8888
    sockaddr_in serAddr;                // ① 创建一个“地址信息”变量
    serAddr.sin_family = AF_INET;        // ② 用IPv4地址（固定写法）
    serAddr.sin_port = htons(8888);      // ③ 端口号：8888
    serAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // ④ IP：本机

    connect(clientFd, (sockaddr*)&serAddr, sizeof(serAddr)); // ⑤ 发起连接

    cout<<"连接服务端成功"<<endl;

    string input;
    while(1){
        cout<<"请输入信息"<<endl;
        getline(cin,input);

        sendMsg(clientFd,input);//发
        string res=recvMsg(clientFd);
        cout<<"服务端回复"<<res<<endl;
    }
    close(clientFd);
    return 0;
}