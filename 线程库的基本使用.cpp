#include <bits/stdc++.h>
using namespace std;
void printhello(string& s){
    cout<<"子线程运行: "<<s<<endl;
    return ;
}
int main(){
    string ms="nihaoya";
    thread thread1(printhello,ref(ms));//thread创建一个线程  (函数名,函数参数)
    bool t=0;
    if(t=thread1.joinable()){
        thread1.join();//主线程等子线程结束后才向下走
        }
    cout<<"主线程正在运行";
    return 0;
}

