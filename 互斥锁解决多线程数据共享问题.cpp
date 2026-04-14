//多个线程间数据共享就会产生数据竞争,解决方法是加锁
#include <bits/stdc++.h>
using namespace std;
int a=0;
mutex mtx;
void func(){
    for(int i=0;i<10000;i++){
        mtx.lock();//数据操作前加锁,确保只有一个线程在操作
        a++;
        mtx.unlock();//解锁
    }
}
int main(){
    thread thread1(func);
    thread thread2(func);
    thread1.join();
    thread2.join();
    cout<<a;
    return 0;
}