create database if not exists YGGL;
use YGGL;

create table Departments(
                            DepartmentID char(3) not null primary key,
                            DepartmentName char(20) not null,
                            Note varchar(100)
);
select *
from Departments;

create table Employees(
    EmployeeID char(6) not null primary key,
    Name  char(10) not null,
    Education  char(4) not null ,
    Birthday date not null ,
    Sex bit not null default 1,
    Workyer int,
    Address varchar(40),
    PhoneNumber char(12),
    DepartmentID char(3) not null,
    foreign key(DepartmentID) references Departments(DepartmentID)
);
drop table Employees;


create table Salary(
    EmployeeID char(6)  not null primary key,
    Income float not null,
    Outcome float not null
);

/*
show databases;
drop database YGGL;

show tables;
desc Employees;
select *
from Employees;

alter table Employees rename E;
alter table Employees add age int;
alter table Employees change Name Name char(10);
alter table Employees modify Name int;
alter table Employees drop Name;
alter table Employees drop column Name; -- ???
alter table Departments rename column Note to n;

-- delete big to use drop
drop table if exists Employees;

-- other to use delete + from + where
delete
from Employees
where Employees.Name = 1;

 */

insert into Departments values (0, '研发部', null);
insert into Departments values (1, '市场部', null);
insert into Departments values (2, '销售部', null);
insert into Departments values (3, '咨询部', null);
insert into Departments values (4, '项目部', null);
insert into Departments values (5, '生产部', null);
insert into Departments values (6, '财务部', null);
insert into Departments values (7, '人力资源部', null);
insert into Departments values (8, '经理办公室', null);
insert into Departments values (9, '秘书办', null);

insert into Employees values('0', 'a', '小学', 20000101, 0, 1, '0', 0, 0);
insert into Employees values('1', 'b', '高中', 20000201, 1, 2, '1', 0, 0);
insert into Employees values('2', 'c', '高中', 20000301, 0, 3, '2', 0, 1);
insert into Employees values('3', 'd', '高中', 20000401, 0, 3, '3', 0, 0);
insert into Employees values('4', 'e', '高中', 20000501, 1, 4, '4', 0, 8);
insert into Employees values('5', 'f', '高中', 20000601, 0, 7, '5', 0, 2);
insert into Employees values('6', 'g', '高中', 20000701, 0, 6, '6', 0, 0);
insert into Employees values('7', 'h', '高中', 20000801, 1, 8, '7', 0, 3);
insert into Employees values('8', 'i', '大学', 20000901, 0, 9, '8', 0, 4);
insert into Employees values('9', 'j', '大学', 20010101, 0, 2, '9', 0, 9);
insert into Employees values('10', 'k', '大学', 20020101, 0, 3, '10', 0, 3);
insert into Employees values('11', 'l', '大学', 20030101, 1, 8, '11', 0, 2);
insert into Employees values('12', 'm', '大学', 20040101, 0, 9, '12', 0, 5);
insert into Employees values('13', 'n', '大学', 20050101, 0, 3, '13', 0, 9);
insert into Employees values('14', 'o', '大学', 20060101, 0, 7, '14', 0, 8);

insert into Salary values('0', 2000, 1000), ('1', 3000, 1000), ('2', 4000, 1000), ('3', 5000, 1000), ('4', 6000, 1000),
                          ('5', 7000, 1000), ('6', 8000, 1000), ('7', 9000, 1000), ('8', 1000, 1000), ('9', 2000, 1000),
                          ('10', 3000, 1000), ('11', 4000, 1000), ('12', 5000, 1000), ('13', 6000, 1000),
                          ('14', 7000, 1000);

select Salary.EmployeeID, Name, Education, Birthday, Sex, Workyer, Address, PhoneNumber, Employees.DepartmentID, DepartmentName, Note, Income, Outcome
from Employees, Departments, Salary
where Employees.DepartmentID = Departments.DepartmentID and Salary.EmployeeID = Employees.EmployeeID;

select EmployeeID, PhoneNumber, Address
from Employees;

select E.EmployeeID
from Employees as E, Salary as S
where E.EmployeeID = S.EmployeeID and S.Income > 6000;

select E.EmployeeID, (S.Income - S.Outcome) as salary
from Employees as E, Salary as S
where E.EmployeeID = S.EmployeeID
group by E.EmployeeID;

