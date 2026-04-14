//1 lock_guard

//lock_guard<mutex> lg(mtx)相当于 mtx.lock()+mtx.unlock()

//例如:
// #include <bits/stdc++.h>
// using namespace std;
// int a=0;
// mutex mtx;
// void func(){
//     for(int i=0;i<10000;i++){
//         mtx.lock();//数据操作前加锁,确保只有一个线程在操作
//         a++;
//         mtx.unlock();//解锁
//     }
// }
// int main(){
//     thread thread1(func);
//     thread thread2(func);
//     thread1.join();
//     thread2.join();
//     cout<<a;
//     return 0;
// }

//等价于:
#include <bits/stdc++.h>
using namespace std;
int a=0;
mutex mtx;
void func(){
    for(int i=0;i<10000;i++){

        
        lock_guard<mutex> lg(mtx);

        //mtx.lock();//数据操作前加锁,确保只有一个线程在操作
        a++;
        //mtx.unlock();//解锁
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
