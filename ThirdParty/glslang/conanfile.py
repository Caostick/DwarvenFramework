from conans import ConanFile, CMake
import os


class glslangConan(ConanFile):
    name = "glslang"
    version = "master"
    license = "glslang license"
    author = "glslang provider"
    url = "glslang url"
    description = "glslang"
	
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}
    generators = "cmake"

    scm = {
        "type": "git",
        "subfolder": "glslang",
        "url": "https://github.com/KhronosGroup/glslang.git",
        "revision": "master"
    }
	
    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder="glslang")
        cmake.build()

    def package(self):
        self.copy("glslang/*.h", dst="include", keep_path=True)
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.includedirs = [os.path.join("include", "glslang")]
        if self.settings.build_type == "Debug":
            self.cpp_info.libs = ["glslangd", "GenericCodeGend", "glslang-default-resource-limitsd", "HLSLd", "MachineIndependentd", "OGLCompilerd", "OSDependentd", "SPIRVd", "SPVRemapperd"]
        else:
            self.cpp_info.libs = ["glslang", "GenericCodeGen", "glslang-default-resource-limits", "HLSL", "MachineIndependent", "OGLCompiler", "OSDependent", "SPIRV", "SPVRemapper"]