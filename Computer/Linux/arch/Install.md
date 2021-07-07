# 安装

## 启动

* 镜像u盘 (MD5… PGP…)

> 例:dd bs=4M if=/home/liukanglai/Downloads/archlinux-2020.04.01-x86_64.iso of=/dev/sda status=progress oflag=sync

* u盘启动 UEFI `root@archiso~#`

# 查看指南:ls 

* 验证启动模式:ls /sys/firmware/efi/efivars 有输出即为UEFI

## 联网

* ip link –看网卡  `enp开头有线网卡 wlp无线网卡` 

    > 看有无网: 命令 ping www.baidu.com 后显 ….ms 即有网
    >
    > enp开头有线网卡 wlp无线网卡 


(手机usb共享网,连接电脑)

### 无线

* ip link set .. up

- 扫描：iwlist .. scan | grep ESSID
- 连接：

    > wpa_passphrase 网络名 密码 > internet.conf
    > 
    > wpa_supplicant -c internet.conf -i 设备名 &

- 分配ip地址：dhcpcd &

- 如需web登录(Captive Portal): elinks http…

### 有线

- systemctl enable dhcpcd
- systemctl restart dhcpcd

## 配置

- 时间同步: timedatectl set-ntp true

    > 手动: timectl set-time “year-month-date h-minute-s”

- 检查:timedatectl status

## 安盘

- 查看硬盘: lsblk或fdisk -l

    > /dev/… nvme0n(高级)或sda… (新盘不同)

- 分盘: cfdisk 进gpt 分后yes

格式|大小|目录|格式
----|----|----|----
fat32|300M|EFI|EFI system
ext4| |/|linux filesystem
swap | |/|linux swap

- 格式化: 

    > /：mkfs.ext4 /dev/..
    >
    > EFI: mkfs.vfat或mkfs.fat -F32
    > 
    > swap: mkswap -f /…

- 打开swap：swapon /…
 
- 挂载:
 
    > /: mount /dev/..  /mnt
    > 
    > mkdir /mnt/boot
    > 
    > EFI: mount /dev/..  /mnt/boot
 
## 软件源

- sudo pacman-mirrors -c China
- vim /etc/pacman.d/mirrorlist 把China移到最前

```
 清华大学
Server = https://mirrors.tuna.tsinghua.edu.cn/archlinux/$repo/os/$arch
## 163
Server = http://mirrors.163.com/archlinux/$repo/os/$arch
## aliyun
Server = http://mirrors.aliyun.com/archlinux/$repo/os/$arch
```
 
- 刷新: pacman -syy (-syu)
 
## 安装

- pacstrap /mnt base linux linux-firmware vim  base-devel man 

> linux中包含EFI文件！！！
 
## 进入chroot

- 生成fstab???
 
    > genfstab -L /mnt >> /mnt/etc/fstab
 
- 检查:cat /mnt/etc/fstab
 
- arch-chroot /mnt

## 基本设置

- 时区: ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime
- 设置时间标准UTC,并调整时间漂移 : hwclock –-systohc
- 本地化: 

    > vim /etc/locale.gen (去#号 en_US.UTF-8 UTF-8   zh_CN.UTF-8 UTF-8    zh_HK.UTF-8 UTF-8   zh_TW.UTF-8 UTF-8 )
    >
    > locale-gen(生成)
    >

- vim /etc/locale.conf 输入： LANG=en_US.UTF-8
- vim /etc/hostname 编辑主机名称hostname
- vim /etc/hosts
 
- 文末加
- 127.0.0.1 localhost
- ::1 localhost
- 127.0.1.1 hostname.localdomain hostname
- 设密码:passwd

## 网络

pacman -S networkmanager iw wpa_supplicant dialog dhcpcd netctl 

## 安装启动加载器

- pacman -S efibootmgr  grub os-prober intel-ucode ntfs-3g
- grub-install -–target=x86_64-efi -–efi-directory=/boot –-bootloader-id=grub 
- 生成:grub-mkconfig -o /boot/grub/grub.cfg
 
## 重启:exit

- killall wpa_supplicant dhcpcd
- umount  /mnt/root
- umount  /mnt
- reboot
 
##
 
有线： dhcpcd
 
- wifi-menu 即可
- systemctl enable dhcpcd
- dhcpcd &
`连网可将iwlist换为iw` 
 
## 用户

- pacman -S sudo
- useradd -m -g users -G wheel -s /bin/bash …(name)
- passwd ..(name)
- 修改sudo权限:vim /etc/sudoers or (ln -s /usr/bin/vim /usr/bin/vi  visudo)

`Uncomment to allow members of group wheel to execute any command
 去%wheel前#`
 
## 源 

- vim /etc/pacman.conf 文末：

    > [multilib]去#

    > /#[custom]变：

    > [archlinuxcn]
- 只能加一个

```
[archlinuxcn]
# The Chinese Arch Linux communities packages.

# SigLevel = Optional TrustAll
SigLevel = Optional TrustedOnly
Include = /etc/pacman.d/archlinuxcn(need created)

vim /etc/pacman.d/archlinuxcn

input:
# 官方源
Server   = http://repo.archlinuxcn.org/$arch
# 163源
Server = http://mirrors.163.com/archlinux-cn/$arch
# 清华大学
Server = https://mirrors.tuna.tsinghua.edu.cn/archlinuxcn/$arch
```

- 安装archlinuxcn-keyring
- pacman -Syu

## 字体:
 
- 思源字体:
- adobe-source-han-serif-cn-fonts - 思源宋体简体中文部分
- adobe-source-han-serif-tw-fonts - 思源宋体繁体中文部分
- adobe-source-han-sans-cn-fonts - 思源黑体简体中文部分
- adobe-source-han-sans-tw-fonts - 思源黑体繁体中文部分
- ttf-dejavu ttf-liberation wqy-microhei ttf-wps-fonts
- JetBrains Mono!!!
 
## 目录:

- 安装xdg-user-dirs
- 执行xdg-user-dirs-update

## 显卡

- 查看：lspci | grep VGA
- 英特尔：xf86-video-intel
- amd: xf86-video-ati
- 英伟达：nvidia nvidia-settings（非必要不装）

 [更多](https://wiki.archlinux.org/index.php/Xorg#Driver_installation) 

## 声卡

- alsa-utils


## 桌面

- 安装:
 
- xorg xorg-server xorg-xinit
- sddm sddm-kcm
- plasma packagekit-qt5
- 可选：kde-applications (kde-base)

> if you haven't install app,then a terminal needed(yakuake)
 
- 网络工具:net-tools plasma-nm network-manager-applet
- systemctl disable netctl
- systemctl enable NetworkManager
- systemctl enable sddm
- systemctl start sddm
 
