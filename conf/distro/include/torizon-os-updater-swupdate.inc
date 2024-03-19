require conf/distro/include/torizon-os-nonostree.inc

IMAGE_CLASSES += "image_types_zchunk image_types_swupdate_tezi"
IMAGE_INSTALL:append = " swupdate swupdate-www swupdate-progress libubootenv-bin "
IMAGE_FSTYPES:append = " ext4.gz ext4.zck ext4.zck.zckheader swupdate_tezi "
IMAGE_FSTYPES:remove = " teziimg"
WKS_FILE:verdin-imx8mm = "ts-verdin-imx8mm.wks"
OFFSET_SPL_PAYLOAD:verdin-imx8mm = ""
KERNEL_DEVICETREE:verdin-imx8mm = "freescale/imx8mm-verdin-wifi-dahlia.dtb"

ROOTFS_POSTPROCESS_COMMAND:append = " toradex_swupdate_update_boot;"
toradex_swupdate_update_boot() {
    # Copy files intended for the boot partition into the /boot directory of
    # the root partition.  We will modify the UBoot environment to load them
    # from there.  I feel like there is a cleaner way to do this but cannot
    # find it at the moment.
    # This should probably work using IMAGE_BOOT_FILES rather than hardcoding
    # but this is enough for my benchmarking needs
    cp ${DEPLOY_DIR_IMAGE}/overlays.txt ${IMAGE_ROOTFS}/boot/overlays.txt
    cp -R ${DEPLOY_DIR_IMAGE}/overlays/ ${IMAGE_ROOTFS}/boot/overlays
    cp ${DEPLOY_DIR_IMAGE}/${KERNEL_IMAGETYPE} ${IMAGE_ROOTFS}/boot/
    cp ${DEPLOY_DIR_IMAGE}/$(basename ${KERNEL_DEVICETREE}) ${IMAGE_ROOTFS}/boot/
    cp ${DEPLOY_DIR_IMAGE}/boot.scr-${MACHINE} ${IMAGE_ROOTFS}/boot/boot.scr
}
