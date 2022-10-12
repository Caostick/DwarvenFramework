from conans import ConanFile, CMake
import os


class glslangConan(ConanFile):
    name = "assimp"
    version = "master"
    license = "assimp license"
    author = "assimp provider"
    url = "assimp url"
    description = "assimp"
	
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "cmake"

    scm = {
        "type": "git",
        "subfolder": "assimp",
        "url": "https://github.com/assimp/assimp.git",
        "revision": "master"
    }
	
    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="assimp")
        cmake.build()

    def package(self):
        self.copy("*config.h", dst="include/assimp/include/assimp", keep_path=False)
        self.copy("assimp/*.h", dst="include", keep_path=True)
        self.copy("assimp/*.inl", dst="include", keep_path=True)
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)
		
    def package_info(self):
        self.cpp_info.includedirs = [os.path.join("include", "assimp", "include")]
        if self.settings.build_type == "Debug":
            self.cpp_info.libs = ["assimp-vc142-mtd"]
        else:
            self.cpp_info.libs = ["assimp-vc142-mt"]