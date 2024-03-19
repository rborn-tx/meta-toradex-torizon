require conf/distro/include/torizon-os-nonostree.inc

PREFERRED_RPROVIDER_u-boot-default-env = "u-boot-toradex"

DISTRO_FEATURES += "rauc"
IMAGE_CLASSES += "image_types_rauc_tezi"
IMAGE_INSTALL:append = " rauc rauc-grow-data-part casync "
IMAGE_FSTYPES:append = " rauc_tezi "
IMAGE_FSTYPES:remove = " teziimg"
WKS_FILE = "sdimage-dual-toradex.wks.in"
OFFSET_SPL_PAYLOAD:verdin-imx8mm = ""
KERNEL_DEVICETREE:verdin-imx8mm = "freescale/imx8mm-verdin-wifi-dahlia.dtb"

RAUC_KEYRING_FILE = "/work/dmoseley/local/rauc-example-ca/ca.cert.pem"
RAUC_KEY_FILE = "/work/dmoseley/local/rauc-example-ca/private/development-1.key.pem"
RAUC_CERT_FILE = "/work/dmoseley/local/rauc-example-ca/development-1.cert.pem"
RAUC_SLOT_rootfs = "torizon-core-lite"
RAUC_SLOT_rootfs[adaptive] = "block-hash-index"
RAUC_BUNDLE_FORMAT = "verity"
RAUC_CASYNC_BUNDLE = "1"

ROOTFS_POSTPROCESS_COMMAND:append = " toradex_rauc_update_boot;"
toradex_rauc_update_boot() {
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
