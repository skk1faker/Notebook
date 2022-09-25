## 参考资料
https://blog.csdn.net/liuhhaiffeng/article/details/104040174 --> 无法输出getpid()主要原因为输出的函数gdb中call和print不知道返回值类型，需要使用`call (int)getpid()`或者`print (int)getpid()`此时的输出就可以了


https://www.cnblogs.com/ainingxiaoguai/p/16492898.html --> 解决fork产生的多进程调试（注意不是**多线程**）
## 1 简介
&emsp;本文主要是解决调试fork()函数产生多线程的问题。

### 1.1 gdb中的两个变量
(a) `follow-fork-mode` 可选择的值`[parent|child]`
(b) `set follow-fork-mode parent` 表示最后gdb跟踪父进程，`set follow-fork-mode child`表示gdb跟踪子进程
(c) `detach-on-fork`   可选择的值`[on|off]`
`set detach-on-fork on`表示最后gdb只跟踪`follow-fork-mode`指定的进程，如果改为`off`则所有进程都会中断，然后一个一个被gdb调试
### 1.2 gdb中调试的进程如何切换
 不同进程在调试的过程中可以被切换，`info inferiors`会显示`gdb`调试了几个进程，使用`inferiors id`（这个是这些被gdb调试的进程在gdb中的编号，不是pid）。
### 1.3 gdb如何调试指定进程
`gdb attach pid` 或者进入gdb中 `attach pid` pid值的是进程id

## 2. 举例
对如下代码进行调试
```cpp
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

int a()
{
	return 1;
}

int main()
{
	pid_t pid = fork();

	printf("%d\n",pid);
	printf("pid = %d\n",getpid());
	while(1){
		sleep(1);
	}
	return 0;
}

```

1. file 命令加载一个可执行文件
```shell
(base) skt1faker@SKT1Faker:~/my_procedure/c++/process_fork$ gdb
(gdb) file a.out
```
2. 加上断点，设置首先进入哪个进程（父进程还是子进程）

```shell
Reading symbols from a.out...
(gdb) l
1	#include<stdlib.h>
2	#include<stdio.h>
3	#include<unistd.h>
4	
5	int a()
6	{
7		return 1;
8	}
9	
10	int main()
(gdb) 
11	{
12		pid_t pid = fork();
13	
14		printf("%d\n",pid);
15		printf("pid = %d\n",getpid());
16		while(1){
17			sleep(1);
18		}
19		return 0;
20	}
(gdb) b 15
Breakpoint 1 at 0x11e2: file gdb_multi_pro.cpp, line 15.
(gdb) set follow-fork-mode child
(gdb) show follow-fork-mode 
Debugger response to a program call of fork or vfork is "child".
```
3. 设置是否调试所有进程

```shell
(gdb) set detach-on-fork off
(gdb) show detach-on-fork 
Whether gdb will detach the child of a fork is off.
(gdb) r
Starting program: /home/skt1faker/my_procedure/c++/process_fork/a.out 
[Attaching after process 18389 fork to child process 18393]
[New inferior 2 (process 18393)]
Reading symbols from /home/skt1faker/my_procedure/c++/process_fork/a.out...
Reading symbols from /usr/lib/debug/.build-id/18/78e6b475720c7c51969e69ab2d276fae6d1dee.debug...
Reading symbols from /usr/lib/debug/.build-id/45/87364908de169dec62ffa538170118c1c3a078.debug...
0
[Switching to process 18393]

Thread 2.1 "a.out" hit Breakpoint 1, main () at gdb_multi_pro.cpp:15
15		printf("pid = %d\n",getpid());
```
4. 查看gdb调试的进程，并切换进程

```shell
(gdb) info inferiors 
  Num  Description       Executable        
  1    process 18389     /home/skt1faker/my_procedure/c++/process_fork/a.out 
* 2    process 18393     /home/skt1faker/my_procedure/c++/process_fork/a.out 
(gdb) inferior 1
[Switching to inferior 1 [process 18389] (/home/skt1faker/my_procedure/c++/process_fork/a.out)]
[Switching to thread 1.1 (process 18389)]
#0  arch_fork (ctid=0x7ffff7fa8810) at ../sysdeps/unix/sysv/linux/arch-fork.h:49
49	../sysdeps/unix/sysv/linux/arch-fork.h: No such file or directory.
```
切换完进程后发现栈帧有点不对劲，但是实际上刚刚的断点已经在两个进程中都打上了，只要continue就可以了（查看断点就可以知道），或者我们直接frame 2去打个断点。