select count(Name) as "财物部的员工总数"
from Employees, Departments
where Employees.DepartmentID = Departments.DepartmentID and DepartmentName = '财务部';

select Name as "姓名", Birthday as "出生日期"
from Employees
where Sex = 1;


-- 2
select Name, address,
case
    when Income < 3000 then '低收入'
    when Income > 7000 then '高收入'
    else '中收入'
end as Income
from Employees natural join Salary;

select Name
from Employees natural join Departments
where DepartmentName = '研发部' and Birthday <= all(
    select Birthday
    from Employees natural join Departments
    where DepartmentName = '销售部'
    );

select Name, Income
from (Employees natural join Salary) natural join Departments
where DepartmentName = '研发部' and Income > 5000;

with FinanceIncome as (
    select Income, Name
    from (Employees natural join Salary) natural join Departments
    where DepartmentName = '研发部'
)
select Name,
case
    when Income = (
        select max(Income)
        from FinanceIncome
    ) then concat(cast(Income as varchar(10)), '(最高收入)')
    when Income = (
        select min(Income)
        from FinanceIncome
    ) then concat(cast(Income as varchar(10)), '(最低收入)')
    else Income
end as Income
from FinanceIncome
where Income >= all ( -- Income's scope ???
        select Income
        from FinanceIncome
    ) or
    Income <= all (
        select Income
        from FinanceIncome
    );
/* why can't?
case
when Income = max(Income) then concat(cast(max(Income) as varchar(10)), '(最高收入)')
when Income = min(Income) then concat(cast(min(Income) as varchar(10)), '(最低收入)')
end as income
 */

create view Employees_view as
    select PhoneNumber, Name, (Income - Outcome) as wage
    from Employees natural join Salary;
select *
from Employees_view;

create view DS_VIEW as
    select *
    from Departments;
select *
from DS_VIEW;

select Name, wage
from Employees_view
where Name = 'a';

/*
Modify the Employees_view?

- Can't

1. The from clause has only one database relation
2. The select clause contains only attribute names of the relation and does not have any
expressions, aggregates, or distinct specification.
(wage is a expression!)
3. Any attribute not listed in the select clause and be set to null; that is, it does not
have a not null constraint and is not part of a primary key.
4. The query does not have a group by or having clause.


 */

-- It can only have two clauses 'with' at most?

/*-- wrong:
Where Income >= all Income

-- correct:
where Income >= all (
        select Income
        from FinanceIncome
    )

from FinanceIncome as S, FinanceIncome as T
-- wrong:
when S.Income >= all(T.Income)
-- correct:
when S.Income > T.Income
*/

create table tes(
    Name varchar(10),
    age int
);

insert into tes values ('a', 1), ('b', 2);

create view j as
select *
from tes
where age = 1;
-- with check option;

insert into j value ('c', 2); -- value in table!
drop view j;

create view j as
select Name
from tes
where age = 1;

insert into j value ('b'); -- value in table! others is null!

/*
select user();
select current_user();
select user
from mysql.user;
desc user;

select user, host, account_locked, password_expired
from liukanglai@localhost;

select user, host, db, command
from information_schema.processlist;
 */

-- 三

select
case
    when sex = 1 then concat(cast(count(Name) as varchar(10)), '(男性)')
    when sex = 0 then concat(cast(count(Name) as varchar(10)), '(女性)')
end as '人数'
from Employees
group by sex;

-- scalar
select distinct case
    when Sex = 1 then '男性'
    when Sex = 0 then '女性'
end as sex, (select count(*)
    from Employees as y  -- from Employees natural join Departments; wrong!
    where x.Sex = y.sex) as number
from Employees as x;

select DepartmentName, count(Name) as '员工人数'
from Employees natural join Departments
group by DepartmentName
having count(Name) > 2;

select Name as '姓名', Birthday as '出生日期'
from Employees natural join Departments
where DepartmentName = '研发部';

-- 4
create user '王明';
grant select, insert, delete on YGGL.* to '王明';
create user '刘星';
grant select on Employees to '刘星';
grant update on Employees.Address to '刘星';
grant update(Address) on Employees to '刘星';
create user '张新';
grant select, insert, delete, update on YGGL.* to '张新';
-- grant alter table on YGGL.* to '张新';
create user '杨兰';
create view Em_come as
    select DepartmentName, max(Income), min(Income), avg(Income)
    from Employees natural join Departments natural join Salary
    group by DepartmentName;
