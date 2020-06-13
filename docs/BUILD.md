
## Build Instructions

- [Introduction](#introduction)
- [Prerequisites](#prerequisites)
  * [Windows Prerequisites](#windows-prerequisites)
  * [Linux Prerequisites](#linux-prerequisites)
  * [MacOS Prerequisites](#macos-prerequisites)
- [Building the project](#building-the-project)
  * [Build on Windows](#build-on-windows)
  * [Build on Linux](#build-on-Linux)
  * [Build on MacOS](#build-on-MacOS)
  * [Build using QtCreator](#build-using-qtcreator)

### Introduction

Project Structure:

* QuickCut - the actual GUI.
* QuickCutConsole - maps the actual key strokes and parses the `profiles.json` file.
* QuickCutService - responsible for making sure that `QuickCutConsole` is running and also future updater is planned.
* QuickCutShared - shared resources/implementations across the projects.

All QuickCut projects use the Qt Framework and are designed to be portable across different platforms. Currently the code should compile on all platforms, however the `QuickCutConsole` and `QuickCutService` require a Unix hook implementation to make this work on Linux and MacOS which has yet to be developed (pull requests for this feature are welcomed!).


### Prerequisites

Please note that these are the environment that can be confirmed as working, however, different versions could potentially work as well.

* [Conan Package Manager](https://docs.conan.io/en/latest/installation.html).
* [CMake](https://cmake.org/download/) - You can install any version greater than v12.
* [Qt 5.15.0](https://www.qt.io/download/)
* [Boost](https://www.boost.org/) - no specific version required (Tested on versions greater than 1.69.0). Note that if you use conan package manager, you don't need to install this.

#### Windows Prerequisites

* Visual Studio 2019 - Also note that if you prefer to use Visual Studio as the IDE, rather than QtCreator, make sure to also install [Qt VS Tools](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools-19123) extension.

* Qt - install the following kits -> msvc2019 & msvc2019_64 (might work with other Qt versions that uses [QtMsBuild](https://www.qt.io/blog/2018/01/24/qt-visual-studio-new-approach-based-msbuild) new approach).

#### Linux Prerequisites

TODO: Write docs.

#### MacOS Prerequisites

TODO: Write docs.



### Building the project

#### Build on Windows

##### Building using CMake

Once you have Qt installed, just like the `Developer Command Prompt for VS` to setup environment variables to the terminal, Qt provides their own environment tools in:

`%AppData%\Microsoft\Windows\Start Menu\Programs\Qt\5.15.0`

Start by launching `Qt 5.15.0 (MSVC 2019 32-bit)` and then run the following commands in the root directory of the project:

```sh
mkdir build-x86 && cd build-x86
cmake .. -G "Visual Studio 16 2019" -A Win32 -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release -- /M
windeployqt .\bin\
```

For x64 builds, launch `Qt 5.15.0 (MSVC 2019 64-bit)` and then:

```sh
mkdir build-x64 && cd build-x64
cmake .. -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release -- /M
windeployqt .\bin\
```

Note that you can launch the generated solution file from the terminal. That way it will pass all the environment variables to IDE, without having to manually change any environment variables in your system.

#### Build on Linux

```sh
mkdir build && cd build
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release -- -j
```
Note that you can specify how many cores to use when building using the `-j` flag. i.e. for 8 cores `-j8`.

#### Build on MacOS

```sh
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release -- -j
```

Note that you can specify how many cores to use when building using the `-j` flag. i.e. for 8 cores `-j8`.

#### Build using QtCreator

This method should work on all platforms, however, note that adding MacOS & Linux support is still work in progress.

In QtCreator you need to make sure to properly [configure your Qt-Kits](https://doc.qt.io/qtcreator/creator-targets.html). You can then load the project using the `CMakeLists.txt` file in the root directory of the project.

If everything went right, you should be able to compile and debug the project.