from conans import ConanFile, CMake, tools
import os

class WrappedVarConan(ConanFile):
    name = "wrapped_var"
    description = "Mutex variable wrappers like Rust's std::sync::Mutex but for C++"
    version = "0.1"
    license = "MIT"
    url = "https://github.com/mik90/wrapped_var"
    settings = "os", "compiler", "build_type", "arch"
    generators = "cmake"
    exports_sources = "include/*", "CMakeLists.txt", "test/*", "cmake/*"
    no_copy_source = True
    testing_enabled = tools.get_env("CONAN_RUN_TESTS", True)

    def requirements(self):
        if self.testing_enabled:
            self.requires("gtest/1.10.0")

    # Only for building tests
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        if self.testing_enabled:
            cmake.test()

    def package(self):
        self.copy("*.hpp", src="includes", dst="wrapped_var")
        if tools.get_env("CONAN_RUN_TESTS", True):
            self.copy("*.cpp", src="test", dst="test")

    def package_id(self):
        self.info.header_only()
