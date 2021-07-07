#
- sudo gunzip ***.gz
- chmod +x clash***
- ./clash -d .

- wget (-P /opt/clash/) -O config.yaml [你的订阅链接]
- config.yaml

# arch

- pacman -S clash / or clash-premium-bin
- run clash
- ~/.config/clash: yaml

- delete: /usr/lib/systemd/system/clash@.service and  /usr/lib/systemd/user/clash@.service

# autostart:https://github.com/Dreamacro/clash/wiki/clash-as-a-daemon

- /etc/systemd/system/clash.service

        [Unit]
        Description=Clash daemon, A rule-based proxy in Go.
        After=network.target
        
        [Service]
        Type=simple
        Restart=always
        Restart=on-failure
        ExecStart=/usr/local/bin/clash -d /etc/clash // need change
        
        [Install]
        WantedBy=multi-user.target

- Launch clashd on system startup with:
$ systemctl enable clash
- Launch clashd immediately with:
$ systemctl start clash
- Check the health and logs of Clash with:
$ systemctl status clash
$ journalctl -xe

# update subscription
[root@localhost ~]# crontab -e
填入以下内容
29 6    * * *   root    pgrep clash | xargs kill -s 9
30 6    * * *   root    mv /opt/clash/config.yaml /opt/clash/configbackup.yaml
31 6    * * *   root    wget -P /opt/clash/ -O config.yaml [你的订阅链接]
32 6    * * *   root    nohup /opt/clash/clash -d /opt/clash/
按Esc和:wq保存退出
重启crontab，使配置生效
[root@localhost ~]# systemctl restart crond.service

# cotroller

取消注释 external-controller、external-ui 和 secret，并配置 secret 作为访问 dashboard 的口令。

在终端中通过 clash 命令启动 Clash。如果配置了 dashboard，可以在局域网内的其他设备上开启浏览器，访问 http://10.0.1.11:6300/ui/，其中 10.0.1.11 即此前配置的 Pi 的 IP 地址，端口 6300 即 Clash 监听的外部控制器端口。然后输入如下信息：

Host 为 10.0.1.11，即 Pi 的 IP 地址。
端口为 6300，即 external-controller: 0.0.0.0:6300 所配置的端口。
密钥即 secret 所配置的口令，上述示例中为 your-secret-passphrase。

# shellclash

- source ~/.bashrc &> /dev/null

启动脚本：clash

启动clash服务：$clashdir/start.sh start

停止clash服务：$clashdir/start.sh stop

重启clash服务：$clashdir/start.sh restart

更新订阅文件：$clashdir/start.sh getyaml


