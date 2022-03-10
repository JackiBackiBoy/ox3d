<p align="center">
<img width=20% height=20% src="https://user-images.githubusercontent.com/44316688/155890017-8769276f-69d2-48c6-847e-994c2533e118.png">
</p>
<h1 align="center">Ox3d</h1>

<p align="center">
Graphics engine powered by Vulkan® and C++.
</p>

# Compatibility
The graphics engine offers full Vulkan support on all modern versions of Windows, excluding versions released before Windows 7 (6.1.7601). When it comes to the macOS version of the engine there is at the time writing no native Vulkan support from Apple, since Apple expects the Metal graphics API to be used instead.

However, the Vulkan SDK provides *partial* Vulkan support through the use of the MoltenVK library which is a "translation" or "porting" library that maps most of the Vulkan functionality to the underlying graphics support (via Metal) on macOS, iOS, and tvOS platforms. It is **not** a fully-conforming Vulkan driver for macOS, iOS, or tvOS devices but it works as a preliminary solution until the Metal API has been integrated in the engine.

# Build Instructions
## Prerequisites
- [Vulkan SDK](https://www.lunarg.com/vulkan-sdk/) version 1.3.204 or newer
- [CMake](https://cmake.org/) version 3.20.0 or newer


## Building for Windows
In order to build one can use the included build-script ```winBuild.bat```, or by building it manually with the follow shell commands:
```bash
mkdir build
cd build
cmake ..
make
```

This will generate an executable file named ```ox3d``` which can then be ran using ```./ox3d```.

## Building for macOS
Installing CMake can either be done manually by following the link above, or by using a package manager such as [Homebrew](https://brew.sh/)
```bash
brew install cmake
```

The project can then be built by using the included build-script ```unixBuild.sh```, or by building it manually with the follow shell commands:
```bash
mkdir build
cd build
cmake ..
make
```

This will generate an executable file named ```ox3d``` which can then be ran using ```./ox3d```.

  
# ox3d (currently under full reconstruction) 🚧
As of February 18 2022 the project is making a full transition to Vulkan and Metal graphics APIs. In order to guarantee a solid structure the "deprecated" branch will act as the preliminary version of the graphics (currently written in OpenGL and only available for Windows). The aim is to allow the graphics engine to run optimally on all three major platforms i.e. Linux, macOS and Windows.

# Roadmap
- [ ] Integrate Vulkan graphics API (Windows, Linux) - **Projected release:** March
- [ ] Integration Metal graphics API (macOS, iOS, tvOS) - **Projected release:** May