```shell
(gdb) bt
#0  arch_fork (ctid=0x7ffff7fa8810) at ../sysdeps/unix/sysv/linux/arch-fork.h:49
#1  __libc_fork () at ../sysdeps/nptl/fork.c:76
#2  0x00005555555551c9 in main () at gdb_multi_pro.cpp:12
(gdb) c
Continuing.
18393

Thread 1.1 "a.out" hit Breakpoint 1, main () at gdb_multi_pro.cpp:15
15		printf("pid = %d\n",getpid());
(gdb) info b
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   <MULTIPLE>         
	breakpoint already hit 2 times
1.1                         y   0x00005555555551e2 in main() at gdb_multi_pro.cpp:15 inf 1
1.2                         y   0x00005555555551e2 in main() at gdb_multi_pro.cpp:15 inf 2
```
脱离进程
```shell
(gdb) detach
Detaching from program: /home/skt1faker/my_procedure/c++/process_fork/a.out, process 18389
[Inferior 1 (process 18389) detached]
pid = 18389
(gdb) info b
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   <MULTIPLE>         
	breakpoint already hit 2 times
1.1                         y   0x00005555555551e2 in main() at gdb_multi_pro.cpp:15 inf 1
1.2                         y   0x00005555555551e2 in main() at gdb_multi_pro.cpp:15 inf 2
(gdb) info inferiors 
  Num  Description       Executable        
* 1    <null>            /home/skt1faker/my_procedure/c++/process_fork/a.out 
  2    process 18393     /home/skt1faker/my_procedure/c++/process_fork/a.out 
(gdb) quit
A debugging session is active.

	Inferior 2 [process 18393] will be killed.

Quit anyway? (y or n) y
```


实际上断点也可以通过事件来获取
catch专用于获取一些事件，catch fork就可以将程序在fork函数处停住。
```shell
base) skt1faker@SKT1Faker:~/my_procedure/c++/process_fork$ gdb 
(gdb) file a.out 
Reading symbols from a.out...
(gdb) set 
Display all 140 possibilities? (y or n)
(gdb) set detach-on-fork off
(gdb) catch fork
Catchpoint 1 (fork)
(gdb) c
The program is not being run.
(gdb) r
Starting program: /home/skt1faker/my_procedure/c++/process_fork/a.out 

Catchpoint 1 (forked process 19109), arch_fork (ctid=0x7ffff7fa8810)
    at ../sysdeps/unix/sysv/linux/arch-fork.h:49
49	../sysdeps/unix/sysv/linux/arch-fork.h: No such file or directory.
(gdb) bt
#0  arch_fork (ctid=0x7ffff7fa8810) at ../sysdeps/unix/sysv/linux/arch-fork.h:49
#1  __libc_fork () at ../sysdeps/nptl/fork.c:76
#2  0x00005555555551c9 in main () at gdb_multi_pro.cpp:12
(gdb) info b
bookmarks    breakpoints  
(gdb) info b
bookmarks    breakpoints  
(gdb) info inferiors 
  Num  Description       Executable        
* 1    process 19105     /home/skt1faker/my_procedure/c++/process_fork/a.out 
(gdb) n
[New inferior 2 (process 19109)]
53	in ../sysdeps/unix/sysv/linux/arch-fork.h
(gdb) 
__libc_fork () at ../sysdeps/nptl/fork.c:78
78	../sysdeps/nptl/fork.c: No such file or directory.
(gdb) 
134	in ../sysdeps/nptl/fork.c
(gdb) 
144	in ../sysdeps/nptl/fork.c
(gdb) 
147	in ../sysdeps/nptl/fork.c
(gdb) 
main () at gdb_multi_pro.cpp:14
14		printf("%d\n",pid);
(gdb) info inferiors 
  Num  Description       Executable        
* 1    process 19105     /home/skt1faker/my_procedure/c++/process_fork/a.out 
  2    process 19109     /home/skt1faker/my_procedure/c++/process_fork/a.out 

```
在打断点的时候会有打一个段点出现在两个进程的情况，就是打在a进程，b进程也有，不过我么可以使用pid来区分，`b linenum if pid==0`，就是在子进程打断点（pid是fork返回值），或者`b linenum if (int)getpid()==pid`（这里的pid指的是进程的id，操作系统中的）。

