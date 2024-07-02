from conan import ConanFile


class Sdl2MinesweeperRecipe(ConanFile):
    settings = 'os', 'compiler', 'build_type', 'arch'
    generators = 'CMakeDeps'

    def requirements(self):
        self.requires("gtest/1.14.0")
