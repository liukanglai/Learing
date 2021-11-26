# /etc/fstab

- 

# <= 2T

- fdisk -l
- cfdisk
- lsblk

# 

- parted /dev/...

    mklabel gpt
    unit TB
    mkpart primary 0 3 // start-end
    print
    quit


# df

- look up 

- 另外,如果用户需要取消磁盘挂载超时时间,可以直接在“/etc/fstab”文件中添加“x-systemd.device-timeout=0”。如下:

        #
        # /etc/fstab
        # Created by anaconda on Mon Sep 14 17:25:48 2015
        #
        # Accessible filesystems, by reference, are maintained under '/dev/disk'
        # See man pages fstab(5), findfs(8), mount(8) and/or blkid(8) for more info
        #
        /dev/mapper/euleros-root / ext4 defaults,x-systemd.device-timeout=0 0 0
        UUID=afcc811f-4b20-42fc-9d31-7307a8cfe0df /boot ext4 defaults,x-systemd.device-timeout=0 0 0
        /dev/mapper/euleros-home /home ext4 defaults 0 0
        /dev/mapper/euleros-swap swap swap defaults 0 0
