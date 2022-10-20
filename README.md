# DwarvenFramework (Very WIP. Unusable atm)

Modules:
=============
- [DvarvenCore](https://github.com/Caostick/DwarvenFramework/tree/main/DwarvenCore)

- [DwarfResources](https://github.com/Caostick/DwarvenFramework/tree/main/DwarfResources)
- [DwarfWindow](https://github.com/Caostick/DwarvenFramework/tree/main/DwarfWindow)
- [DwarfRenderer](https://github.com/Caostick/DwarvenFramework/tree/main/DwarfRenderer)
- [DwarfInput](https://github.com/Caostick/DwarvenFramework/tree/main/DwarfInput)

Dependencies:
=============
- [Vulkan SDK](https://vulkan.lunarg.com)
- [Conan](https://conan.io)
- [Python](https://www.python.org)

Build:
=============
ThirdParty:

Direct build: 'conan create . -s build_type=Debug'

All build: 'conan install .' or 'conan install . --build missing'
Example for Debug: 'conan install . -s build_type=Debug --build missing'
