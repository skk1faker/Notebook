exec执行的文件可以改变有效用户id，但是也可以改变实际用户id

但是修该的时候要求用有效用户id能有权限修改

实验如下：
首先需要知道一点，setuid和seteuid需要权限，所以执行函数的时候是sudo执行函数的，`root`的`uid = euid = 0`
```cpp
//test.cpp 被exec调用的可执行文件源码
#include<cstring>
#include<errno.h>
#include<iostream>
#include<cstdio>
#include<unistd.h>
#include<stdlib.h>
using namespace std;
int main()
{
	setuid(1000);
	puts(strerror(errno));
	printf("euid is ,%d uid is %d\n",geteuid(),getuid());
	setuid(1001);
	puts(strerror(errno));
	printf("afterchange \neuid is ,%d uid is %d",geteuid(),getuid());

	return 0;
}


```

```cpp
//exec.cpp 调用test可执行文件的源代码
#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<stdio.h>
#include<cstring>
using namespace std;
int main()
{
	char buffer[100];

	sprintf(buffer,"child is uid:%d, euid:%d",getuid(),geteuid());
	puts(buffer);
	seteuid(1001);
	printf("seteuid status is %s\n",strerror(errno));

	sprintf(buffer,"child is uid:%d, euid:%d",getuid(),geteuid());
	puts(buffer);
	if(execl("./test","test",NULL) == -1){
		printf("exec fail is %s\n",strerror(errno));
	}
	return 0;
}

```


```shell

(base) skt1faker@SKT1Faker:~/my_procedure/c++/exec$ ./exec
child is uid:1000, euid:1000
seteuid status is Operation not permitted
child is uid:1000, euid:1000
in test
Success
euid is ,1000 uid is 1000
Operation not permitted
afterchange 
euid is ,1000 uid is 1000
(base) skt1faker@SKT1Faker:~/my_procedure/c++/exec$ sudo ./exec
child is uid:0, euid:0
seteuid status is Success
child is uid:0, euid:1001
in test
Operation not permitted
euid is ,1001 uid is 0
Operation not permitted
afterchange 
euid is ,1001 uid is 0


```

出现这种情况的原因是有效用户id从0（root）切换到其他有效用户id之后修改就不能使用setuid,我们将exec的源码修改一下
```cpp
#include<iostream>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<stdio.h>
#include<cstring>
using namespace std;
int main()
{
	char buffer[100];

	sprintf(buffer,"child is uid:%d, euid:%d",getuid(),geteuid());
	puts(buffer);
	//seteuid(1001);		// 将修改eid的位置注释掉哟
	//printf("seteuid status is %s\n",strerror(errno));

	sprintf(buffer,"child is uid:%d, euid:%d",getuid(),geteuid());
	puts(buffer);
	if(execl("./test","test",NULL) == -1){
		printf("exec fail is %s\n",strerror(errno));
	}
	return 0;
}
```
发现这种情况下在exec中可以修改uid和euid

```shell
(base) skt1faker@SKT1Faker:~/my_procedure/c++/exec$ sudo ./exec
child is uid:0, euid:0
child is uid:0, euid:0
in test
Success
euid is ,1000 uid is 1000
Operation not permitted
afterchange
euid is ,1000 uid is 1000

```

总结起来就是，只要euid权限够，就能修改随便修改uid和euid,

经过查阅一些文件发现

修改进程UID的规则

1、超级管理管理用户有权调用setuid,修改三个用户ID为指定uid
2、普通用户有权设置有效用户ID为实际用户ID或保存设置的UID
3、exec执行时会设置有效用户ID为执行程序的setuid
