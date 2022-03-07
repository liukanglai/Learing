ranger

- code clion idea qtcreater
- vnote typora suziwen/markdownxiaoshujiang https://github.com/marktext/marktext
  (gimp)

  pdf 可直接用浏览器打开，也可选用其他专用的阅读软件，如 okular 或者 calibre。okular 在打开大型 epub 时会非常卡顿,并且图片模糊不清，Foliate 是阅读 epub 的更佳选择。

- https://github.com/qier222/YesPlayMusic
- yay -S yesplaymusic

# rime

- ~/.local/share/fcitx5/rime/build/default.yaml: put simple first(in schema_list)
- use new file(no deffect default.yaml), in build fold, create default.custom.yaml:

- yay -S rime-cloverpinyin (schema_list: - schema: clover)

> https://github.com/fkxxyz/rime-cloverpinyin/wiki/linux

- yay -S ttf-apple-emoji

# 蓝牙耳机

- bluez bluez-utils
- lsmod (check whether btusb is loaded, if not, then modprobe btusb)
- systemctl start bluetooth.service
- bluedevil:kde

```
bluez软件包提供蓝牙协议栈
bluez-utils软件包提供bluetoothctl工具
pulseaudio-bluetooth则为bluez提供了PulseAudio音频服务,若没有安装则蓝牙设备在配对完成后,连接会失败,提示
pavucontrol则提供了pulseaudio的图形化控制界面
pulseaudio-alsa(可选)则使pulseaudio和alsa协同使用，之后就可以用alsamixer来管理蓝牙音频
```

- 修改/etc/bluetooth/main.conf 底部的 [Policy]

```
/etc/bluetooth/main.conf
[Policy]
AutoEnable=true
```

- systemctl enable bluetooth
- systemctl start bluetooth
- pulseaudio -k # 确保没有 pulseaudio 启动
- pulseaudio --start # 启动 pulseaudio 服务

# 软件

## professional

sudo pacman -S libreoffice-still #稳定版
sudo pacman -S libreoffice-fresh #尝鲜版
yay -S onlyoffice-bin

- install deepin-wine firstly.

  > deepin-wine-qq
  >
  > deepin-wine-wechat
  >
  > deepin-wine-tim

> 办公软件 WPS 安装软件和缺失字体：
>
> sudo pacman -S wps-office ttf-wps-fonts
>
> 如果你下载了国际版本缺失中文：
>
> sudo pacman -S wps-office-mui-zh-cn

- baidunetdisk-electron (yay)
- tlp - 电池
- google-chrome/chromium (Chromium 的用户资料在~/.config/chromium/Default)
- qtcreator ark cmake ctags electron
- flameshot cheese vlc gwenview gimp
- okular
- yakuake ranger
- ark
- woeusb
- neofetch
- openssh 远程连接工具
- deepin-screenshot Flameshot 现代、快捷、轻便的截图工具
- SimpleScreenRecorder 轻量的录屏软件
- kdenlive shotcut 强大的视频剪辑软件
- sublime-text-dev 代码编辑器
- proxychains-ng 终端内科学上网代理工具
- redshift 显示屏色温调节工具
- telegram-desktop 客户端开源的加密聊天工具
- liferea RSS 阅读器
- qbittorrent 好用的 BT 下载工具
- calibre 电子书转换、编辑、阅读工具
- gthumb 图片浏览工具,可简单编辑图片,可清除照片元数据
- libreoffice-fresh 必备的办公软件
- peek 录制 GIF 动图
- inkscape 强大的矢量图形编辑软件
- fontforge 字体设计、编辑软件
- aria2 强大的多线程下载工具
- youtube-dl YouTube 视频下载工具
- AppImageLauncher .appimage 文件的启动器

cool tips:

- neofetch
- sl(steam locomotive)
- cmatrix
- aalib (aafire -driver curses)
- fortune-mod
- yes lufei(c-r)

# 系统时间

1. 24 小时：

   - qt5-base-24h

2. windows 修改 Windows 硬件时钟为 UTC 时间

   - 以管理员身份打开 「PowerShell」，输入以下命令：

     Reg add HKLM\SYSTEM\CurrentControlSet\Control\TimeZoneInformation /v RealTimeIsUniversal /t REG_DWORD /d 1
     或者打开「注册表编辑器」，定位到 计算机\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\TimeZoneInformation 目录下，新建一个 DWORD 类型，名称为 RealTimeIsUniversal 的键，并修改键值为 1 即可。

- sudo pacman -S ntpdate
- sudo ntpdate time.windows.com
- sudo hwclock --localtime --systohc
- hwclock --show
- hwclock --systohc

# deb 安装

- yay -S debtap
- 也应该安装 bash， binutils ，pkgfile 和 fakeroot 依赖包。
- 创建/更新 pkgfile 和 debtap 数据库。
- sudo debtap -u
- 转换 deb 包 debtap (-q / -Q: no question) \*.deb
- 安装 sudo pacman -U <package-name>

        # /usr/bin/debtap
        替换：http://ftp.debian.org/debian/dists
        为：https://mirrors.ustc.edu.cn/debian/dists

        替换：http://archive.ubuntu.com/ubuntu/dists
        为：https://mirrors.ustc.edu.cn/ubuntu/dists/

