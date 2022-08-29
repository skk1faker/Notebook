#include<iostream>
#include<assert.h>
#include<signal.h>
#include<unistd.h>
using std::cout;
using std::endl;


void signalhander(int sig)
{
    cout<<"处理一些信号"<<endl;
    return ;
}

int main()
{


    //现将信号和信号处理函数进行绑定
    signal(SIGTSTP,signalhander);

    //  对于一个进程特别注意，屏蔽信号只是将信号处理延后执行(延至解除屏蔽，解除屏蔽也是设置以下屏蔽信号集)；而忽略表示将信号丢弃处理。
    sigset_t mask, ori_mask, output_mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGTSTP);
    assert(sigprocmask(SIG_BLOCK,&mask,&ori_mask) == 0);     // 加上他你的ctrl + z就不能将程序暂停！ 
    //sigprocmask 函数的介绍见linux高性能服务器183页
    sigpending(&output_mask);                               // 查看被挂起的信号
    cout<<sigismember(&output_mask,SIGTSTP)<<endl;          // 此时sigstp还未挂其（主要是手速不快），所以当前被挂起的信号集里面没有SIGTSTP信号，输出为0
    sleep(2);   
    
    sigpending(&output_mask);                               // 查看当前被挂起的信号有那些
    cout<<sigismember(&output_mask,SIGTSTP)<<endl;          // 当我们发送了SIGTSTP信号之后，这个信号是一定会在此时挂起的，那么我们直接既可以查到output_mask中有刮起的信号，输出为1
    printf("sleep2 结束\n");
    assert(sigprocmask(SIG_SETMASK,&ori_mask, NULL) == 0);     // 此时ctrl + z可以将其暂停 , 同样是使用sigprocmask可以将原来的掩码恢复，
    // 一些其他的信号处理函数这里就不举例了

    // 此时将收到的信号处理了，会执行signalhander函数

    sleep(2);
    printf("sleep2 结束");

}

/*

sleep2 结束(base) skt1faker@SKT1Faker:~/my_procedure/c++/server/signal$ ./signal_test 
0
^Z
1
sleep2 结束
处理一些信号
sleep2 结束(base) skt1faker@SKT1Faker:~/my_procedure/c++/server/signal$ 
(base) skt1faker@SKT1Faker:~/my_procedure/c++/server/signal$ ./signal_test 
0
0
sleep2 结束
sleep2 结束

*/
