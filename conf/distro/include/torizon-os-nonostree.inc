# For swupdate, Rauc and Mender
EXTRA_IMAGE_FEATURES += "read-only-rootfs read-only-rootfs-delayed-postinsts"
DISTRO_FEATURES:remove = " sota "
INITRAMFS_IMAGE:forcevariable = ""
IMAGE_FSTYPES:remove = " ostreepush garagesign garagecheck "
BBMASK:append = " \
    /meta-updater \
    /meta-toradex-torizon/recipes-bsp/u-boot/ \
    /meta-openembedded/meta-oe/recipes-extended/ostree \
"
CORE_IMAGE_BASE_INSTALL:remove = " \
    ostree-customize-plymouth \
    ostree-devicetree-overlays \
"
IMAGE_CLASSES:remove = "image_type_torizon image_types_ostree image_types_ota"
IMAGE_CLASSES_TORIZON_REMOVE:remove = "image_type_tezi"
