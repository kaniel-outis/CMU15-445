Lecture 1 ～  4

1.关系型数据库（(*PostgreS*QL、Mysql、SQLite)，SQLite(free)运用最广泛，主要在移动端市场。第一次接触SQLite是之前在写Django项目的时候，当时好像是因为SQLite可以在编辑器直接看到，比较方便就直接用了，没想到是运用最广泛的。

  课上老师是在三个数据库之间来回切换，给我展示下在不同的database下操作的情况(date|time)。

~~不正经总结：1）   高情商：Mysql不会报错 ｜ 低情商：Mysql没一个算对的~~ 

总结：不同的database有不同做法。



2.重定向(写入新表 ｜ 写入已存在)，这个其实有点类似Shell里的重定向。

 order by （**最后**对结果排序）   



3.关系代数



4.Advaned SQL

1）嵌套查询（时间复杂度）

2）windows function （eg. 取每门课最高分的人，rank）

3）CTE（with 能递归）



总结：前2个lecture差不多就是国内大半的课程



5.Relational DBs ｜ Storage ｜ Execution ｜ Concurrency control ｜ Recovery ｜ Distributed DBs



6.存储体系

​		cpu Registers

​		cpu caches                         Volatile、Random access

​		DRAM                

===================

 		SSD

​		HDD									Non-Volatile、sequential access

​		Network store





7. ​	Memory         buffer pool       <----> execution engine         

   ​	=======================

   ​	Disk         DBfiles



8.why not use the os?

Os 像货车（通用）；保时捷    （很有意思的比喻）



9.

​									OLAP（A：analysize）(column store) ----> hadoop



OLTP（T：transaction）(row store)

(redis)



