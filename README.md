# python3-embedded-example
Example of how to embedd Python3 into a C++ application using CMake.
Most Python embedding examples are simplistic and do not show how to make a self-contained installer, in other words deploy an application. 

This example shows how to:
1. Configure Python dependencies for one library and one executable using CMake and the vcpkg package manager.
2. Initialize Python in isolated mode to avoid using system installed Python.
3. Execute python code within the application.
4. Import Python modules both defined internally (in cpp files) and externally (in Python files) and use them in C++. 
5. Package Python with the application including modules installed via pip.
6. Build and test the installer on Windows/Mac/Linux using Github workflows.


## Setup
Clone the repository and its submodules.
- `git clone https://github.com/ResearchDaniel/python3-embedded-example.git --recurse-submodules`

### Generate build system using CMake 
Configure for your platfrom (VCPKG_TARGET_TRIPLET: `x64-windows` or `x64-osx` or `x64-linux`) using one of the provided CMake presets (`msvc-vcpkg` or `xcode-vcpkg` or `ninja-vcpkg`). This part will download and compile depencies using the vcpkg package manager so it can take some time.
```
cmake -S python3-embedded-example -B build 
        -DVCPKG_TARGET_TRIPLET='x64-windows'
        -DCMAKE_BUILD_TYPE=Release
        --preset msvc-vcpkg
```
Where -S \<path-to-source\> and -B \<path-to-build\>

### Build
``` cmake --build build --config Release --parallel --target ALL_BUILD package ```



