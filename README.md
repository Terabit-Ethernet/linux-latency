# Understanding Host Network Stack Latency — Instrumented Linux Kernel

This repository contains the modified Linux 5.10.46 kernel used in the SIGCOMM 2026 paper
*Understanding Host Network Stack Latency*. It instruments the host network stack for
per-layer latency measurement and implements the mechanisms evaluated in the paper:

| Mechanism | Description | `CONFIG_IRQ_TIME_ACCOUNTING` |
| --- | --- | --- |
| Default Linux | Unmodified accounting behavior, used as the baseline | `n` |
| **IRQa** | Linux's built-in IRQ time accounting | `y` |
| **ACC** | Our accurate time accounting | `y` |
| **PCSched** | Requests-count-based scheduling | `y` |

For the experiment setup, workloads, and measurement scripts, see
[Terabit-Ethernet/understand_latency](https://github.com/Terabit-Ethernet/understand_latency).

## Requirements

- A Debian/Ubuntu host that boots via GRUB (the commands below use `update-initramfs`,
  `update-grub`, and `grub-reboot`).
- Root access, roughly 30 GB of free disk space, and a machine you are willing to reboot.
- Build dependencies:

  ```shell
  sudo apt update
  sudo apt install build-essential flex bison bc rsync libncurses-dev libssl-dev libelf-dev dwarves
  ```

> [!IMPORTANT]
Build a **separate kernel image for each machine**. The latency monitor is keyed to the
> host's own IP address, which is hardcoded at compile time (see [step 6](#6-set-the-latency-monitor-source-ip)).

## Installation

### I. Clean the tree

`make mrproper` deletes `.config`, so run it **before** installing a configuration file.

```shell
make clean
make mrproper
```

### II. Choose a kernel configuration

We provide the configuration used in all of our experiments; you can also start from your own.

```shell
# Our configuration (recommended)
cp kernel_config .config

# Or your own
cp PATH_TO_YOUR_CONFIG .config
```

### III. Set the required options

Set `CONFIG_IRQ_TIME_ACCOUNTING` according to the variant you want to measure (see the table
above), and set `CONFIG_LOCALVERSION` to a unique suffix so this kernel does not collide with
anything already in `/boot`.

```shell
# Default Linux
./scripts/config --disable CONFIG_IRQ_TIME_ACCOUNTING

# IRQa, ACC, or PCSched
./scripts/config --enable CONFIG_IRQ_TIME_ACCOUNTING

# Unique name for this build
./scripts/config --set-str CONFIG_LOCALVERSION "-latency"
```

You can also edit `.config` by hand if you prefer.

### IV. Finalize the configuration

```shell
# If you started from our configuration file
make oldconfig

# If you started from a different configuration file
make olddefconfig
```

`oldconfig` prompts for every option that has no value yet; `olddefconfig` accepts the
defaults silently.

### V. Confirm the kernel release name

This is a dry run: it produces no binaries, but it tells you exactly which kernel name the
build will use, so you do not overwrite your working kernel or an earlier build of this one.

```shell
make -s kernelrelease   # e.g. 5.10.46-latency
```

Use this string wherever `$ver` appears below.

### VI. Set the latency monitor source IP

In `include/net/latency.h`, set the `LATENCY_MONITOR_SOURCE_IP` macro to the address of the
machine **this kernel will be installed on**. The macro is compared against
`inet_sk(sk)->inet_saddr` — the socket's **sending** address — and is what identifies traffic
belonging to our experimental applications. Keep the existing literal format of the macro.

### VII. Set the per-stage latency output filter

To limit ftrace output when multiple cores transmit simultaneously, per-stage latency measurements are recorded only for selected logical CPUs. Update the CPU filter in `drivers/net/ethernet/mellanox/mlx5/core/en_tx.c` to select the logical CPUs you want to measure. The default selection (CPUs 1 and 73) requires no changes on CloudLab r650 servers. 

In `mlx5e_txwqe_complete()`, locate the following condition and replace 1 and 73 as needed:
```c
if (sysctl_net_latency_breakdown_on && skb->sport && (cpu == 1 || cpu == 73))
```

### VIII. Remove any previous build with the same release name

Skip this the first time you build. If you are rebuilding with the same `CONFIG_LOCALVERSION`
(for example after switching `CONFIG_IRQ_TIME_ACCOUNTING`), clear the old artifacts first:

```shell
ver=$(make -s kernelrelease)
sudo update-initramfs -d -k "$ver"
sudo rm -rf "/lib/modules/$ver"
sudo rm -f "/boot/vmlinuz-$ver" "/boot/System.map-$ver" "/boot/config-$ver"
ls /boot | grep -F "$ver" || echo "No /boot leftovers for $ver"
sudo update-grub
```

### IX. Build and install

Compile as your normal user; only the install steps need root.

```shell
make -j"$(nproc)" bzImage
make -j"$(nproc)" modules
sudo make -j"$(nproc)" modules_install
sudo make -j"$(nproc)" install
```

### X. Boot into the new kernel

Confirm the menu entry title before selecting the new kernel to reboot, since it must match character for character.

```shell
ver=$(make -s kernelrelease)
sudo update-grub
grep -F "$ver" /boot/grub/grub.cfg   # check the exact entry title
sudo grub-reboot "Advanced options for Ubuntu>Ubuntu, with Linux $ver"
sudo reboot
```

`grub-reboot` applies to the next boot only, so a failed boot falls back to your previous
kernel. Once you are satisfied, `sudo grub-set-default` makes the choice permanent.

### XI. Verify

```shell
uname -r                                 # should print the release name from step 5
grep CONFIG_IRQ_TIME_ACCOUNTING /boot/config-"$(uname -r)"
```

## Experiments

Our experiment settings are documented at
[Terabit-Ethernet/understand_latency](https://github.com/Terabit-Ethernet/understand_latency).

## Citation

If you find this work useful, please cite:

```latex
@inproceedings{UnderstandLatencyUVA,
  title={Understanding Host Network Stack Latency},
  author={Zuo, Tianyu and Hwang, Jaehyun and Tang, Ao and Agarwal, Rachit and Cai, Qizhe},
  booktitle={Proceedings of the ACM SIGCOMM 2026 Conference},
  pages={1127--1140},
  year={2026}
}
```