```shell
(base) skt1faker@SKT1Faker:~/my_procedure/c++/process_fork$ gdb 
(gdb) file a.out
Reading symbols from a.out...
(gdb) l
1	#include<stdlib.h>
2	#include<stdio.h>
3	#include<unistd.h>
4	
5	int a()
6	{
7		return 1;
8	}
9	
10	int main()
(gdb) 
11	{
12		pid_t pid = fork();
13	
14		printf("%d\n",pid);
15		printf("pid = %d\n",getpid());
16		while(1){
17			sleep(1);
18		}
19		return 0;
20	}
(gdb) b 14
Breakpoint 1 at 0x11cc: file gdb_multi_pro.cpp, line 14.
(gdb) set detach-on-fork off
(gdb) r
Starting program: /home/skt1faker/my_procedure/c++/process_fork/a.out 
[New inferior 2 (process 19877)]
Reading symbols from /home/skt1faker/my_procedure/c++/process_fork/a.out...
Reading symbols from /usr/lib/debug/.build-id/18/78e6b475720c7c51969e69ab2d276fae6d1dee.debug...
Reading symbols from /usr/lib/debug/.build-id/45/87364908de169dec62ffa538170118c1c3a078.debug...

Thread 1.1 "a.out" hit Breakpoint 1, main () at gdb_multi_pro.cpp:14
14		printf("%d\n",pid);
(gdb) l
9	
10	int main()
11	{
12		pid_t pid = fork();
13	
14		printf("%d\n",pid);
15		printf("pid = %d\n",getpid());
16		while(1){
17			sleep(1);
18		}
(gdb) info b
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   <MULTIPLE>         
	breakpoint already hit 1 time
1.1                         y   0x00005555555551cc in main() at gdb_multi_pro.cpp:14 inf 1
1.2                         y   0x00005555555551cc in main() at gdb_multi_pro.cpp:14 inf 2
(gdb) l17
Undefined command: "l17".  Try "help".
(gdb) l 17
12		pid_t pid = fork();
13	
14		printf("%d\n",pid);
15		printf("pid = %d\n",getpid());
16		while(1){
17			sleep(1);
18		}
19		return 0;
20	}
(gdb) info inferior 
  Num  Description       Executable        
* 1    process 19873     /home/skt1faker/my_procedure/c++/process_fork/a.out 
  2    process 19877     /home/skt1faker/my_procedure/c++/process_fork/a.out 
(gdb) b 17 if (int)getpid()==19873
Breakpoint 2 at 0x5555555551fa: /home/skt1faker/my_procedure/c++/process_fork/gdb_multi_pro.cpp:17. (2 locations)
(gdb) b 18.(1)
Function "18.(1)" not defined.
Make breakpoint pending on future shared library load? (y or [n]) n
(gdb) c
Continuing.
19877
pid = 19873

Thread 1.1 "a.out" hit Breakpoint 2, main () at gdb_multi_pro.cpp:17
17			sleep(1);
(gdb) info b
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   <MULTIPLE>         
	breakpoint already hit 1 time
1.1                         y   0x00005555555551cc in main() at gdb_multi_pro.cpp:14 inf 1
1.2                         y   0x00005555555551cc in main() at gdb_multi_pro.cpp:14 inf 2
2       breakpoint     keep y   <MULTIPLE>         
	stop only if (int)getpid()==19873
	breakpoint already hit 1 time
2.1                         y   0x00005555555551fa in main() at gdb_multi_pro.cpp:17 inf 1
2.2                         y   0x00005555555551fa in main() at gdb_multi_pro.cpp:17 inf 2
(gdb) inferior 2
[Switching to inferior 2 [process 19877] (/home/skt1faker/my_procedure/c++/process_fork/a.out)]
[Switching to thread 2.1 (process 19877)]
#0  arch_fork (ctid=0x7ffff7fa8810) at ../sysdeps/unix/sysv/linux/arch-fork.h:49
49	../sysdeps/unix/sysv/linux/arch-fork.h: No such file or directory.
(gdb) c
Continuing.

Thread 2.1 "a.out" hit Breakpoint 1, main () at gdb_multi_pro.cpp:14        # 看到这里击中断点
14		printf("%d\n",pid);
(gdb) c
Continuing.
0
pid = 19877
^C      # 这里就没击中断点，只好发送信号中断。
Thread 2.1 "a.out" received signal SIGINT, Interrupt.
0x00007ffff7e921b4 in __GI___clock_nanosleep (clock_id=<optimized out>, clock_id@entry=0, 
    flags=flags@entry=0, req=req@entry=0x7fffffffdb00, rem=rem@entry=0x7fffffffdb00)
    at ../sysdeps/unix/sysv/linux/clock_nanosleep.c:78
78	../sysdeps/unix/sysv/linux/clock_nanosleep.c: No such file or directory.
```