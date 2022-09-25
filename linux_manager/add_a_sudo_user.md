## 参考
0. 查看用户的gid和group
id username
```shell
(base) skt1faker@SKT1Faker:~$ id luluzi
uid=1001(luluzi) gid=1000(skt1faker) groups=1000(skt1faker),27(sudo)
```

1. 如何修改用户的gid

改变uid
skt1faker的uid是1000，luluzi的uid是1001
所以luluzi的uid改为1000会出错，但是改为1002没有问题

```shell

(base) skt1faker@SKT1Faker:~$ id luluzi
uid=1001(luluzi) gid=1000(skt1faker) groups=1000(skt1faker),27(sudo)
(base) skt1faker@SKT1Faker:~$ usermod -u 1000 luluzi
usermod: UID '1000' already exists
(base) skt1faker@SKT1Faker:~$ sudo usermod -u 1002 luluzi
[sudo] password for skt1faker: 
(base) skt1faker@SKT1Faker:~$ id luluzi
uid=1002(luluzi) gid=1000(skt1faker) groups=1000(skt1faker),27(sudo)
```

改变gid
```shell
(base) skt1faker@SKT1Faker:~$ sudo usermod -g 1000 luluzi
(base) skt1faker@SKT1Faker:~$ id luluzi
uid=1001(luluzi) gid=1000(skt1faker) groups=1000(skt1faker),27(sudo)
(base) skt1faker@SKT1Faker:~$ sudo usermod -g sudo luluzi
(base) skt1faker@SKT1Faker:~$ id luluzi
uid=1001(luluzi) gid=27(sudo) groups=27(sudo)

```
https://www.thegeekdiary.com/how-to-correctly-change-the-uid-and-gid-of-a-user-group-in-linux/

2. 将一个用户加入到一个groups中,**注意这个是一个用户权限的关键，要想使用sudo命令必须将用户加入sudo的组里面**
```shell
(base) skt1faker@SKT1Faker:~$ id luluzi
uid=1001(luluzi) gid=27(sudo) groups=27(sudo)
(base) skt1faker@SKT1Faker:~$ sudo usermod -G skt1faker luluzi
(base) skt1faker@SKT1Faker:~$ id luluzi 
uid=1001(luluzi) gid=27(sudo) groups=27(sudo),1000(skt1faker)

```
https://www.geeksforgeeks.org/shell-script-to-give-root-privileges-to-a-user/

2. 将一个用户移出groups

`gpasswd -d userName groupName`

```shell
(base) skt1faker@SKT1Faker:~$ id luluzi
uid=1001(luluzi) gid=1000(skt1faker) groups=1000(skt1faker),27(sudo)
(base) skt1faker@SKT1Faker:~$ sudo gpasswd -d luluzi sudo
Removing user luluzi from group sudo
(base) skt1faker@SKT1Faker:~$ id luluzi
uid=1001(luluzi) gid=1000(skt1faker) groups=1000(skt1faker)

```

https://blog.csdn.net/mydriverc2/article/details/113901715




2. 如何添加一个用户

`adduser username --home /home/username  `
当然还有其他选项在文中介绍

https://www.geeksforgeeks.org/adduser-command-in-linux-with-examples/

当然还可以useradd，但是这种方法比较低级（我也不知道那里低级）
https://linuxize.com/post/how-to-create-users-in-linux-using-the-useradd-command/