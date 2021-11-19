
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

