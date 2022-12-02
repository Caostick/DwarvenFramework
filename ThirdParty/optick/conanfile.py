from conans import ConanFile, CMake
import os


class optickConan(ConanFile):
    name = "optick"
    version = "master"
    license = "optick license"
    author = "optick provider"
    url = "optick url"
    description = "optick"
	
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "cmake"

    scm = {
        "type": "git",
        "subfolder": "optick",
        "url": "https://github.com/bombomby/optick.git",
        "revision": "master"
    }
	
    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="optick")
        cmake.build()
        cmake.install()

    def package(self):
        cmake = CMake(self)
		
    def package_info(self):
        self.cpp_info.includedirs = [os.path.join("include")]
        if self.settings.build_type == "Debug":
            self.cpp_info.libs = ["OptickCored"]
        else:
            self.cpp_info.libs = ["OptickCore"]