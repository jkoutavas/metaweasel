Last updated February 4th, 2018

| master                   |
|:-------------------------|
| [![Build Status][1]][2]  |

[1]: https://dev.azure.com/HeynowSoftware/metaweasel/_apis/build/status/metaweasel?branchName=master
[2]: https://dev.azure.com/HeynowSoftware/metaweasel/_build?definitionId=3

# Introduction

MetaWeasel lets you view exif metadata on images and remove Exif.Image.ImageDescription tags from them, a common task that my underwater photography friend has to deal with manually, so I made him a utility for it. MetaWeasel is based on the excellent Exiv cross-platform library http://www.exiv2.org/index.html and builds on macOS and Windows using the JUCE application framework. Learn about JUCE here: http://www.juce.com

# How To Build

There are a couple of build dependencies you have to setup before you can build for your OS.

## macOS build prerequisites

For macOS builds, you'll need Sierra with XCode 9.2 installed, or High Sierra with Xcode 9.4.1 installed.

Download the .zip of JUCE v5.3.2 from https://github.com/WeAreROLI/JUCE/releases/tag/5.3.2. Unpack the zip and place it at `~/JUCE`

We use homebrew (brew) to install our external softare and tool dependencies. Install brew using this command:
````
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
````
You can read more about brew here: http://brew.sh

Then install MetaWeasel's C++ dependencies with this command:
````
brew install exiv2
````

## Windows build prerequisites

To date we've been doing builds on Windows 10 with Visual Studio 2017 installed. You'll need to ensure that the "Desktop development with C++" and the latest "Windows 10 SDK for Desktop C++" features are installed and also select the "Windows 10 SDK (10.0.16299.0) for Desktop C++ [x86 and x64]
Use the "Developer Command Prompt for VS2017" any time this README calls for using a Windows command prompt. It properly sets up the paths to the "msbuild" command.

Download the .zip of JUCE v5.4.1 from https://github.com/WeAreROLI/JUCE/releases/tag/5.4.1. Unpack the zip and place it at `C:\tools\JUCE`

Then from a command prompt, do the following:
````
cd \tools
git clone https://github.com/Microsoft/vcpkg vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
````

Then install MetaWeasle's C++ dependencies with this command:
````
vcpkg install --triplet x86-windows exiv2
````

## Doing the build

You can hand-build MetaWeasel by opening `{clone-root/common/MetaWeasel/MetalWeasel.jucer}` in Projucer and then generating the Xcode or Visual Studio 2017 IDE project, and building that. That is the normal path for doing development work on the project, using the Debug configuration. 

Or, you can forego using Projucer and even Xccode and Visual Studio, using CMake and FRUT (https://github.com/McMartin/FRUT). Here's how that's invoked on macOS:
Installed FRUT:
```
cd \tools
git clone https://github.com/McMartin/FRUT.git FRUT
cd FRUT
mkdir build && cd build/
cmake .. -DCMAKE_INSTALL_PREFIX=../prefix -DJUCE_ROOT=\tools\JUCE
cmake --build . --target install
```
Build the app:
```
cd {clone-root}/common/MetaWeasel
mkdir mac_build; cd mac_build
cmake .. -G"Unix Makefiles" -DMetaWeasel_jucer_FILE=../MetaWeasel.jucer
cmake -DCMAKE_BUILD_TYPE=Release --build .
cmake --build .
```

And here's how a build is done on Windows:
```
cd {clone-root}\common\MetaWeasel
mkdir win_build; cd win_build
cmake .. -G"NMake Makefiles" -DMetaWeasel_jucer_FILE=..\MetaWeasel.jucer
cmake -DCMAKE_BUILD_TYPE=Release --build .
cmake --build .
```

If you need to generate a new CMakeLists.txt:
```
cd ~/dev/repos/heynow/metaweasel/common
$HOME/FRUT/prefix/FRUT/bin/Jucer2Reprojucer MetaWeasel.jucer $HOME/FRUT/prefix/FRUT/cmake/Reprojucer.cmake
```
Note that there has been edits committed to for the original CMakeLists.txt file
