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

        # SDL options
        "sdl/*:shared": True,
        # Subsystems
        "sdl/*:audio": False,
        "sdl/*:video": True,
        "sdl/*:gpu": False,
        "sdl/*:render": True,
        "sdl/*:camera": False,
        "sdl/*:joystick": False,
        "sdl/*:haptic": False,
        "sdl/*:hidapi": False,
        "sdl/*:power": False,
        "sdl/*:sensor": False,
        "sdl/*:dialog": False,
        # Audio
        # Linux only
        "sdl/*:alsa": False,
        "sdl/*:pulseaudio": False,
        "sdl/*:sndio": False,
        # Video
        "sdl/*:opengl": False,
        "sdl/*:opengles": False,
        "sdl/*:x11": False,
        "sdl/*:xcursor": False,
        "sdl/*:xdbe": False,
        "sdl/*:xinput": False,
        "sdl/*:xfixes": False,
        "sdl/*:xrandr": False,
        "sdl/*:xscrnsaver": False,
        "sdl/*:xshape": False,
        "sdl/*:xsync": False,
        "sdl/*:wayland": False,
        "sdl/*:vulkan": False,
        "sdl/*:metal": False,
        "sdl/*:directx": False,
        # Hidapi
        "sdl/*:libusb": False,
        # Other
        "sdl/*:libudev": False,
        "sdl/*:dbus": False,
        "sdl/*:libiconv": False,
    }

    def config_options(self):
        self.options["sdl/*"].x11 = self.settings.os == "Linux"
        self.options["sdl/*"].xinput = self.settings.os == "Linux"

    def requirements(self):
        self.requires("gtest/1.17.0")
        self.requires("sdl/3.4.0")
        self.requires("sdl_image/3.4.0")
        self.requires("spdlog/1.17.0")

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
