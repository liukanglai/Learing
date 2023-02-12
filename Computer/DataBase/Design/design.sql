set names utf8;
drop database if exists FinalDesignDataBase;
create database FinalDesignDataBase charset utf8;
use FinalDesignDataBase;

# 用户
create table user
(
    id       int primary key auto_increment,
    username varchar(49),
    password varchar(49)
);
#drop table user;
#路飞 luffy (海贼船长）
insert into user
values (null, 'luffy', 'luffy');
#索隆 zoro (普通海贼）
insert into user
values (null, 'zoro', 'zoro');
#
insert into user
values (null, 'law', 'law');
#战国 zhanguo (政府）
insert into user
values (null, 'zhanguo', 'zhanguo');
#卡普 garp (海军队长）
insert into user
values (null, 'garp', 'garp');
#克比 kebi（普通海军）
insert into user
values (null, 'kebi', 'kebi');

select *
from user;

create table SmallForces
(
    SmallForcesName varchar(12) primary key ,
    Rewards          int,
    Numbers          int,
    ForceValue       int,
    FruitNumber      int,
    PirateStatus     varchar(12),
    Money            int
);
#drop table SmallForces;

insert into SmallForces
values ('caomaohzt', 250000000, 3, 15500, 0, '自由', 100000000);
insert into SmallForces
values ('hearthzt', 80020000, 2, 6000, 0, '自由', 6000000);
insert into SmallForces
values ('marine0', NULL, 2, 9000, 0, '自由', 4000000);
insert into SmallForces
values ('marine1', NULL, 2, 11000, 0, '自由', 5000000);

select *
from SmallForces;

# 人物表
create table CharacterTable ( # character 名字不能用！
    CharacterID      int primary key auto_increment,
    CharacterName    varchar(12),
    Age              int not null,
    IsDevilFuriter   bit, # 是否有恶魔果实
    IsDomineering    bit, # 是否有霸气
    BigForcesName    varchar(12),
    SmallForcesName varchar(12),
    ForceValue       int,
    Position         varchar(12), # 职务
    IsWanted         bit, # 是否被悬赏
    Rewards          int, # 悬赏金
    Status           varchar(12), # 自由，被抓
    foreign key (SmallForcesName) references SmallForces(SmallForcesName)
);
#drop table CharacterTable;
insert into CharacterTable
values (null, 'luffy', 22, 1, 1, 'pirate', 'caomaohzt', 6000, 'captain', 1, 100000000, '自由');
insert into CharacterTable
values (null, 'zoro', 24, 0, 1, 'pirate', 'caomaohzt', 5000, 'member', 1, 80000000, '自由');
insert into CharacterTable
values (null, 'sanji', 23, 0, 1, 'pirate', 'caomaohzt', 4500, 'cook', 1, 70000000, '自由');
insert into CharacterTable
values (null, 'law', 26, 1, 1, 'pirate', 'hearthzt', 5000, 'captain', 1, 80000000, '自由');
insert into CharacterTable
values (null, 'bepo', 15, 0, 0, 'pirate', 'hearthzt', 1000, 'member', 1, 20000, '自由');
insert into CharacterTable
values (null, 'smoker', 36, 1, 1, 'marine', 'marine0', 5000, 'captain', 0, NULL, '自由');
insert into CharacterTable
values (null, 'tashigi', 22, 0, 1, 'marine', 'marine0', 4000, 'member', 0, NULL, '自由');
insert into CharacterTable
values (null, 'garp', 60, 0, 1, 'marine', 'marine1', 8000, 'captain', 0, NULL, '自由');
insert into CharacterTable
values (null, 'kebi', 20, 0, 1, 'marine', 'marine1', 3000, 'member', 0, NULL, '自由');
insert into CharacterTable
values (null, 'zhanguo', 60, 1, 1, 'government', NULL, 7000, 'governmenter', 0, NULL, '自由');

select *
from CharacterTable;


create view CaoMember as
select * from CharacterTable where SmallForcesName = 'caomaohzt';

create view HeartMember as
select * from CharacterTable where SmallForcesName = 'hearthzt';

create view Marine0Member as
select * from CharacterTable where SmallForcesName = 'marine0';

create view Marine1Member as
select * from CharacterTable where SmallForcesName = 'marine1';

