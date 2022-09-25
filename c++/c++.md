## 一些语法知识

1. 继承权限
 继承的时候父类中private在任何继承权现下都无法访问，public和protected在继承权现为public、protected、private下分别变为（public、protected）(protected、protected) (private、private)
2. privated和protected的作用
private继承后子类无法访问，但是可以借助protected、public来间接访问父类的private
protected可以被继承
private和protected都不可一杯外界访问，但是可以借助类中的public进行访问


3. void f() throw(std::exception)	// 表示这个函数会抛出std::exception类型的异常。

4. class\_name \*\*temp =  new class\_name \*[100]表示声明一个大小为100的一个class\_name类型的指针数组

5. delete [] a能将a指向的一片区域析构,例如a指向a[0],a[1],a[2]..,delete []之后就会对a[0],a[2]...调用析构函数，delete a能将a指向的一个元素即a[0]析构。
https://blog.csdn.net/cbNotes/article/details/38900799
