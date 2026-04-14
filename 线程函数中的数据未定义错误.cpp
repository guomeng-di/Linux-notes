//传递临时变量的问题
//1 ref 传递引用类型     注意形参"&"
#include <bits/stdc++.h>
using namespace std;
void foo(int& x){
    x+=1;
    return ;
}
int main(){
    int a=1;
    thread thread1(foo,ref(a));
    cout<<a<<endl;//a=1(子线程执行前的主线程)
    thread1.join();
    cout<<a<<endl;//a=2(子线程执行完毕后的主线程)
    return 0;
}

//传递指针或引用指向局部变量的问题
//解决方法:将变量设置为全局变量

//传递指针或引用指向已释放的问题


