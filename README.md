# Linux Kernel for Understanding Host Network Stack Latency

## Installing instructions

1. Copy one of config files in `/boot` to the local folder 
   ``` 
      cp /boot/config-some-kernel-version  .config
   ```

2. (Optional) Fully clean the kernel build and configurations with:
   ```shell
   sudo make clean
   sudo make mrproper
   ```

3. Generate kernel configuration with old config file with `make oldconfig` if both kernels are the same version. Or with `make olddefconfig` if you only want an intersection set. Note you don't need `sudo` privilage to generate configurations.

   ```shell
   # Another .config from 5.10.46
   make oldconfig

   # Another .config from other versions
   make olddefconfig
   ```

4. In `.config`:
   1. set `CONFIG_IRQ_TIME_ACCOUNTING=y` to turn on IRQ time accounting for enabling the solution, 
   2. set `CONFIG_LOCALVERSION` to a customized name to avoid collision with existing kernels, 
   3. and confirm the kernel name after compilation with:
      ```shell
      sudo make kernelrelease
      ```

5. In `include/net/latency.h`, change the `LATENCY_MONITOR_SOURCE_IP` macro to the address of of the local server. This address will be used to identify our experimental applications.

6. (Optinal) Clean old built kernel with the same local version name with the localversion seen in `/boot` folder.
   ```shell
   ver='5.10.46-{LOCALVERSION}'
   sudo update-initramfs -d -k "$ver"
   sudo rm -rf "/lib/modules/$ver"
   sudo rm -f "/boot/vmlinuz-$ver" "/boot/System.map-$ver" "/boot/config-$ver"
   ls /boot | grep -F "$ver" || echo "No /boot leftovers for $ver"
   sudo update-grub
   ```

7. Compile and build the kernel:
   ```shell
   sudo make -j$(nproc) bzImage
   sudo make -j$(nproc) modules
   sudo make modules_install
   sudo make install
   ```

8. (Optional) Change the default kernel version:
   1. edit default grub
      ```shell
      sudo vim /etc/default/grub
      ```
   2. comment out the current `GRUB_DEFAULT` line and add a new line:
      ```shell
      GRUB_DEFAULT="1>Ubuntu, with Linux 5.10.46-{LOCALVERSION}"
      ```

9. Update the grub menu and then reboot the servers:
   ```shell
   sudo update-grub2
   sudo grub-reboot "Advanced options for Ubuntu>Ubuntu, with Linux 5.10.46-{LOCALVERSION}"
   sudo reboot
   ```
