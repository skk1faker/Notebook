//https://kalacloud.com/blog/how-to-create-a-new-user-and-grant-permissions-in-mysql/    数据库存授权！

// https://www.169it.com/it-mysql/article-mysql-12284902738511381427/  windows  下的示范。

//其他相关资料
// keping.me/linux-cpp-mysql/
// https://github.com/bskari/mysql-cpp/blob/master/MySql.cpp
// https://www.google.com.hk/search?q=linux+c%2B%2B操作mysql&newwindow=1&hl=zh-CN&biw=1600&bih=763&sxsrf=ALiCzsY8O1nnoqypzUzbToukHKperYpxsg%3A1661658711411&ei=V-YKY6zhGJWC-Aajyq2wAg&ved=0ahUKEwjs6OyJ0ej5AhUVAd4KHSNlCyYQ4dUDCA0&uact=5&oq=linux+c%2B%2B操作mysql&gs_lcp=Cgdnd3Mtd2l6EAM6BwgAEEcQsAM6BAgjECc6BQgAEIAEOgsILhCABBDHARDRAzoECAAQQzoKCC4QxwEQ0QMQQzoOCC4QgAQQxwEQ0QMQ1AI6CAguEIAEENQCOgUIABDLAToHCAAQgAQQDDoHCAAQChDLAToFCCEQoAE6BAgAEB5KBAhBGABKBAhGGABQjwVYnYABYPmCAWgIcAF4AYAB5gGIAb4kkgEGNC4zMS4ymAEAoAEByAEKwAEB&sclient=gws-wiz

// 源码来源
//https://softexpert.wordpress.com/2007/10/18/how-to-connect-to-a-mysql-db-using-c-linux/          linux如何使用mysql
#include<mysql/mysql.h>
#include<string>
#include<iostream>

using std::cout;
using std::endl;

int main()
{

    MYSQL_RES *result;

    MYSQL_ROW row;

    MYSQL *connection, mysql;
    int state;
    mysql_init(&mysql);
    std::string host = "localhost";
    std::string usr = "wxp";
    std::string pswd = "wang1_";
    std::string database = "wang_database";

    connection = mysql_real_connect(&mysql,host.c_str(),usr.c_str(),pswd.c_str(),database.c_str(),0,0,0);   // connection 和&mysql实际上是一个地址

    if (connection == NULL)
    {
        printf(mysql_error(&mysql));
        return 1;
    }
    state = mysql_query(&mysql, "SELECT * FROM fun_table"); // 这句话传入mysql能运行的语句，成功运行则返回0
    if (state !=0)
    {
        printf(mysql_error(connection));    // 如果不成功根据这个mysql_error就能判断哪里不正确
        return 1;
    }
    result = mysql_store_result(connection);
    printf("Rows:%d\n",mysql_num_rows(result));
    cout<<mysql_num_fields(result)<<endl;
    for( int i = 0;i < mysql_num_fields(result);i++){
        // cout<<typeid(mysql_fetch_field(result)).name()<<'\t';
        // typeid().name();的使用
        // https://blog.csdn.net/cpp_learner/article/details/120314388
        MYSQL_FIELD *field = mysql_fetch_field(result);     
        cout<<field->table<<'\t';   // 数据库的表名字
        cout<<field->db<<'\t';      // 数据库名
        cout<<field->name<<'\t';    // 表的各项名字
        cout<<endl;
    }
    cout<<endl;
    if(mysql_fetch_field(result) == NULL){      // 最后没有列的时候会返回NULL
        cout<<"最后没有列的时候会返回NULL"<<endl;
    }
    while ( ( row=mysql_fetch_row(result)) != NULL )
    {
        // printf(" %s, %s\n", (row[0] ? row[0] : "NULL"), (row[1] ? row[1] : "NULL" ));
        printf("%s\t%s\t%s\n", row[0], row[1],row[2]);
    }
    mysql_free_result(result);

mysql_close(connection); 
}

// 操作方法api
// https://blog.csdn.net/Tanswer_/article/details/72796570
// https://blog.csdn.net/fengxinlinux/article/details/75675360
// https://blog.csdn.net/qq_25908839/article/details/102753510

