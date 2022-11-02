//https://blog.51cto.com/760470897/1790060
// --> 主要原因//https://coolshell.cn/articles/12103.html
#include<iostream>
#include<stdio.h>
#include<unistd.h>
using namespace std;
pid_t pid;
void f()
{
	pid = vfork();
	printf("%d,pid = %d\n",1,pid);
	if(pid == 0)exit(0);	// 这里直接exit不会出现问题，如果是return会破坏栈，这样程序返回的时候会出问题，不论是在main函数中还是exit函数中。
	return ;
}
int f2()
{
	return 1;
}
int main()
{

	int c;
	f();
	//c = f2();
	printf("%d,pid = %d\n",c,pid);
	return 0;
}
