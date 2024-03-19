require conf/distro/include/torizon-os-nonostree.inc
inherit mender-full
inherit mender-toradex

MENDER_FEATURES_ENABLE:append = " mender-uboot mender-image-sd"
MENDER_FEATURES_DISABLE:append = " mender-grub mender-image-uefi"
IMAGE_CLASSES += "image_type_mender_tezi"
IMAGE_FSTYPES:append = " mender_tezi"
IMAGE_FSTYPES:remove = " teziimg"
IMAGE_BOOT_FILES:remove = "zImage ${KERNEL_DEVICETREE} overlays.txt overlays/*;overlays/"
MENDER_BOOT_PART_SIZE_MB:verdin-imx8mm = "32"
OFFSET_SPL_PAYLOAD:verdin-imx8mm = ""
KERNEL_DEVICETREE:verdin-imx8mm = "freescale/imx8mm-verdin-wifi-dahlia.dtb"
MENDER_UBOOT_POST_SETUP_COMMANDS:append = " ; setenv tdxargs \${tdxargs} \${bootargs}; "
MENDER_UBOOT_POST_SETUP_COMMANDS:append = " ; setenv overlays_file /boot/overlays.txt ; setenv overlays_prefix boot/overlays/ "
TORADEX_BSP_VERSION = "toradex-bsp-6.2.0"
MENDER_STORAGE_TOTAL_SIZE_MB = "3072"

MENDER_ARTIFACT_NAME ?= "release-1"
