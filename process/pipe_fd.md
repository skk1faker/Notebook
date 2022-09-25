1. 管道是两个文件描述符fd[0]、fd[1]指向了同一个文件，两个文件描述符都可读可写，一般用于父子进程间通讯，父进程和子进程一个close(fd[0]，另一个close(fd[1])，这种关闭互不影响（可能只是引用计数减少1，在unix和linux平台需要考虑多进程的情况，fork时，子进程继承父进程所拥有的文件描述符，需要所有拥有者都close文件描述符才会把资源销毁，所以需要使用引用计数。）
https://blog.csdn.net/Dontla/article/details/124131873

2. fork()子进程与父进程之间的文件描述符问题 
https://wenku.baidu.com/view/b7cdf96c6aeae009581b6bd97f1922791688bebc.html

理解Linux的文件描述符FD与Inode - engru的文章 - 知乎
https://zhuanlan.zhihu.com/p/143430585
