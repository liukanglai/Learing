# install 

1. mariadb
  - mariadb-install-db --user=mysql --basedir=/usr --datadir=/var/lib/mysql
  - systemctl start mariadb.service

> etc/my.cnf.d/mysql-clients.cnf, and add auto-rehash under mysql

# run

- mysql -u name -p

        MariaDB> CREATE USER 'name'@'localhost' IDENTIFIED BY 'password'; 

        host：指定该用户在哪个主机上可以登陆，如果是本地用户可用localhost，如果想让该用户可以从任意远程主机登陆，可以使用通配符%
        password：该用户的登陆密码，密码可以为空，如果为空则该用户可以不需要密码登陆服务器

        MariaDB> GRANT ALL PRIVILEGES ON mydb.* (*.*) TO 'monty'@'localhost';
        GRANT privileges ON databasename.tablename TO 'username'@'host'

        MariaDB> FLUSH PRIVILEGES;
        MariaDB> quit


