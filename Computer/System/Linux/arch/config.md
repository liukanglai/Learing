
- git  libconfig wget curl npm exfat-utils p7zip unzip zip unrar ranger tlp ark neofetch bash-completion
- vim nvim
- google-chrome ark cmake ctags flameshot cheese vlc gwenview gimp okular yakuake 
- code clion idea qtcreater
- vnote typora 

# yay

1. compiler 

        git clone https://aur.archlinux.org/yay.git 
        cd yay 
        makepkg -si

2. pacman -S yay

- 源：yay --aururl "https://aur.tuna.tsinghua.edu.cn" --save (https://aur.archlinux.org)
- 查看：yay -P -g(位于 ~/.config/yay/config.json) 
- yay -Syu
 
## 输入法：安装fcitx
 
1. fcitx+fcitx-im+fcitx-sunpinyin(fcitx-qt5 fcitx-configtool) kcm-fcitx fcitx-qt4+fcitx-sogoupinyin(yay)

> vim /etc/profile
 
        export XMODIFIERS="@im=fcitx"
        export GTK_IM_MODULE="fcitx"
        export QT_IM_MODULE="fcitx" 

2. fictx5-im (fictx5+fictx5-gtk/qt+fcitx5-configtool)
    - fcitx5-chinese-addons
    - fcitx5-rime
    - fcitx5-nord(Setting -> Location -> input method -> Configure addons -> Classic user interface -> Theme.)

> Edit /etc/environment and add the following lines:

        GTK_IM_MODULE=fcitx
        QT_IM_MODULE=fcitx
        XMODIFIERS=@im=fcitx

-  ~/.local/share/fcitx5/rime/build/default.yaml: put simple first(in schema_list)
- use new file(no deffect default.yaml), in build fold, create default.custom.yaml:

- yay -S rime-cloverpinyin (schema_list: - schema: clover)

> https://github.com/fkxxyz/rime-cloverpinyin/wiki/linux

- yay -S ttf-apple-emoji

- For vim: vim-fcitx (set ttimeoutlen=100)

# 触控板 

- yay -S kcm-pointing-devices-git
- sudo pacman -S xf86-input-libinput
- sudo pacman -S xf86-input-synaptics  #触摸板驱动#
 
# 蓝牙耳机

- bluez bluez-utils  pulseaudio-bluetooth pavucontrol pulseaudio-alsa
- bluedevil:kde
``` 
bluez软件包提供蓝牙协议栈
bluez-utils软件包提供bluetoothctl工具
pulseaudio-bluetooth则为bluez提供了PulseAudio音频服务,若没有安装则蓝牙设备在配对完成后,连接会失败,提示
pavucontrol则提供了pulseaudio的图形化控制界面
pulseaudio-alsa(可选)则使pulseaudio和alsa协同使用，之后就可以用alsamixer来管理蓝牙音频
```
你只需要将 AutoEnable=true 添加在 /etc/bluetooth/main.conf 底部的 [Policy] 下面：
/etc/bluetooth/main.conf
[Policy]
AutoEnable=true

- systemctl enable bluetooth
- systemctl start bluetooth
- pulseaudio -k                   # 确保没有pulseaudio启动
- pulseaudio --start              # 启动pulseaudio服务

# 软件

- install deepin-wine firstly.

    > deepin-wine-qq
    > 
    > deepin-wine-wechat
    > 
    > deepin-wine-tim
 
> 办公软件WPS安装软件和缺失字体：
>
> sudo pacman -S wps-office ttf-wps-fonts
>
> 如果你下载了国际版本缺失中文：
>
> sudo pacman -S wps-office-mui-zh-cn

- baidunetdisk-bin (yay)
- tlp - 电池
- google-chrome/chromium  (Chromium 的用户资料在~/.config/chromium/Default)
- qtcreator  ark cmake ctags electron
- flameshot  cheese  vlc  gwenview  gimp
- okular
- yakuake  ranger
- ark
- woeusb
- neofetch 
- openssh 远程连接工具
- deepin-screenshot  Flameshot 现代、快捷、轻便的截图工具
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
- AppImageLauncher   .appimage文件的启动器 

# 系统时间

1. 24小时：
    - qt5-base-24h

2. windows 修改 Windows 硬件时钟为 UTC 时间
    - 以管理员身份打开 「PowerShell」，输入以下命令：
    > Reg add HKLM\SYSTEM\CurrentControlSet\Control\TimeZoneInformation /v RealTimeIsUniversal /t REG_DWORD /d 1
    - 或者打开「注册表编辑器」，定位到 计算机\HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\TimeZoneInformation 目录下，新建一个 DWORD 类型，名称为 RealTimeIsUniversal 的键，并修改键值为 1 即可。

- sudo pacman -S ntpdate
- sudo ntpdate time.windows.com
- sudo hwclock --localtime --systohc
- hwclock --show
- hwclock --systohc


# 代理

- cfw
- qv2ray出错，可删除再装，（用pacman装，用yay装，）
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


# deb安装

- yay -S debtap
- 也应该安装bash， binutils ，pkgfile 和 fakeroot 依赖包。
- 创建/更新 pkgfile 和 debtap 数据库。
- sudo debtap -u ..
- 转换deb包 debtap *.deb
- 安装 sudo pacman -U <package-name>

# 美化

- yay -S ocs-url
- 设置外观 layan
- yay -S layan-kde-git
- author: adhe in `https://store.kde.org/find?search=gruv`(all gruvbox)
- .local/share/
- gruvbox konsole, yakuake(.config/yakuakerc)
- 窗口管理-任务切换器-主窗口可视化切换
- 安装lattw-dock
  >布局/
- 终端,字体
- 毛玻璃compton
    > .config/compton.conf

# dolphin

- https://wiki.archlinux.org/title/Dolphin#File_previews
- kdegraphics-thumbnailers

# pdf

- poppler
- poppler-data

# vmbox

- sudo pacman -S linux-headers
- sudo pacman -S virtualbox  选择 1 virtualbox-host-dkms
- sudo pacman -S virtualbox-guest-iso
- sudo /sbin/vboxconfig setup

# java

- sudo pacman -S jre-openjdk
- sudo pacman -S jdk-openjdk
