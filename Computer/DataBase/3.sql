
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
insert into Employees values('1', 'b', '高中', 20000201, 'M', 2, '1', 0, 0);

select *
from Employees_log;

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
