from conan import ConanFile
from conan.tools.files import copy
import pathlib


class MswprRecipe(ConanFile):
    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = 'CMakeDeps'
    default_options = {
        'sdl_image/*:shared': True,
        "sdl_image/*:with_jxl": False,
        "sdl_image/*:with_libjpeg": False,
        "sdl_image/*:with_libtiff": False,
        "sdl_image/*:with_libpng": False,
        "sdl_image/*:with_libwebp": False,
        "sdl_image/*:with_avif": False,
        'sdl/*:gpu': False,
        'sdl/*:x11': False,
        'sdl/*:alsa': False,
        'sdl/*:dbus': False,
        'sdl/*:xdbe': False,
        'sdl/*:audio': False,
        'sdl/*:power': False,
        'sdl/*:sndio': False,
        'sdl/*:video': True,
        'sdl/*:xsync': False,
        'sdl/*:camera': False,
        'sdl/*:dialog': False,
        'sdl/*:haptic': False,
        'sdl/*:hidapi': False,
        'sdl/*:libusb': False,
        'sdl/*:opengl': False,
        'sdl/*:render': True,
        'sdl/*:sensor': False,
        'sdl/*:shared': True,
        'sdl/*:vulkan': False,
        'sdl/*:xfixes': False,
        'sdl/*:xinput': False,
        'sdl/*:xrandr': False,
        'sdl/*:xshape': False,
        'sdl/*:libudev': False,
        'sdl/*:wayland': False,
        'sdl/*:xcursor': False,
        'sdl/*:joystick': False,
        'sdl/*:libiconv': False,
        'sdl/*:opengles': False,
        'sdl/*:pulseaudio': False,
        'sdl/*:xscrnsaver': False,
    }

    def config_options(self):
        self.options["sdl/*"].x11 = self.settings.os == "Linux"

    def requirements(self):
        self.requires("gtest/1.15.0")
        self.requires("sdl/3.2.20")
        self.requires("sdl_image/3.2.4")
        self.requires("spdlog/1.15.0")

    def generate(self):
        if not self.settings.os == "Windows":
            return

        for dep in self.dependencies.values():
            if dep.cpp_info.bindirs:
                dst_folder = pathlib.Path(self.build_folder).parent
                extension = 'dll'
                self.output.info(
                    f'Looking by mask *.{extension} in bindir: {dep.cpp_info.bindir}')
                copied_files = copy(
                    self, pattern=f'*.{extension}',
                    src=dep.cpp_info.bindir,
                    dst=dst_folder
                )
                self.output.info(f'Copied files: {copied_files}')