grant select on Em_come to '杨兰';

-- 5
create table ES_U(
    Eno varchar(10) primary key,
    OldIncome int check (OldIncome between 2000 and 20000),
    NewIncome int not null check (NewIncome between 2000 and 20000)
);

-- 6
alter table Employees modify Sex char(2);

alter table Employees modify Sex bit not null default 1;

-- change the values at first.
update Employees
set Sex = case
              when Sex = 'M' then '1'
              else '0'
    end;

update Employees
set Sex = case
        when Sex = '1' then 'M'
        else 'F'
    end;

-- how to delete the check and show it?
alter table Employees
    add check (Sex = '男' or Sex = '女' or Sex = 'F' or Sex = 'M');
-- alter table Employees
-- add constraint name_sex check (Sex = '男' or Sex = '女' or Sex = 'F' or Sex = 'M');

alter table Employees drop constraint CONSTRAINT_1;

-- 7
insert into ES_U values('a', 0, 0);
insert into Employees values('100', 'a', '小学', 20000101, 0, 1, '0', 0, 0);

-- 8
create trigger raise_salary before update on ES_U
    for each row
    begin
        if NEW.NewIncome > OLD.NewIncome then
            set NEW.OldIncome = OLD.NewIncome;
        end if;
    end;

insert into ES_U values ('a', null, 5000);

update ES_U
set NewIncome = '6000'
where Eno = 'a';

select *
from ES_U;

-- 9
create table Employees_log(
    Num_employees int
);

insert into Employees_log value((
    select count(*)
    from Employees
    ));

--  Unsafe query: 'Update' statement without 'where' updates all table rows at once
create trigger add_num_employees after delete on Employees
    for each row
    begin
        update Employees_log
            set Num_employees = (select count(*) from Employees)
            where 1;
    end;

create trigger minus_num_employees after insert on Employees
    for each row
    begin
        update Employees_log
            set Num_employees = (select count(*) from Employees)
            where 1;
    end;

delete from Employees
where EmployeeID = '1';
insert into Employees values('1', 'b', '高中', 20000201, 0, 2, '1', 0, 0);

select *
from Employees_log;

select *
from Employees;
-- 10

-- function must have return...
create procedure abort()
    begin
        rollback;
    end;

create trigger check_department before delete on Departments
    for each row
    begin
        if OLD.DepartmentID in (select DepartmentID from Employees) then
            -- how to terminate the operation?
            -- signal sqlstate '45000'; -- 45000 or 45001 ?
            call abort();
        end if;
    end;

delete from Departments
where DepartmentID = 0;

-- 四

-- MySQL doesn't have user-defined types.
-- set @Employee_num = char(6), not null;

set @wage = (
    select Income - Outcome
    from Salary
    where EmployeeID = '1'
);

select @wage as wage;

set @wagea = (
     select Income - Outcome
     from Salary natural join Employees
     where Name = 'a'
    );
select @wagea as wage;

create procedure judge()
begin
    if @wagea > 6000
        then  select 'a', @wagea;
    else
         select 'a', '收入不高于6000';
    end if;
end;

call judge();

drop procedure print;
create procedure print(number int)
    begin
        declare flag int;
        set flag  = 0;
        while number > 0 do
            set number =  number -1;
        end while;
    end;

call print(5);
-- mysql haven't print!!!

SET @NUMBER = 6;
SELECT REPEAT('* ', @NUMBER := @NUMBER - 1) as 'triangle'
    FROM information_schema.tables LIMIT 5;

create table Income_loop_num(
    ID char(6),
    Loop_num int
);

create procedure addIncome(add_num int)
begin
    declare number int;
    declare i int;
    declare count int;
    declare tem_income int;
    declare tem_id char(6);
    select count(*) from Salary into number;
    set i = 0;
    set count = 0;
    while i < number do
            select Income, EmployeeID from Salary1 LIMIT i, 1 into tem_income, tem_id;
            if tem_income < 4000 then
                while tem_income < 6000 do
                    set tem_income = tem_income + add_num;
                    set count = count +1;
                    end while;
                update Salary1 set Income = tem_income where EmployeeID = tem_id;
                insert into Income_loop_num value (tem_id, count);
            end if;
            set count = 0;
        set i = i + 1;
    end while;
end;

