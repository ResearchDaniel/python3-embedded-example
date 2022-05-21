# Set RPATH (relative path) differently depending on build-time and install/package.
# At build time we use full paths to the generatated libraries.
# When packaging, however, we must use relative paths so that the app 
# can be used on other computers.
# See:
# https://gitlab.kitware.com/cmake/community/-/wikis/doc/cmake/RPATH-handling#always-full-rpath
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
# Use full RPATH for the build tree 
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# Don't use the install RPATH when building 
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
# Add the automatically determined parts of the RPATH,
# which point to directories outside the build tree, to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # loader_path is where the executable/library is located.
    # Frameworks are stored in Contents/Frameworks while the executable is in
    # Contents/MacOS
    list(APPEND CMAKE_INSTALL_RPATH "@loader_path/" "@loader_path/../Frameworks")
elseif("${CMAKE_HOST_SYSTEM_NAME}" MATCHES "Linux")
    # Need to escape $ so that CMake does not treat it as a variable
    list(APPEND CMAKE_INSTALL_RPATH "\$ORIGIN" "\$ORIGIN/../lib")
elseif("${CMAKE_HOST_SYSTEM_NAME}" MATCHES "Windows")
    # Unused on Windows
endif()
