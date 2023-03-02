# Linux Kernel for Understanding Host Network Stack Latency

## Installing instructions

1. Copy one of config files in `/boot` to the local folder 
 ``` 
    cp /boot/config-some-kernel-version  .config
 ```

2. config the kernel and press Enter when having questions

```
  make oldconfig
```

3. Compile and build the kernel

```
  sudo make -j32 bzImage
  make -j32 modules
  make modules_install
  make install;
```

4. Change the default kernel version:

```
   sudo vim /etc/default/grub
```
Comment out the current `GRUB_DEFAULT` line and add a new line:
```
   GRUB_DEFAULT="1>Ubuntu, with Linux 5.10.46+"
```

6. Update the grub menu and then reboot the servers:

```
sudo update-grub2
sudo reboot
```
