//线程1先获取m1,再获取m2;线程2先获取m2,再获取m1

#include <bits/stdc++.h>
using namespace std;
mutex m1,m2;
void func1(){
    for(int i=0;i<50;i++)
    {m1.lock();
    m2.lock();
    m1.unlock();
    m2.unlock();}
}
void func2(){
    for(int i=0;i<50;i++)
    {m2.lock();
    m1.lock();
    m2.unlock();
    m1.unlock();}
}
int main(){
    thread t1(func1);
    thread t2(func2);
    t1.join();
    t2.join();
    cout<<"over";
    return 0;
}

//可能出现t1给1上锁等2,t2给2上锁等1的情况->死锁
// 想象一下这两个线程同时运行，可能会发生以下情况：
// t1 抢占先机：t1 执行 m1.lock() 成功，持有了 m1 锁。
// t2 抢占先机：就在 t1 还没来得及锁 m2 的瞬间，t2 执行 m2.lock() 成功，持有了 m2 锁。
// 陷入僵局：
// t1 继续执行，尝试 m2.lock()。但是 m2 已经被 t2 拿走了，所以 t1 阻塞（等待），等待 t2 释放 m2。
// t2 继续执行，尝试 m1.lock()。但是 m1 已经被 t1 拿走了，所以 t2 阻塞（等待），等待 t1 释放 m1。
// 结果：t1 在等 t2 释放资源，t2 在等 t1 释放资源。双方都在持有自己手中资源的同时，等待对方释放资源，
// 形成了死循环等待，程序就会卡在这里永远无法结束，也就是“死锁”。

//解决方法:
//要么 t1 先拿到两个锁，t2 在外面排队；要么 t2 先拿到两个锁，t1 在外面排队。

//如下:
// #include <bits/stdc++.h>
// using namespace std;
// mutex m1,m2;
// void func1(){
//     for(int i=0;i<50;i++)
//     {m1.lock();
//     m2.lock();
//     m1.unlock();
//     m2.unlock();}
// }
// void func2(){
//     for(int i=0;i<50;i++)
//     {m1.lock();
//     m2.lock();
//     m2.unlock();
//     m1.unlock();}
// }
// int main(){
//     thread t1(func1);
//     thread t2(func2);
//     t1.join();
//     t2.join();
//     cout<<"over";
//     return 0;
// }