create table Island
(
    IslandName       varchar(12) primary key,
    IslandRank       int,
    SmallForcesName varchar(12) not null
);
#drop table Island;

insert into Island
values ('fish', 6, 'caomaohzt');
insert into Island
values ('iron', 5, 'hearthzt');
insert into Island
values ('xiangbodi', 9, 'marine0');
insert into Island
values ('cake', 9, 'marine1');

create table PrivateAffair
(
    AffairID          int primary key auto_increment,
    SmallForcesName   varchar(12),
    CharacterName       varchar(12),
    AffairDescription varchar(20),
    AffairStatus      varchar(12),
    foreign key (SmallForcesName) references SmallForces(SmallForcesName)
);

create table GovAffair
(
    AffairID          int primary key auto_increment,
    SmallForcesName  varchar(12),
    IslandName        varchar(9),
    AffairStatus      varchar(12),
    AffairDescription varchar(20),
    foreign key (SmallForcesName) references SmallForces(Smallforcesname)
);
#drop table GovAffair;

create table SkyEye
(
    AffairID          varchar(9),
    Sponsor       varchar(9), # 发起人
    Sufferer            varchar(9), # 承受者
    IslandName        varchar(9),
    AffairDescription varchar(9),
    foreign key (Sponsor) references SmallForces(Smallforcesname),
    foreign key (Sufferer) references SmallForces(Smallforcesname),
    foreign key (IslandName) references Island(IslandName)
);
#drop table SkyEye;

create view Rewards as
    select CharacterName, SmallForcesName, Rewards, CharacterID
    from CharacterTable
    where IsWanted = 1;

create procedure CHECK_Members(user varchar(12))
begin
    declare sForce varchar(12);
    select SmallForcesName into sForce from CharacterTable where CharacterName = user;
    select * from CharacterTable where SmallForcesName = sForce;
end;

create procedure CHECK_SmallForces(user varchar(12))
begin
    declare sForce varchar(12);
    select SmallForcesName into sForce from CharacterTable where CharacterName = user;
    select * from SmallForces where SmallForcesName = sForce;
end;

create procedure CHECK_PrivateAffair(user varchar(12))
begin
    declare sForce varchar(12);
    select SmallForcesName into sForce from CharacterTable where CharacterName = user;
    select * from PrivateAffair where SmallForcesName = sForce;
end;

select * from GovAffair;
select * from SkyEye;



# 保证输入的值正确！！！
# 抢劫
create function ACTION_PirateRob(user varchar(12), Name varchar(12))
returns int
begin
    declare flag int;
    declare sForce varchar(12);
    declare do_force int;
    declare to_force int;
    declare tem_money int;
    declare tem_fruit int;
    declare do_num int;
    declare to_num int;
    select SmallForcesName into sForce from CharacterTable where CharacterName = user;
    select ForceValue into do_force from SmallForces where SmallForcesName = sForce;
    select ForceValue into to_force from SmallForces where SmallForcesName = Name;
    #if to_force = null then
    #    return 2;
    #end if;

    if do_force >= to_force then # 抢劫成功, 恶魔果实数，金钱，武力值变
        set flag = 1;
        select FruitNumber into tem_fruit from SmallForces where SmallForcesName = Name;
        select Money into tem_money from SmallForces where SmallForcesName = Name;
        select Numbers into do_num from SmallForces where SmallForcesName = sForce;
        select Numbers into to_num from SmallForces where SmallForcesName = Name;

        update SmallForces set FruitNumber = FruitNumber + tem_fruit where SmallForcesName = sForce;
        update SmallForces set Money = Money + tem_money where SmallForcesName = sForce;
        update SmallForces set ForceValue = ForceValue + do_num*100  where SmallForcesName = sForce;
        update CharacterTable set ForceValue = ForceValue + 100 where SmallForcesName = sForce;

        update SmallForces set Money = 0 where SmallForcesName = Name;
        update SmallForces set FruitNumber = 0 where SmallForcesName = Name;
        update SmallForces set ForceValue = ForceValue - to_num*100 where SmallForcesName = Name;
        update CharacterTable set ForceValue = ForceValue - 100 where SmallForcesName = Name;

        insert into SkyEye value (null, sForce, Name, null, '被抢劫');
    else
        set flag = 0;
    end if;

    return flag;
