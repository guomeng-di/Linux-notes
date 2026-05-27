#include <bits/stdc++.h>
#include <thread>
#include <signal.h>  
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>

using namespace std;

bool server_running=1;

void signal_handler(int sig){
    cout<<"收到退出信号，正在关闭服务器...\n";
    server_running=0;
}
void send_response(int fd,const string&msg){
    send(fd,msg.c_str(),msg.size(),0);
}
string recv_command(int fd){
    char buf[1024]={0};
    int n=recv(fd,buf,1024,0);
    if(n<=0) return "";
    string s(buf,n);
    
    while(!s.empty()&&(s.back()=='\r'||s.back()=='\n'||s.back()=='\t'))
        s.pop_back();
    return s;
}
void handle_client(int ctrl_fd){
    int pasv_fd=-1;
    int data_fd=-1;

    send_response(ctrl_fd,"220 Simple FTP Server Ready\r\n");

    while(1){
        string cmd=recv_command(ctrl_fd);
        if(cmd.empty()) continue;

        //USER
        if(cmd.substr(0,4)=="USER")
          send_response(ctrl_fd,"331 Please specify password\r\n");
        
        //PASS
        else if(cmd.substr(0,4)=="PASS")
          send_response(ctrl_fd,"230 Login successful\r\n");

        //PASV
        else if(cmd.substr(0,4)=="PASV"){
            //1 socket->pasv_fd是被动模式下数据通道
            //而函数传进来的fd是命令通道
            pasv_fd=socket(AF_INET,SOCK_STREAM,0);
            //2 bind
            sockaddr_in addr;
            addr.sin_family=AF_INET;
            addr.sin_port=htons(0);//系统自动分配端口
            addr.sin_addr.s_addr=INADDR_ANY;

            bind(pasv_fd,(sockaddr*)&addr,sizeof(addr));
            //3 listen
            listen(pasv_fd,1);

            socklen_t len=sizeof(addr);
            getsockname(pasv_fd,(sockaddr*)&addr,&len);

            int port=ntohs(addr.sin_port);
            int p1=port/256;
            int p2=port%256;

            string response="227 Entering Passive Mode "
                "(127,0,0,1,"+ to_string(p1) + "," + to_string(p2) + ")\r\n";
            send_response(ctrl_fd,response);   
        }
        // LIST
        else if(cmd.substr(0,4)=="LIST"){
            send_response(ctrl_fd,"150 Here comes directory listing\r\n");
            data_fd=accept(pasv_fd,NULL,NULL);

            DIR* dir=opendir(".");
            dirent* entry;
            string result;
            while((entry=readdir(dir))!=NULL){
                result+=entry->d_name;
                result+="\r\n";
            }
            closedir(dir);

            send(data_fd,result.c_str(),result.size(),0);
            close(data_fd);
            close(pasv_fd);
            send_response(ctrl_fd,"226 Directory send OK\r\n");
        }
        // RETR
        else if(cmd.substr(0,4)=="RETR"){
            string filename=cmd.substr(5);
            while(!filename.empty()&&(filename.back()=='\r'||filename.back()=='\n'))
                 filename.pop_back();

            int fd=open(filename.c_str(),O_RDONLY);
            if(fd<0){
                send_response(ctrl_fd,"550 File not found\r\n");
                continue;
            }
            send_response(ctrl_fd, "150 Opening data connection\r\n");
            data_fd=accept(pasv_fd,NULL,NULL);

            char buf[1024*4]={0};
            int n;
            while((n=read(fd,buf,1024*4))>0)
               send(data_fd,buf,n,0);
            
            close(fd);
            close(data_fd);
            close(pasv_fd);
            send_response(ctrl_fd,"226 Transfer complete\r\n");
        }
        //STOR
        else if(cmd.substr(0,4)=="STOR"){
            string filename=cmd.substr(5);
            while(!filename.empty()&&(filename.back()=='\r'||filename.back()=='\n'))
                filename.pop_back();
            
                int fd=open(filename.c_str(),O_CREAT|O_WRONLY|O_TRUNC,0666);
                send_response(ctrl_fd,"150 Ok to send data\r\n");
                data_fd=accept(pasv_fd,NULL,NULL);

                char buf[1024*4]={0};
                int n;
            while((n=recv(data_fd,buf,sizeof(buf),0))>0) write(fd,buf,n);

            close(fd);
            close(data_fd);
            close(pasv_fd);
            send_response(ctrl_fd,"226 Transfer complete\r\n");
        }
        //QUIT
        else if(cmd.substr(0,4)=="QUIT"){
            send_response(ctrl_fd,"221 Goodbye\r\n");
            break;
        }else{
            send_response(ctrl_fd,"500 Unknown command\r\n");
        }
    }
    close(ctrl_fd);
    cout<<"client disconnected\n";
}

int main(){
    signal(SIGINT,signal_handler);
    //1 socket
    int listen_fd=socket(AF_INET,SOCK_STREAM,0);
    //2 bind
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(2100);//监听端口2100
    addr.sin_addr.s_addr=INADDR_ANY;//监听本机所有网卡

    bind(listen_fd,(sockaddr*)&addr,sizeof(addr));
    //3 listen
    listen(listen_fd,10);

    cout<<"FTP Server Start At Port 2100\n";
    
    //4
    while(server_running){
        struct timeval timeout;
        timeout.tv_sec=1;//设置秒数=1秒
        timeout.tv_usec=0;//设置微秒数=0秒
        setsockopt(listen_fd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));

        int client_fd=accept(listen_fd,NULL,NULL);
        if(client_fd<0) continue;
        if(!server_running) break;

        cout<<"new client connected\n";
        thread t(handle_client,client_fd);
        t.detach();
    }
    close(listen_fd);
    cout<<"服务器已安全关闭";
    return 0;
}