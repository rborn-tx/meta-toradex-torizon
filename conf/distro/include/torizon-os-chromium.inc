# Chromium base additions
CORE_IMAGE_EXTRA_INSTALL += "chromium-ozone-wayland weston weston-init weston-examples"
LICENSE_FLAGS_ACCEPTED += "commercial_libav commercial_x264"
DISTRO_FEATURES += "wayland vulkan opengl seccomp"
DISTRO_FEATURES_TORIZON_REMOVE:remove = "alsa wayland x11 opengl vulkan"
IMAGE_FEATURES += "weston"
DISTRO_FEATURES_NATIVE_TORIZON_REMOVE:remove = "x11"
DISTRO_FEATURES_NATIVESDK_TORIZON_REMOVE:remove = "x11"
