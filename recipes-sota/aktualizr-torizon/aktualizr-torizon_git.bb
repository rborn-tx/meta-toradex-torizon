SUMMARY = "Toradex implementation of the Aktualizr SOTA client"
LICENSE = "MPL-2.0"
LIC_FILES_CHKSUM = "file://${S}/LICENSE;md5=815ca599c9df247a0c7f619bab123dad"

GARAGE_SIGN_PV = "0.7.4-25-g7cfca74"
SRC_URI[garagesign.md5sum] = "584cd16aa7824e34b593dae63796466b"
SRC_URI[garagesign.sha256sum] = "c7d5fdceef3e815363e3aa398c38643ca213f9b7f66d50f55c76a66cb74565d2"

SRC_URI = " \
  gitsm://github.com/toradex/aktualizr.git;protocol=https;branch=toradex-master \
  file://aktualizr-torizon.service \
  file://gateway.url \
  file://root.crt \
  https://tuf-cli-releases.ota.here.com/cli-${GARAGE_SIGN_PV}.tgz;unpack=0;name=garagesign \
"

SRCREV = "d71f18a1b8139cd4adadb41667b73d7c92fdbd9f"
SRCREV:use-head-next = "${AUTOREV}"

S = "${WORKDIR}/git"

PV = "1.0+git${SRCPV}"

DEPENDS = "boost curl openssl libarchive libsodium sqlite3 asn1c-native"
DEPENDS:append = "${@bb.utils.contains('OSTREE_ENABLED', '1', ' ostree ', '', d)}"

# GSoC: removed: "aktualizr-default-sec greenboot"
RDEPENDS:${PN}:class-target = "bash lshw aktualizr-hwid aktualizr-polling-interval aktualizr-reboot"

inherit cmake pkgconfig systemd

# GSoC: disable OSTree:
OSTREE_ENABLED = "0"

SYSTEMD_SERVICE:${PN} = "aktualizr-torizon.service"

# For find_package(Git)
OECMAKE_FIND_ROOT_PATH_MODE_PROGRAM = "BOTH"

PACKAGECONFIG ?= " \
    ${@bb.utils.filter('SOTA_CLIENT_FEATURES', 'hsm serialcan ubootenv', d)} \
    ${@bb.utils.contains('OSTREE_ENABLED', '1', ' ostree ', '', d)} \
"
PACKAGECONFIG[warning-as-error] = "-DWARNING_AS_ERROR=ON,-DWARNING_AS_ERROR=OFF,"
PACKAGECONFIG[ostree] = "-DBUILD_OSTREE=ON,-DBUILD_OSTREE=OFF,ostree,"
PACKAGECONFIG[ubootenv] = ",,u-boot-fw-utils,u-boot-fw-utils"
PACKAGECONFIG:remove:class-native = "ubootenv"
PACKAGECONFIG:class-native = "sota-tools"
PACKAGECONFIG[sota-tools] = "-DBUILD_SOTA_TOOLS=ON -DGARAGE_SIGN_ARCHIVE=${WORKDIR}/cli-${GARAGE_SIGN_PV}.tgz, -DBUILD_SOTA_TOOLS=OFF,glib-2.0,"

PROVIDES += "aktualizr"
RPROVIDES:${PN} += "aktualizr aktualizr-info aktualizr-shared-prov"

do_install:append() {
    install -d ${D}${systemd_unitdir}/system
    install -m 0644 ${WORKDIR}/aktualizr-torizon.service ${D}${systemd_unitdir}/system/aktualizr-torizon.service

    install -m 0700 -d ${D}${libdir}/sota/conf.d 
    install -m 0644 ${S}/config/sota-device-cred.toml ${D}/${libdir}/sota/conf.d/20-sota-device-cred.toml
    install -m 0644 ${S}/config/sota-uboot-env.toml ${D}/${libdir}/sota/conf.d/30-rollback.toml
    
    install -m 0644 ${WORKDIR}/gateway.url ${D}/${libdir}/sota/gateway.url
    install -m 0644 ${WORKDIR}/root.crt ${D}/${libdir}/sota/root.crt
}

PACKAGES =+ "${PN}-misc"

FILES:${PN} += " \
  ${bindir}/aktualizr-torizon \
  ${libdir}/libaktualizr.so \
  ${systemd_unitdir}/system/aktualizr-torizon.service \
  ${sysconfdir}/sota/* \
  ${libdir}/sota/* \
  ${libdir}/sota/conf.d \
  ${libdir}/sota/conf.d/20-sota-device-cred.toml \
  ${libdir}/sota/conf.d/30-rollback.toml \
  ${bindir}/aktualizr-info \
  ${binddir}/aktualizr-cert-provider \
"

FILES:${PN}-dev = " \
  ${libdir}/libsota_tools.so \
  ${includedir}/libaktualizr \
  ${libdir}/pkgconfig \
"

FILES:${PN}-misc = " \
  ${bindir}/aktualizr-secondary \ 
  ${libdir}/libaktualizr_secondary.so \
  ${libdir}/libsota_tools.so \
  ${bindir}/aktualizr-get \
  ${bindir}/uptane-generator \
"

BBCLASSEXTEND = "native"
