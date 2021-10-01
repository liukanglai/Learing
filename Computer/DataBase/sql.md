# sql

> DDL DML DQL DCL

1. 
  - ; end
  - ingore case
  - comment: -- #; `/* */`

## database

1. create

        create database Student;
        create database if not exists Student;
        create database Student character set utf-8;

2. retrieve

        show databases;
        show create database Student;

3. update

        alter database Student character set utf-8;

4. delete

        drop database Student;
        drop database if exists Student;

5. change

        select database();
        use Student;

## table

> Except number, all other types need to use '' or "" to include.

        create table Sc(
            name varchar(20),
            age int,
            score double (3, 7),
            birthday data,
            birthday datatime,
            insert_time timestamp,
            );
        # copy:
        create table Sc like some;

        show tables;
        desc Sc;

        select * from Sc;
        select name, age from Sc;
        select 字段列表, from 表名列表, where 条件列表, group by 分组字段, having 分组之后的条件, order by 排序, limit 分页限定

        alter table Sc rename to Sc1;
        alter table Sc add height int;
        alter table Sc change height height1 int;
        alter table Sc modify height double;
        alter table Sc drop height;

        insert into Sc(name, age) values('j', 12);

        update Sc set name = 'j' where age = 11;

        drop table Sc;
        drop table if exists Sc;

        distinct # eliminate repetition

        delete from Sc where age < 10;
        delete from 表名；-- 不推荐使用。有多少条记录就会执行多少次删除操作。
        TRUNCATE TABLE 表名； -- 推荐使用，效率更高 先删除表，然后再创建一张一样的表。

        if null(name, Name);
        as;


## 

        >,<,>=,<=,=,<>
        BETWEEN...AND
        IN(集合)
        LIKE
        IS NULL
        and or &&
        or or ||
        not or !
## 

        order by name (asc);
        order by name desc;

        count(*)
        max
        min
        sum
        avg

        group by name;
        
        limit 1;

# constraint

        # change/delete constraint: alter
        primary key
        not null
        unique
        foreign key name references
        auto_increment


#

