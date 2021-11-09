#

- lsmod
- insmod
- rmmod (-f)

#

- First, find out, which other modules use the module sdhci:

        lsmod | grep sdhci
- You will get a list like this:

        module size used_by
- Try unloading these modules (used\_by) before or together with the module you want to unload:

        sudo modprobe -r <module found from lsmod> <module you want to remove>
If you want to prevent the module from loading on the next boot, add it to the blacklist:

        echo -e "sdhci\n" | sudo tee -a /etc/modprobe.d/blacklist.conf
