For the i.MX 6Quad/DualLite SABRE-SD Board, the build procedure usually depends on whether you want:

1. Linux BSP (Yocto-based)
2. Android BSP
3. Standalone U-Boot + kernel build
4. Buildroot

The most common workflow today is **Yocto Linux BSP** using NXP’s `meta-imx`.

---

# 1. Host Setup (Ubuntu)

Recommended:

* Ubuntu 20.04 or 22.04
* Required packages:

```bash
sudo apt update

sudo apt install gawk wget git diffstat unzip texinfo gcc \
build-essential chrpath socat cpio python3 python3-pip \
python3-pexpect xz-utils debianutils iputils-ping \
python3-git python3-jinja2 libegl1-mesa libsdl1.2-dev \
xterm rsync curl
```

---

# 2. Download NXP Yocto BSP

Create workspace:

```bash
mkdir -p ~/imx6
cd ~/imx6
```

Clone NXP manifest:

```bash
repo init -u https://github.com/nxp-imx/imx-manifest.git \
-b imx-linux-kirkstone -m imx-5.15.71-2.2.0.xml

repo sync
```

---

# 3. Setup Yocto Environment

Initialize build environment:

```bash
DISTRO=fsl-imx-wayland MACHINE=imx6qsabresd \
source imx-setup-release.sh -b build
```

For DualLite:

```bash
MACHINE=imx6dlsabresd
```

Typical machines:

* `imx6qsabresd` → Quad
* `imx6dlsabresd` → DualLite

NXP BSP includes:

* U-Boot
* Linux kernel
* Device tree
* Root filesystem ([NXP Semiconductors][1])

---

# 4. Build Image

Common image:

```bash
bitbake imx-image-full
```

Smaller image:

```bash
bitbake core-image-minimal
```

Build artifacts appear in:

```bash
build/tmp/deploy/images/imx6qsabresd/
```

Typical outputs:

* `.wic`
* `Image`
* `.dtb`
* `imx-boot`

---

# 5. Flash SD Card

Find SD device:

```bash
lsblk
```

Flash image:

```bash
sudo dd if=imx-image-full-imx6qsabresd.wic \
of=/dev/sdX bs=1M status=progress conv=fsync
```

Replace:

* `/dev/sdX` with your SD card device.

---

# 6. Boot Switch Settings

For SABRE-SD SD-card boot:

SW6:

| 1  | 2   | 3   | 4   | 5   | 6   | 7  | 8   |
| -- | --- | --- | --- | --- | --- | -- | --- |
| ON | OFF | OFF | OFF | OFF | OFF | ON | OFF |

([NXP Semiconductors][2])

---

# 7. Serial Console

Use:

* 115200 baud
* 8N1

Linux host:

```bash
sudo minicom -D /dev/ttyUSB0 -b 115200
```

---

# 8. Standalone U-Boot Build

Clone U-Boot:

```bash
git clone https://source.denx.de/u-boot/u-boot.git
cd u-boot
```

Configure:

```bash
make mx6sabresd_defconfig
```

Build:

```bash
make -j$(nproc)
```

Outputs:

* `SPL`
* `u-boot-dtb.img`

Flash:

```bash
sudo dd if=SPL of=/dev/sdX bs=1K seek=1 conv=notrunc
sudo dd if=u-boot-dtb.img of=/dev/sdX bs=1K seek=69 conv=notrunc
sync
```

([U-Boot Documentation][3])

---

# 9. Standalone Linux Kernel Build

Clone kernel:

```bash
git clone https://github.com/nxp-imx/linux-imx.git
cd linux-imx
```

Checkout matching branch:

```bash
git checkout lf-5.15.y
```

Configure:

```bash
make imx_v7_defconfig
```

Build:

```bash
make -j$(nproc) zImage dtbs
```

Outputs:

* `arch/arm/boot/zImage`
* DTBs in:
  `arch/arm/boot/dts/`

---

# 10. Boot Files Needed

Minimum boot components:

* U-Boot
* Kernel (`zImage`)
* Device tree (`.dtb`)
* Root filesystem

([NXP Semiconductors][1])

---

# 11. Typical SD Card Layout

## Partition 1 (FAT32)

Contains:

* zImage
* *.dtb
* boot.scr

## Partition 2 (ext4)

Contains:

* rootfs

---

# 12. Common U-Boot Boot Command

Example:

```bash
setenv bootargs console=ttymxc0,115200 root=/dev/mmcblk1p2 rootwait rw

fatload mmc 2:1 ${loadaddr} zImage
fatload mmc 2:1 ${fdt_addr} imx6q-sabresd.dtb

bootz ${loadaddr} - ${fdt_addr}
```

---

# 13. Useful Debug Tips

## Check boot source

In U-Boot:

```bash
mmc list
mmc dev 0
mmc part
```

## Verify DTB

Wrong DTB is the #1 boot issue on i.MX6.

Examples:

* `imx6q-sabresd.dtb`
* `imx6dl-sabresd.dtb`

---

If you want, I can also provide:

* complete Yocto build script
* exact BSP version recommendations
* Android build procedure
* secure boot procedure
* custom board porting guide
* flashing eMMC instead of SD
* TFTP/NFS boot setup
* fast boot optimization for i.MX6
* Qt/Wayland graphics enablement
* Dockerized build environment for i.MX6 Yocto builds

