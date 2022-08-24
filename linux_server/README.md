# Q1:close一个socket之后相当于主动关闭一个链接，就不能接收这个socket之后发来的信息，send函数不能让服务器accept一个连接。
测试方法：
1. 打开44行的break， 客户端不断给服务器发信息，然后服务器突然关闭链接，接着accept一个链接，客户端仍然send信息，此时send和accept是无法连接上的.
2. 将44行break注释掉，close一个socket链接之后，就没有办法recv这个socket的信息了。