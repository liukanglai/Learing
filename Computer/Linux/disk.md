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
