SUMMARY = "Bootloader artifacts needed by TorizonCore Builder to re-sign an image"
DESCRIPTION = "Packs the intermediate, pre-signing bootloader artifacts that \
TorizonCore Builder needs to recreate and re-sign the bootloader of a pre-built \
Torizon OS image. This recipe aggregates output deployed by other recipes; it \
builds nothing of its own."
SECTION = "BSP"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

require recipes-bsp/tcb-signing-files/tcb-signing-files.inc

inherit deploy nopackages

PACKAGE_ARCH = "${MACHINE_ARCH}"
COMPATIBLE_MACHINE = "(verdin-imx8mp|verdin-imx8mm)"

INHIBIT_DEFAULT_DEPS = "1"

do_fetch[noexec] = "1"
do_unpack[noexec] = "1"
do_patch[noexec] = "1"
do_configure[noexec] = "1"
do_install[noexec] = "1"

B = "${WORKDIR}/build"

# This recipe aggregates output deployed by other recipes, so it must depend on
# their deploy tasks; DEPLOY_DIR_IMAGE is otherwise just a directory that may or
# may not have been filled in yet.
do_compile[depends] += "${@' '.join('%s:do_deploy' % r for r in d.getVar('TCB_SIGNING_INPUT_DEPENDS').split())}"

# The filelist decides the archive's contents, and the enable condition decides
# whether anything downstream uses it.
do_compile[vardeps] += "TCB_SIGNING_FILELIST TCB_SIGNING_SUPPORT"

do_compile() {
    if [ -z "${TCB_SIGNING_FILELIST}" ]; then
        bbfatal "TCB_SIGNING_FILELIST is empty for MACHINE \"${MACHINE}\", so there" \
                "is nothing to pack. Either define it or keep this recipe out of the" \
                "build (see TCB_SIGNING_SUPPORT)."
    fi

    # Reproducible archive: without this, the members' mtimes track when
    # do_deploy ran, ownership tracks the build user, and directories are packed
    # in readdir order; so two builds of the same sources produce different
    # bytes. The gzip layer needs no help: tar compresses a stream, and gzip
    # stores neither a name nor a timestamp for one.
    #
    # Timestamps follow the reproducible-builds convention and are normalised
    # only when SOURCE_DATE_EPOCH is set; sorting, ownership and link handling
    # do not depend on it and are always applied.
    #
    # Note: SOURCE_DATE_EPOCH is in BB_BASEHASH_IGNORE_VARS, so referencing it
    # neither changes this task's hash nor rebuilds the tarball when it changes.
    tar_timestamp_args=""
    if [ -n "${SOURCE_DATE_EPOCH}" ]; then
        tar_timestamp_args="--mtime=@${SOURCE_DATE_EPOCH} --clamp-mtime"
    fi

    # Change to DEPLOY_DIR_IMAGE so that the filelist's globs are expanded
    # there; the shell expands them where the command runs and tar cannot do it.
    #
    # Note: --format is pinned so that a future tar changing its default cannot
    # change the archive's bytes.
    (cd "${DEPLOY_DIR_IMAGE}" && tar \
        --dereference --hard-dereference \
        --sort=name --format=gnu \
        ${tar_timestamp_args} \
        --owner=0 --group=0 --numeric-owner \
        -czf "${B}/${TCB_SIGNING_FILES_TARBALL}" ${TCB_SIGNING_FILELIST})
}
do_compile[dirs] = "${B}"
do_compile[cleandirs] = "${B}"

do_deploy() {
    install -m 0644 "${B}/${TCB_SIGNING_FILES_TARBALL}" "${DEPLOYDIR}/${TCB_SIGNING_FILES_TARBALL}"
}
addtask deploy after do_compile before do_build
