EXTRA_IMAGE_FEATURES += "debug-tweaks"
IMAGE_ROOTFS_SIZE = "1048576"
CORE_IMAGE_BASE_INSTALL:remove = "usermount"
IMAGE_INSTALL:append = " os-release udev-extraconf "

# These settings are explicitly only needed for chromium
# but to make sure all builds use the same settings will
# will apply them here.
LICENSE_FLAGS_ACCEPTED += "commercial_libav commercial_x264"
DISTRO_FEATURES += "wayland vulkan opengl seccomp"
DISTRO_FEATURES_TORIZON_REMOVE:remove = "alsa wayland x11 opengl vulkan"
DISTRO_FEATURES_NATIVE_TORIZON_REMOVE:remove = "x11"
DISTRO_FEATURES_NATIVESDK_TORIZON_REMOVE:remove = "x11"

# Note: BENCHMARK_INCLUDED_* variables are defined in bblayers.conf.
require conf/distro/include/torizon-os-browser-${BENCHMARK_INCLUDED_BROWSER}.inc
require conf/distro/include/torizon-os-updater-${BENCHMARK_INCLUDED_UPDATER}.inc
