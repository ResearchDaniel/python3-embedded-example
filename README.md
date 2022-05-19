# python3-embedded-example
Example of how to embedd Python3 into a C++ application using CMake.
Most Python embedding examples are too simplistic and do not show how to deploy, in other words make a self-contained installer, an application. 

This example shows how to:
1. Configure Python dependencies for one library and one executable using CMake and vcpkg.
2. Initialize Python in isolated mode to avoid using system installed Python.
3. Execute python code within the application.
4. Import Python modules both defined internally (in cpp files) and externally (in Python files) and use them in C++. 
5. Package Python with the application including modules installed via pip.
6. Build and test the installer on Windows/Mac/Linux using Github workflows.