end;

# 占领
create function ACTION_PirateOccupy(user varchar(12), Name varchar(12))
returns int
begin
    declare flag int;
    declare sForce varchar(12);
    declare IsForce varchar(12);
    declare do_force int;
    declare to_force int;
    declare do_num int;
    declare tem_rank int;
    select SmallForcesName into sForce from CharacterTable where CharacterName = user;
    select ForceValue into do_force from SmallForces where SmallForcesName = sForce;
    select IslandRank into tem_rank from Island where IslandName = Name;
    select SmallForcesName into IsForce from Island where IslandName = Name;
    select ForceValue into to_force from SmallForces where SmallForcesName = IsForce;

    if (sForce <> IsForce) && (do_force >= to_force) then # 不是已占领岛
        set flag = 1;
        select Numbers into do_num from SmallForces where SmallForcesName = sForce;

        update SmallForces set FruitNumber = FruitNumber + tem_rank where SmallForcesName = sForce;
        update SmallForces set Money = Money + tem_rank*100000 where SmallForcesName = sForce;
        update SmallForces set ForceValue = ForceValue + do_num*100  where SmallForcesName = sForce;
        update CharacterTable set ForceValue = ForceValue + 100 where SmallForcesName = sForce;

        update Island set SmallForcesName = sForce where IslandName = Name;
        insert into SkyEye value (null, sForce, IsForce, Name, '岛屿被占领');
    else
        set flag = 0;
    end if;
    return flag;
end;

# 抓捕
create function ACTION_MarineArrest(user varchar(12), Name varchar(12))
returns int
begin
    declare flag int;
    declare sForce varchar(12);
    declare do_force int;
    declare to_force int;
    declare tem_money int;
    declare tem_fruit int;
    declare do_num int;
    declare to_num int;
    select SmallForcesName into sForce from CharacterTable where CharacterName = user;
    select ForceValue into do_force from SmallForces where SmallForcesName = sForce;
    select ForceValue into to_force from SmallForces where SmallForcesName = Name;

    if do_force >= to_force then
        set flag = 1;
        select FruitNumber into tem_fruit from SmallForces where SmallForcesName = Name;
        select Money into tem_money from SmallForces where SmallForcesName = Name;
        select Numbers into do_num from SmallForces where SmallForcesName = sForce;
        select Numbers into to_num from SmallForces where SmallForcesName = Name;

        update SmallForces set FruitNumber = FruitNumber + tem_fruit where SmallForcesName = sForce;
        update SmallForces set Money = Money + tem_money where SmallForcesName = sForce;
        update SmallForces set ForceValue = ForceValue + do_num*100  where SmallForcesName = sForce;
        update CharacterTable set ForceValue = ForceValue + 100 where SmallForcesName = sForce;

        update SmallForces set Money = 0 where SmallForcesName = Name;
        update SmallForces set FruitNumber = 0 where SmallForcesName = Name;
        update SmallForces set ForceValue = ForceValue - to_num*100 where SmallForcesName = Name;
        update SmallForces set PirateStatus = '被抓' where SmallForcesName = Name;
        update CharacterTable set ForceValue = ForceValue - 100 where SmallForcesName = Name;
        insert into SkyEye value (null, sForce, Name, null, '抓捕');
    else
        set flag = 0;
    end if;

    return flag;
end;

#收复
create function ACTION_MarineRecapture(user varchar(12),Name varchar(12))
returns int
begin
    declare flag int;
    declare sForce varchar(12);
    declare IsForce varchar(12);
    declare do_force int;
    declare to_force int;
    declare do_num int;
    declare tem_rank int;
    select SmallForcesName into sForce from CharacterTable where CharacterName = user;
    select ForceValue into do_force from SmallForces where SmallForcesName = sForce;
    select SmallForcesName into IsForce from Island where IslandName = Name;
    select IslandRank into tem_rank from Island where IslandName = Name;
    select ForceValue into to_force from SmallForces where SmallForcesName = IsForce;

    if (sForce <> IsForce)  && (do_force >= to_force) then # 不是已占领岛
        set flag = 1;
        select Numbers into do_num from SmallForces where SmallForcesName = sForce;

        update SmallForces set FruitNumber = FruitNumber + tem_rank where SmallForcesName = sForce;
        update SmallForces set Money = Money + tem_rank*100000 where SmallForcesName = sForce;
        update SmallForces set ForceValue = ForceValue + do_num*100  where SmallForcesName = sForce;
        update CharacterTable set ForceValue = ForceValue + 100 where SmallForcesName = sForce;

        update Island set SmallForcesName = sForce where IslandName = Name;
        insert into SkyEye value (null, sForce, IsForce, Name, '岛屿被收复');
    else
        set flag = 0;
    end if;
    return flag;
