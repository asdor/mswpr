from conan import ConanFile


class Sdl2MinesweeperRecipe(ConanFile):
    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = 'CMakeDeps'

    def requirements(self):
        self.requires("gtest/1.14.0")
        self.requires("sdl/2.28.3")
        self.requires("sdl_image/2.6.3")