/*

获取或初始化MYSQL结构
MYSQL *mysql_init(MYSQL *mysql)	

连接到MySQL服务器。
MYSQL *mysql_real_connect(MYSQL *mysql, const char *host, const char *user, const char *passwd, const char *db, unsigned int port, const char *unix_socket, unsigned long client_flag)

执行指定为“以Null终结的字符串”的SQL查询。
int mysql_query(MYSQL *mysql, const char *stmt_str)

将查询的整个结果读取到客户端，分配一个 MYSQL_RES结构，并将结果放入此结构中
MYSQL_RES *mysql_store_result(MYSQL *mysql)  // 对于返回值，我们常用下面的函数处理

返回上次执行语句的结果集的列数。
unsigned int mysql_field_count(MYSQL *mysql)

返回结果集中的列数。
unsigned int mysql_num_fields(MYSQL_RES *result)

返回结果集中的行数。
my_ulonglong mysql_num_rows(MYSQL_RES *result)

从结果集中获取下一行数据
MYSQL_ROW mysql_fetch_row(MYSQL_RES *result)    // 这个是一行一行获取，调用之后返回下一行的结果，这个结果类似一个数组，利用数组访问此行的列结果	

与之对应的获取一个列的头部
MYSQL_FIELD* mysql_fetch_field(MYSQL_RES *result)
MYSQL_FIELD -> name     // 标头名字
MYSQL_FIELD -> db       // 数据库名字
MYSQL_FIELD -> table    // 数据库的表格名字

对于一个指令有可能会出错，比如连接时或者查询的时候，
mysql_error(MYSQL *mysql)返回上一个 MySQL 操作产生的文本错误信息。


释放结果集空间
void mysql_free_result(MYSQL_RES *result)	

*/




/*
最全api
https://zhuanlan.zhihu.com/p/339021526
1.mysql_affected_rows() 返回被最新的UPDATE, DELETE或INSERT查询影响的行数。

2.mysql_close() 关闭一个服务器连接。

3.mysql_connect() 连接一个MySQL服务器。该函数不推荐；使用mysql_real_connect()代替。

4.mysql_change_user() 改变在一个打开的连接上的用户和数据库。

5.mysql_create_db() 创建一个数据库。该函数不推荐；而使用SQL命令CREATE DATABASE。

6.mysql_data_seek() 在一个查询结果集合中搜寻一任意行。

7.mysql_debug() 用给定字符串做一个DBUG_PUSH。

8.mysql_drop_db() 抛弃一个数据库。该函数不推荐；而使用SQL命令DROP DATABASE。

9.mysql_dump_debug_info() 让服务器将调试信息写入日志文件。

10.mysql_eof() 确定是否已经读到一个结果集合的最后一行。这功能被反对; mysql_errno()或mysql_error()可以相反被使用。

11.mysql_errno() 返回最近被调用的MySQL函数的出错编号。

12.mysql_error() 返回最近被调用的MySQL函数的出错消息。

13.mysql_escape_string() 用在SQL语句中的字符串的转义特殊字符。

14.mysql_fetch_field() 返回下一个表字段的类型。

15.mysql_fetch_field_direct () 返回一个表字段的类型，给出一个字段编号。

16.mysql_fetch_fields() 返回一个所有字段结构的数组。

17.mysql_fetch_lengths() 返回当前行中所有列的长度。

18.mysql_fetch_row() 从结果集合中取得下一行。

19.mysql_field_seek() 把列光标放在一个指定的列上。

20.mysql_field_count() 返回最近查询的结果列的数量。

21.mysql_field_tell() 返回用于最后一个mysql_fetch_field()的字段光标的位置。

22.mysql_free_result() 释放一个结果集合使用的内存。

23.mysql_get_client_info() 返回客户版本信息。

24.mysql_get_host_info() 返回一个描述连接的字符串。

25.mysql_get_proto_info() 返回连接使用的协议版本。

26.mysql_get_server_info() 返回服务器版本号。

27.mysql_info() 返回关于最近执行得查询的信息。

28.mysql_init() 获得或初始化一个MYSQL结构。

29.mysql_insert_id() 返回有前一个查询为一个AUTO_INCREMENT列生成的ID。

30.mysql_kill() 杀死一个给定的线程。

31.mysql_list_dbs() 返回匹配一个简单的正则表达式的数据库名。

32.mysql_list_fields() 返回匹配一个简单的正则表达式的列名。

33.mysql_list_processes() 返回当前服务器线程的一张表。

34.mysql_list_tables() 返回匹配一个简单的正则表达式的表名。

35.mysql_num_fields() 返回一个结果集合重的列的数量。

36.mysql_num_rows() 返回一个结果集合中的行的数量。

37.mysql_options() 设置对mysql_connect()的连接选项。

38.mysql_ping() 检查对服务器的连接是否正在工作，必要时重新连接。

39.mysql_query() 执行指定为一个空结尾的字符串的SQL查询。

40.mysql_real_connect() 连接一个MySQL服务器。

41.mysql_real_query() 执行指定为带计数的字符串的SQL查询。

42.mysql_reload() 告诉服务器重装授权表。

43.mysql_row_seek() 搜索在结果集合中的行，使用从mysql_row_tell()返回的值。

44.mysql_row_tell() 返回行光标位置。

45.mysql_select_db() 连接一个数据库。

46.mysql_shutdown() 关掉数据库服务器。

47.mysql_stat() 返回作为字符串的服务器状态。

48.mysql_store_result() 检索一个完整的结果集合给客户。

49.mysql_thread_id() 返回当前线程的ID。

50.mysql_use_result() 初始化一个一行一行地结果集合的检索。 
*/