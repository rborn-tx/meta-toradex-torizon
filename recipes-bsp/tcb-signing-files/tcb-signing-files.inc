# Common definitions for the "TCB signing files" tarball.
#
# The tarball holds the intermediate, pre-signing bootloader artifacts that
# TorizonCore Builder needs in order to re-sign a pre-built Torizon OS image.
# It is built by the tcb-signing-files recipe and shipped inside the Toradex
# Easy Installer image.
#
# This file is required by both sides so that the two agree on one definition:
#
#   - recipes-bsp/tcb-signing-files/tcb-signing-files.bb  -- builds and deploys it
#   - classes/image_type_torizon.bbclass                  -- puts it in the Tezi image
#
# Everything the enable condition reads comes from the signing class, which is
# inherited globally (INHERIT += "<...>-signed"), so these values resolve the
# same way in a recipe as in an image.

# Name of the tarball, as deployed and as seen by TCB inside the Tezi image.
TCB_SIGNING_FILES_TARBALL = "tcb_signing_files.tar.gz"

# The recipe that produces it.
TCB_SIGNING_FILES_RECIPE = "tcb-signing-files"

def is_hab_signed_bootloader_and_fit_enabled(d):
    if d.getVar('TDX_IMX_HAB_ENABLE') == '1' and d.getVar('UBOOT_SIGN_ENABLE') == '1':
        return '1'

    return '0'

# Whether this build exports the TCB signing files at all.
TCB_SIGNING_SUPPORT ?= "0"
TCB_SIGNING_SUPPORT:verdin-imx8mp ?= "${@is_hab_signed_bootloader_and_fit_enabled(d)}"
TCB_SIGNING_SUPPORT:verdin-imx8mm ?= "${@is_hab_signed_bootloader_and_fit_enabled(d)}"

# What goes into the tarball, relative to DEPLOY_DIR_IMAGE. Entries are expanded
# by the shell in that directory, so globs work; an entry that matches nothing
# fails the build rather than producing a short tarball.
TCB_SIGNING_FILELIST ?= ""
TCB_SIGNING_FILELIST:verdin-imx8mp ?= "uboot_config bl31* lpddr4_pmu_train_* u-boot.dtb u-boot-nodtb.bin spl/ u-boot-dtbs/"
TCB_SIGNING_FILELIST:verdin-imx8mm ?= "uboot_config bl31* lpddr4_pmu_train_* u-boot.dtb u-boot-nodtb.bin spl/ u-boot-dtbs/"

# The recipes whose do_deploy output the filelist above reads. tcb-signing-files
# aggregates artifacts produced by other recipes, so it has to depend on their
# deploy tasks explicitly -- the same arrangement meta-freescale's imx-boot uses
# for the same reason. Keep this in step with TCB_SIGNING_FILELIST: a file that
# is packed but not depended upon is a race, not a shortcut.
#
# For the iMX8M machines this is the same set the U-Boot recipe itself depends on
# when it assembles the boot container (imx-boot-container.bbclass), expressed
# through the same variables so that the two cannot drift apart:
#
#   uboot_config, spl/, u-boot-dtbs/, u-boot.dtb, u-boot-nodtb.bin
#                                       <- virtual/bootloader
#   bl31*                               <- IMX_DEFAULT_ATF_PROVIDER  (imx-atf)
#   lpddr4_pmu_train_*                  <- IMX_EXTRA_FIRMWARE        (imx-boot-firmware-files)
TCB_SIGNING_INPUT_DEPENDS ?= ""
TCB_SIGNING_INPUT_DEPENDS:verdin-imx8mp ?= "virtual/bootloader ${IMX_DEFAULT_ATF_PROVIDER} ${IMX_EXTRA_FIRMWARE}"
TCB_SIGNING_INPUT_DEPENDS:verdin-imx8mm ?= "virtual/bootloader ${IMX_DEFAULT_ATF_PROVIDER} ${IMX_EXTRA_FIRMWARE}"
