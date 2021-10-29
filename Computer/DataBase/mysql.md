# install 

1. mariadb
  - mariadb-install-db --user=mysql --basedir=/usr --datadir=/var/lib/mysql
  - systemctl start mariadb.service

> etc/my.cnf.d/mysql-clients.cnf, and add auto-rehash under mysql

# run

- mysql -u name -p

                FLUSH PRIVILEGES;
        quit


