require recipes-bsp/u-boot/u-boot-ota.inc
require recipes-bsp/u-boot/u-boot-rollback.inc

# Dummy implementation: prevent conflict with main version file.
deploy_version_file:k3r5 () {
    bbdebug 1 "u-boot-version file generation skipped for 'k3r5'"
}

# Dummy implementation: prevent conflict with main environment file.
deploy_environ_file:k3r5 () {
    bbdebug 1 "u-boot-initial-env.raw file generation skipped for 'k3r5'"
}

FILESEXTRAPATHS:prepend := "${THISDIR}:"

SRC_URI:remove:k3 = "git://git.toradex.com/u-boot-toradex.git;protocol=https;branch=toradex_ti-u-boot-2024.04"
SRC_URI:prepend:k3 = "git://github.com/rborn-tx/u-boot.git;protocol=https;branch=toradex_ti-u-boot-2024.04-am62-bootflow "
SRC_URI:append:k3 = " \
    file://enable-bootstd-debug.cfg \
"
