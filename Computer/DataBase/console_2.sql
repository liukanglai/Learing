create database if not exists YGGL;
use YGGL;

create table Departments(
                            DepartmentID char(3) not null primary key,
                            DepartmentName char(20) not null,
                            Note varchar(100)
);

create table Employees(
    EmployeeID char(6) not null primary key,
    Name  char(10) not null,
    Education  char(4) not null ,
    Birthday date not null ,
    Sex bit not null default 1,
    Worker int,
    Address varchar(40),
    PhoneNumber char(12),
    DepartmentID char(3) not null,
    foreign key(DepartmentID) references Departments(DepartmentID)
);


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

drop table if exists Employees;
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

insert into Salary values('0', 2000, 1000);
insert into Salary values('1', 3000, 1000);
insert into Salary values('2', 4000, 1000);
insert into Salary values('3', 5000, 1000);
insert into Salary values('4', 6000, 1000);
insert into Salary values('5', 7000, 1000);
insert into Salary values('6', 8000, 1000);
insert into Salary values('7', 9000, 1000);
insert into Salary values('8', 1000, 1000);
insert into Salary values('9', 2000, 1000);
insert into Salary values('10', 3000, 1000);
insert into Salary values('11', 4000, 1000);
insert into Salary values('12', 5000, 1000);
insert into Salary values('13', 6000, 1000);
insert into Salary values('14', 7000, 1000);

select Salary.EmployeeID, Name, Education, Birthday, Sex, Worker, Address, PhoneNumber, Employees.DepartmentID, DepartmentName, Note, Income, Outcome
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
