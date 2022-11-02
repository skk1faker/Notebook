## 参考
http://t.zoukankan.com/qbmiller-p-4046954.html

write username [tty]
msg
ctrl + D
就可以将信息msg发给username在tty中
tty的查看可以根据who命令来查，例如：

(SWINTS) wangxinpeng@server1:~/car/SwinTextSpotter$ who
cuiqing  tty7         2022-09-20 15:07 (:0)
cuiqing  pts/2        2022-10-02 11:16 (10.71.255.176)
cuiqing  pts/19       2022-10-02 11:16 (10.71.255.176)
wangxinpeng pts/21       2022-10-06 13:16 (10.68.1.88)
cuiqing  pts/29       2022-10-05 02:55 (10.60.26.22)
cuiqing  pts/26       2022-10-05 12:16 (10.60.26.22)
duaihui  pts/31       2022-10-05 21:50 (10.60.26.22)

其中的pts/21 就是wangxinpeng的某一个tty，使用write wangxinpeng pts/21 就可以给在pts/21的王鑫鹏发消息。

who
检查在线情况

mesg y 表示接收所有人发来的消息
mesg n 表示拒绝所有人发来的消息