end;
#drop function ACTION_MarineRecapture;
#select ACTION_MarineRecapture('garp', 'iron');


# 分配恶魔果实
create function ACTION_DistributionDevilFruit(do_user varchar(12), to_user varchar(12))
returns int
begin
    declare flag int;
    declare isde int;
    declare num_fruit int;
    declare sForce varchar(12);
    select IsDevilFuriter into isde from CharacterTable where CharacterName = to_user;
    select SmallForcesName into sForce from CharacterTable where CharacterName = do_user;
    select FruitNumber into num_fruit from SmallForces where SmallForcesName = sForce;
    if isde <> 1 then
        if num_fruit <= 0 then
            set flag = 2;
        else
            set flag = 1;
            update CharacterTable set IsDevilFuriter = 1 where CharacterName = to_user;
            update SmallForces set FruitNumber = FruitNumber-1 where SmallForcesName = sForce;
        end if;
    else
        set flag = 0;
    end if;
    return flag;
end;

# 发起事务
create procedure Request(user varchar(12), info varchar(20))
begin
    declare sForce varchar(12);
    select SmallForcesName into sForce from CharacterTable where CharacterName = user;
    insert into PrivateAffair value(null, sForce, user, info, '待办');
end;

# 处理事务
create procedure Deal(rid int, info varchar(20))
begin
    if info = 'delete' then
        delete from PrivateAffair where AffairID = rid;
        else
        update PrivateAffair set AffairStatus = info where AffairID = rid;
    end if;
end;

create procedure Grequest(sForce varchar(12), IsName varchar(12), info varchar(20))
begin
    insert into GovAffair value(null, sForce, IsName, '待办', info);
end;

create procedure GDeal(rid int, info varchar(20))
begin
    if info = 'delete' then
        delete from GovAffair where AffairID = rid;
    else
        update GovAffair set AffairStatus = info where AffairID = rid;
    end if;
end;

create procedure GiveMoney(do_force varchar(12), to_force varchar(12))
begin
    declare offer int;
    select Rewards into offer from SmallForces where SmallForcesName = to_force;
    update SmallForces set Money = Money + offer where SmallForcesName = do_force;
end;
drop procedure GiveMoney;

# 悬赏
create procedure DoReward(userid int, new int)
begin
    declare old int;
    declare sForce varchar(12);
    select SmallForcesName into sForce from CharacterTable where CharacterID = userid;
    select Rewards into old from CharacterTable where CharacterID = userid;

    update CharacterTable set Rewards = new where CharacterID = userid;
    update CharacterTable set IsWanted = true where CharacterID = userid;
    update SmallForces set Rewards = Rewards - old + new where SmallForcesName = sForce;
end;

create procedure UnRewardId(userid int)
begin
    declare old int;
    declare sForce varchar(12);
    select SmallForcesName into sForce from CharacterTable where CharacterID = userid;
    select Rewards into old from CharacterTable where CharacterID = userid;

    update CharacterTable set Rewards = null where CharacterID = userid;
    update CharacterTable set IsWanted = false where CharacterID = userid;
    update SmallForces set Rewards = Rewards - old where SmallForcesName = sForce;
end;

create procedure UnRewardForce(sForce varchar(12))
begin
    update CharacterTable set Rewards = null where SmallForcesName = sForce;
    update CharacterTable set IsWanted = false where SmallForcesName = sForce;
    update SmallForces set Rewards = 0 where SmallForcesName = sForce;
end;

# 商店
create function BuyFruit(user varchar(12))
returns int
begin
    declare sForce varchar(12);
    declare money int;
    declare flag int;
    select SmallForcesName into sForce from CharacterTable where CharacterName = user;
    select Money into money from SmallForces where SmallForcesName = sForce;

    if money >= 100000000 then
        set flag = 1;
        update SmallForces set Money = Money - 100000000 where SmallForcesName = sForce;
        update SmallForces set FruitNumber = FruitNumber = 1 where SmallForcesName = sForce;
    else
        set flag = 0;
    end if;
    return flag;