call addIncome(50);

select *
from Salary1;

select *
from Income_loop_num;

create table Salary1 like Salary;
insert into Salary1 select * from Salary;

create table grading like Salary;
insert into grading select * from Salary;

alter table grading add grade int;

create procedure grading()
begin
    declare number int;
    declare i int;
    declare tem_income int;
    declare tem_id char(6);
    declare tem_grade int;
    set i = 0;
    select count(*) from grading into number;
    while(i < number) do
        select Income, EmployeeID into tem_income, tem_id from grading limit i, 1;
        if(tem_income < 3000) then
            set tem_grade = 1;
        else
            set tem_grade = (tem_income - 1000) / 1000;
        end if;
        update grading set grade = tem_grade where EmployeeID = tem_id;
        set i = i+1;
        set tem_grade = 0;
        end while;
end;

drop procedure grading;
call grading();

select *
from grading;

select DepartmentName, count(*), max(Income), min(Income), avg(Income)
from Employees natural join Departments natural join Salary
group by DepartmentName;

create procedure compare_income(name1 char(6), name2 char(6))
begin
    declare income1, income2 int;
    select Income into income1 from Salary where EmployeeID = name1;
    select Income into income2 from Salary where EmployeeID = name2;
    if income1 >= income2 then
        select 0;
    else
        select 1;
    end if;
end;

drop procedure compare_income;
call compare_income('1', '0');

select *
from Salary;

create procedure add_income_by_education()
begin
    declare tem_education char(4);
    declare number, i int;
    declare tem_income int;
    declare tem_id char(6);
    set i = 0;
    select count(*) from Employees natural join Salary2 into number;
    while(i < number) do
        select Income, EmployeeID, Education into tem_income, tem_id, tem_education
        from Employees natural join Salary2 limit i, 1;
        case tem_education
            when '小学' then set tem_income = tem_income + 400;
            when '大学' then set tem_income = tem_income + 800;
            when '高中' then set tem_income = tem_income + 1200;
        end case;
        update Salary2 set Income = tem_income where EmployeeID = tem_id;
        set i = i+1;
        end while;
end;

select *
from Salary2;

select *
from Salary;

call add_income_by_education();

create table Salary2 like Salary;
insert into Salary2 select * from Salary;

create procedure cursor_move()
begin
    declare done int default false;
    declare tem_id char(6);
    declare tem_workyer char(3);
    declare need_id char(3);
    declare cursor_i cursor for select EmployeeID, Workyer from Employees;
    declare continue handler for not found set done = true;

  select DepartmentID into need_id from Departments where DepartmentName = '经理办公室';

  open cursor_i;
  read_loop: LOOP
    fetch cursor_i into tem_id, tem_workyer;
    if done then
      leave read_loop;
    END IF;
    if tem_workyer > 10 then
        update Employees1 set DepartmentID = need_id where EmployeeID = tem_id;
    end if;
  end loop;
  close cursor_i;
end;

create table Employees1 like Employees;
insert into Employees1 select * from Employees;

select *
from Employees1;
call cursor_move();


create procedure compute()
begin
    declare done int default false;
    declare tem_education char(4);
    declare total int;
    declare number_education int;
    declare cursor_education cursor for select Education from Employees;
    declare continue handler for not found set done = true;

    set number_education = 0;
    select count(*) into total from Employees;
    open cursor_education;
    label: loop
    fetch cursor_education into tem_education;
    if done then
        leave label;
    end if;
    if tem_education != '小学' then
        set number_education = number_education + 1;
    end if;
    end loop;
    close cursor_education;
    select number_education / total;
end;

call compute();

create procedure judge_rank(id char(6))
begin
    declare tem_id char(6);
    declare count int;
    declare flag int;
    declare cursor1 cursor for select EmployeeID from Salary order by Income desc;
    set count = 0;
    set flag = 0;

    open cursor1;
    label: loop
        fetch cursor1 into tem_id;
    if count = 3 then
        leave label;
    end if;
    if tem_id = id then
        set flag = 1;
        leave label;
    end if;
    set count = count + 1;
    end loop;
    if flag then
        select id, '1' as if_first_3;
    else select id, '0' as if_first_3;
    end if;
    close cursor1;
end;

drop procedure judge_rank;

select * from Salary order by Income desc;
call judge_rank('7');
call judge_rank('6');
call judge_rank('4');
