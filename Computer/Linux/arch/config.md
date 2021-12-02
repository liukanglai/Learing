- git libconfig wget curl npm exfat-utils p7zip unzip zip unrar ranger tlp ark neofetch bash-completion
- vim nvim
- google-chrome ark cmake ctags flameshot cheese vlc gwenview gimp okular yakuake
- deepin-screenshot
- code clion idea qtcreater
- vnote typora suziwen/markdownxiaoshujiang

# yay

1.  compiler

        git clone https://aur.archlinux.org/yay.git
        cd yay
        makepkg -si

2.  pacman -S yay

- 源：yay --aururl "https://aur.tuna.tsinghua.edu.cn" --save (https://aur.archlinux.org)
- 查看：yay -P -g(位于 ~/.config/yay/config.json)
- yay -Syu

## 输入法：安装 fcitx

1. fcitx+fcitx-im+fcitx-sunpinyin(fcitx-qt5 fcitx-configtool) kcm-fcitx fcitx-qt4+fcitx-sogoupinyin(yay)

> vim /etc/profile

        export XMODIFIERS="@im=fcitx"
        export GTK_IM_MODULE="fcitx"
        export QT_IM_MODULE="fcitx"

2. fictx5-im (fictx5+fictx5-gtk/qt+fcitx5-configtool)
   - fcitx5-chinese-addons

```
sudo pacman -S fcitx5-im #基础包组
sudo pacman -S fcitx5-chinese-addons #官方中文输入引擎
sudo pacman -S fcitx5-anthy #日文输入引擎
sudo pacman -S fcitx5-pinyin-moegirl #萌娘百科词库 二刺猿必备(ArchLinuxCn)
sudo pacman -S fcitx5-pinyin-zhwiki #中文维基百科词库
sudo pacman -S fcitx5-material-color #主题
    - fcitx5-rime
    - fcitx5-nord(Setting -> Location -> input method -> Configure addons -> Classic user interface -> Theme.)
```

> Edit /etc/environment and add the following lines:

        GTK_IM_MODULE=fcitx
        QT_IM_MODULE=fcitx
        XMODIFIERS=@im=fcitx

- ~/.local/share/fcitx5/rime/build/default.yaml: put simple first(in schema_list)
- use new file(no deffect default.yaml), in build fold, create default.custom.yaml:

- yay -S rime-cloverpinyin (schema_list: - schema: clover)

> https://github.com/fkxxyz/rime-cloverpinyin/wiki/linux

- yay -S ttf-apple-emoji

- For vim: vim-fcitx (set ttimeoutlen=100)

# 触控板

- yay -S kcm-pointing-devices-git
- sudo pacman -S xf86-input-libinput
- sudo pacman -S xf86-input-synaptics #触摸板驱动#

# 蓝牙耳机

- bluez bluez-utils
- lsmod (check whether btusb is loaded, if not, then modprobe btusb)
- systemctl start bluetooth.service
  //- pulseaudio-bluetooth pavucontrol pulseaudio-alsa
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

# 代理

- cfw
- qv2ray 出错，可删除再装，（用 pacman 装，用 yay 装，）
- clashy
- wine/crossover
- darling

# Terminal

1. bash-completion
2. zsh

   - yay -S zsh
   - sudo chsh -s /bin/zsh username
   - yay -S oh-my-zsh-git
   - cp /usr/share/oh-my-zsh/zshrc ~/.zshrc
   - in .zshrc: export LC_CTYPE=en_US.UTF-8
   - powerlevel10k
   - yakuake --get theme gruvbox

3. translation

   - yay -S node-fanyi or sudo npm install fanyi -g
     > https://github.com/afc163/fanyi

4. tmux

- start with tmux@username.service

      /etc/systemd/system/tmux@.service
      [Unit]
      Description=Start tmux in detached session

      [Service]
      Type=forking
      User=%I
      ExecStart=/usr/bin/tmux new-session -s %u -d
      ExecStop=/usr/bin/tmux kill-session -t %u

      [Install]
      WantedBy=multi-user.target

5. truecolor:

- look vim/UI, tmux

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

# 美化

- yay -S ocs-url
- 设置外观 layan
- yay -S layan-kde-git
- tela purple
- author: adhe in `https://store.kde.org/find?search=gruv`(all gruvbox)
- .local/share/
- gruvbox konsole, yakuake(.config/yakuakerc)
- 窗口管理-任务切换器-主窗口可视化切换
- 安装 lattw-dock
  > 布局
- 终端,字体
- 毛玻璃 compton

  > .config/compton.conf

- widgets: cpu, memory, network, ip

# dolphin

- https://wiki.archlinux.org/title/Dolphin#File_previews
- kdegraphics-thumbnailers

# pdf

- poppler
- poppler-data

# ulauncher

# utools

# stacer

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