end;

create function BuyRewards(user varchar(12), userid int)
returns int
begin
    declare sForce varchar(12);
    declare money int;
    declare offer int;
    declare flag int;
    select SmallForcesName into sForce from CharacterTable where CharacterName = user;
    select Money into money from SmallForces where SmallForcesName = sForce;
    select Rewards into offer from CharacterTable where CharacterID = userid;

    if money >= 3*offer then
        set flag = 1;
        update SmallForces set Money = Money - 3*offer where SmallForcesName = sForce;
        update CharacterTable set Rewards = null where CharacterID = userid;
        update CharacterTable set IsWanted = false where CharacterID = userid;
        update SmallForces set Rewards = Rewards - offer where SmallForcesName = sForce;
    else
        set flag = 0;
    end if;
    return flag;
end;

# 未被抓？
create function BuyForce(user varchar(12), to_force varchar(12))
returns int
begin
    declare sForce varchar(12);
    declare money int;
    declare offer int;
    declare flag int;
    select SmallForcesName into sForce from CharacterTable where CharacterName = user;
    select Money into money from SmallForces where SmallForcesName = sForce;
    select Rewards into offer from SmallForces where SmallForcesName = to_force;

    if money >= 10*offer then
        set flag = 1;
        update SmallForces set Money = Money - 10*offer where SmallForcesName = sForce;
        update SmallForces set PirateStatus = '自由' where SmallForcesName = to_force;
    else
        set flag = 0;
    end if;
    return flag;
end;
#drop function ByForce;
#drop function ByFruit;
#drop function ByRewards;

# 事物插入天眼表
#create trigger IN_SkyEye after update on Island
#    for each row
#    begin
#        insert into SkyEye value (null, NEW.SmallForcesName, null, NEW.IslandName, '岛屿被占领');
#    end

# 插入新成员的触发器 IN_Big_Forces

create procedure joinUser(user varchar(12), sForce varchar(12), userAge int, tem_fruit int, tem_domineer int, userForce int, password varchar(12), flag int, userPosition varchar(12))
begin
    declare bigForce varchar(12);
    declare fruit bit;
    declare domineer bit;
    if tem_fruit = 1 then
        set fruit = 1;
        else
            set fruit = 0;
    end if;
    if tem_domineer = 1 then
        set domineer = 1;
    else
        set domineer = 0;
    end if;
    if flag = 1 then
        set bigForce = 'pirate';
    end if;
    if flag = 0 then
        set bigForce = 'marine';
    end if;
    if flag = 2 then
        set bigForce = 'government';
    end if;
    insert into user value (null, user, password);
    if userPosition = 'captain' then
        insert into SmallForces value (sForce, 0, 1, userForce,  0, '自由', 0);
        insert into CharacterTable value (null, user, userAge, fruit, domineer, bigForce, sForce, userForce, userPosition, 0, 0, '自由');
    end if;
    if userPosition = 'member' then
        insert into CharacterTable value (null, user, userAge, fruit, domineer, bigForce, null, userForce, userPosition, 0, 0, '自由');
        insert into PrivateAffair value(null, sForce, user, '请求加入', '待办');
    end if;
    if userPosition = 'governmenter' then
        insert into CharacterTable value (null, user, userAge, fruit, domineer, bigForce, null, userForce, userPosition, 0, 0, '自由');
    end if;
end;

#drop procedure joinUser;
create procedure addName(rid int)
begin
    declare sForce varchar(12);
    declare add_user varchar(12);
    declare forceValue int;
    select SmallForcesName into sForce from PrivateAffair where AffairID = rid;
    select CharacterName into add_user from PrivateAffair where AffairID = rid;
    select ForceValue into forceValue from CharacterTable where CharacterName = add_user;

    update CharacterTable set SmallForcesName = sForce where CharacterName = add_user;
    update SmallForces set ForceValue = ForceValue + ForceValue where SmallForcesName = sForce;
    update SmallForces set numbers = numbers + 1 where SmallForcesName = sForce;
end;

