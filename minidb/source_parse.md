## parse\_defs.h 定义了一些命令传来的参数应该以什么样子的数据形式存在，如Value，Selects
## trx.h 定义了用于记录对表操作的一个函数，所有记录会记录到表中，其中这里会调用operator中的一些函数
这个东西可以方便我们以后的回滚操作
上网查了一下，trx表示的是事务表。

## RID似乎是和内存有关，record.h
https://zhuanlan.zhihu.com/p/433654215
这篇文章揭示了slotnum和pagenum的关系

## Operation 实际给出了对于表的操作，用于记录操作，对于表的操作有两个信息，一个是操作类型，另一个是操作对象（就是在什么位置，这个也是RID所记录的东西）
operation 也对表进行实际的操作，不光是记录。
operation中child\_会记录一个操作序列，open

## Session类，用来给出当前会话的一些信息，
1. db记录的当前的数据库
2. trx记录了当前会话的操作记录。
3. 每一次会话

## stmt类表示个操作
1. table\*
2. value\*
3. value\_amount
## Tuple
元组（tuple）是关系数据库中的基本概念，关系是一张表，表中的每行（即数据库中的每条记录）就是一个元组，每列就是一个属性。 在二维表里，元组也称为行。


# TupleCell
元组中的单个值


#Field
元组中的列明，表示一个属性

# predicate_operator.cpp
用于过滤的输出的一个类

relation表示的是一个表
attribute表示表中的一个属性（就是一个列）

#0  PredicateOperator::do_predicate (this=0x7f7585579ec0, tuple=...)
    at /root/source/rcf/MiniOB-miniob_test/src/observer/sql/operator/predicate_operator.cpp:64
#1  0x000000000043017b in PredicateOperator::next (this=0x7f7585579ec0)
    at /root/source/rcf/MiniOB-miniob_test/src/observer/sql/operator/predicate_operator.cpp:44
#2  0x0000000000430cf1 in ProjectOperator::next (this=0x7f7585579e70) at /root/source/rcf/MiniOB-miniob_test/src/observer/sql/operator/project_operator.cpp:39
#3  0x00000000004273a3 in ExecuteStage::do_select (this=0x2273140, sql_event=0x7f7578000bb0)
    at /root/source/rcf/MiniOB-miniob_test/src/observer/sql/executor/execute_stage.cpp:440
#4  0x000000000042520d in ExecuteStage::handle_request (this=0x2273140, event=0x7f7578000bb0)
    at /root/source/rcf/MiniOB-miniob_test/src/observer/sql/executor/execute_stage.cpp:141
#5  0x0000000000424999 in ExecuteStage::handle_event (this=0x2273140, event=0x7f7578000bb0)
    at /root/source/rcf/MiniOB-miniob_test/src/observer/sql/executor/execute_stage.cpp:114
#6  0x0000000000435249 in OptimizeStage::handle_event (this=0x2273510, event=0x7f7578000bb0)
    at /root/source/rcf/MiniOB-miniob_test/src/observer/sql/optimizer/optimize_stage.cpp:87
#7  0x0000000000441f4f in QueryCacheStage::handle_event (this=0x2274450, event=0x7f7578000bb0)
    at /root/source/rcf/MiniOB-miniob_test/src/observer/sql/query_cache/query_cache_stage.cpp:86
#8  0x000000000043ce54 in ResolveStage::handle_event (this=0x2273cb0, event=0x7f7578000bb0)
    at /root/source/rcf/MiniOB-miniob_test/src/observer/sql/parser/resolve_stage.cpp:115
#9  0x000000000043a8de in ParseStage::handle_event (this=0x22738e0, event=0x7f7578000bb0)
    at /root/source/rcf/MiniOB-miniob_test/src/observer/sql/parser/parse_stage.cpp:104
#10 0x0000000000440621 in PlanCacheStage::handle_event (this=0x2274080, event=0x7f7578000bb0)
    at /root/source/rcf/MiniOB-miniob_test/src/observer/sql/plan_cache/plan_cache_stage.cpp:99
#11 0x0000000000423329 in SessionStage::handle_request (this=0x21eaab0, event=0x22707b0)
    at /root/source/rcf/MiniOB-miniob_test/src/observer/session/session_stage.cpp:171
#12 0x0000000000421e9b in SessionStage::handle_event (this=0x21eaab0, event=0x22707b0)
    at /root/source/rcf/MiniOB-miniob_test/src/observer/session/session_stage.cpp:105
#13 0x00007f75a0ec5181 in common::Threadpool::run_thread (pool_ptr=0x2270820) at /root/source/rcf/MiniOB-miniob_test/deps/common/seda/thread_pool.cpp:309
#14 0x00007f75a0a081cf in start_thread () from /lib64/libpthread.so.0
#15 0x00007f759ea39d83 in clone () from /lib64/libc.so.6