# display

- vim /etc/sddm.conf.d/dpi.conf: `[X11] ServerArguments=-dpi 96`

# dolphin

yay -S octopi #包管理器前端界面

- https://wiki.archlinux.org/title/Dolphin#File_previews
- kdegraphics-thumbnailers

# pdf

- poppler
- poppler-data

# ulauncher

# utools

# stacer

# vim

- prettier

# vmbox

- // sudo pacman -S linux-headers

- virtualbox virtualbox-ext-vnc virtualbox-guest-iso virtualbox-host-modules-arch
- 再去官网下载 Oracle VM VirtualBox Extension Pack ，在设置中导入使用
- sudo restart systemd-modules-load.service

1. share fold

- sudo mount -t vboxsf share /mnt/kylin

2. 全屏

- 选择不同分辨率

# java

- sudo pacman -S jre-openjdk
- sudo pacman -S jdk-openjdk

# hibernate

- cat /sys/power/image_size: swap size
- sudo filefrag -v /swapfile | sed -n '4p' | awk '{print $4+0}' : get resume_offset
- sudo vim /boot/leader/entries/arch.conf: options root=UUID=SOMEUUID rw resume=/dev/sda1 resume_offset=12345
- sudo vim /etc/mkinitcpio.conf: HOOKS=( base udev **resume** autodetect modconf block filesystems keyboard fsck ) -- add resume
- sudo mkinitcpio -p linux

# space

- mkdir /home/space
- mount /dev/... /home/space
- vim /etc/fstab: `/dev/... /home/space ext4 defaults 1 1`

# Easyconnect

- `https://www.wannaexpresso.com/2020/06/07/easy-connect-manjaro/`

- 错：您的客户端版本与服务器不匹配，请下载更新
- 由于最新版与服务器要求版本不一致，所以需要修改源码包中的 PKGBUILD 文件，然后重新手动 makepkg 生成服务器要求版本然后再安装。

1.  修改 PKGBUILD 文件

        source=("http://download.sangfor.com.cn/download/product/sslvpn/pkg/linux_767/EasyConnect_x64_7_6_7_3.deb"
              "http://ftp.acc.umu.se/pub/GNOME/sources/pango/1.42/pango-1.42.4.tar.xz")
        md5sums=('ac2020ce44583d5ee4552c81563dce9c'
              'deb171a31a3ad76342d5195a1b5bbc7c')

- 修改为

        source=("http://download.sangfor.com.cn/download/product/sslvpn/pkg/linux_01/EasyConnect_x64.deb"
              "http://ftp.acc.umu.se/pub/GNOME/sources/pango/1.42/pango-1.42.4.tar.xz")
        md5sums=('6ed6273f7754454f19835a456ee263e3'
              'deb171a31a3ad76342d5195a1b5bbc7c')

# 打印机

对于日常办公来说，打印机是非常必要的。除此之外，我们建议读者维持一份纸质的密码，包括你可以将你虚拟货币钱包中的私钥打印出来保存，这是非常好的一个方案。对于打印机的品牌，我们建议使用惠普打印机。其对于 Linux 的支持非常到位，可以去其网站查看所支持的设备等详情。在 Arch Linux 上，安装包 hplip 即可。

sudo pacman -S hplip

# worng

一般屏幕会出现形如 A stop job is running for...(1m30s)的信息，这是经常会遇到的关机卡住 1 分 30 秒的问题，一般来说这种情况是出现了某个进程在关机时不愿停止，需要等到超时时间到达强行停止。通用的解决办法是调整缩短这个等待时间，建议从 1 分 30 秒调整至 30 秒，30 秒已经足够几乎所有进程正常结束。

编辑 /etc/systemd/system.conf

sudo vim /etc/systemd/system.conf
找到其中 DefaultTimeoutStopSec 一项，将其前方的井号去掉，并赋值为 30s 即可。最后执行 daemon-reload 使其生效。

sudo systemctl daemon-reload
上述解决方案其实只是将这个等待时间缩小了，并没有解决实际问题。如果你想排查问题真正的原因所在，在关机时如果出现了 A stop job is running for...(1m30s)的信息，耐心等待其结束关机，然后重新启动电脑，执行以下命令：

journalctl -p5
按/(斜杠键)搜索 Killing 关键字，找到你关机的时间附近所在的匹配行，你可以在附近看到到底是哪一个进程导致了 timeout,然后再去排查这个进程有什么问题即可。

升级系统时出现形如 unable to lock database 的错误
可能存在升级系统时异常关机或程序异常退出的情况，或者多个 pacman 的相关程序在同时执行。移除 pacman 的 db 锁即可

sudo rm /var/lib/pacman/db.lck
