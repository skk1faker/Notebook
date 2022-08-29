#include<signal.h>
#include<assert.h>
#include<cstring>
#include<unistd.h>
#include<iostream>
using std::cout;
using std::endl;
void sig_handle(int sig){
    if(sig == SIGTSTP){
        cout<<"no stop haha, and i will sleep"<<endl;
        sleep(5);      // 此时由于我们sa_mask将所有信号屏蔽，此时的ctrl + c并不能被接收
    }
    else {
        cout<< "SIGINT process "<<endl;
        sleep(1);
    }
}
void add_sig( int sig)
{
    struct sigaction sa;    // 前面加上struct，否则会被当作绑定信号的函数
    memset(&sa,0,sizeof(sa));
    sigfillset(&sa.sa_mask);		// sa_mask 的作用主要是在sa_hander函数处理信号的时候需要屏蔽哪些信号
    // https://www.jianshu.com/p/cfd8e9824f54	// 讲述sa_flag的作用
    sa.sa_handler = sig_handle;
    sa.sa_flags |= SA_RESTART;  // 被这个信号中断的系统调用会重新启动，sa_flags主要作用就是程序收到信号的行为
    assert(sigaction(sig, &sa, NULL) != -1);
}

int main(){
    
    add_sig(SIGTSTP);
    add_sig(SIGINT);
    sleep(1);


}
/*
(base) skt1faker@SKT1Faker:~/my_procedure/c++/server/signal$ ./sigaction_test 
^Zno stop haha, and i will sleep
^CSIGINT process 
(base) skt1faker@SKT1Faker:~/my_procedure/c++/server/signal$ ./sigaction_test 
*/
