cmake_minimum_required(VERSION 3.20)
project(python3-embedded-example)

# Global configuration
set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set RPATH (relative path) differently depending on build-time and install/package.
# At build time we use full paths to the generatated libraries.
# When packaging, however, we must use relative paths so that the app 
# can be used on other computers.
include(cmake/rpath.cmake)

# Setup install related paths
if(APPLE)
    set(RUNTIME_INSTALL_DIR ${CMAKE_PROJECT_NAME}.app/Contents/MacOS)
    set(BUNDLE_INSTALL_DIR .)
    set(LIBRARY_INSTALL_DIR ${CMAKE_PROJECT_NAME}.app/Contents/MacOS)
    set(ARCHIVE_INSTALL_DIR ${CMAKE_PROJECT_NAME})
    set(FRAMEWORK_INSTALL_DIR ${CMAKE_PROJECT_NAME}.app/Contents/Frameworks)
    set(INCLUDE_INSTALL_DIR include)
    set(RESOURCE_INSTALL_PREFIX ${CMAKE_PROJECT_NAME}.app/Contents/Resources/)
elseif(WIN32)
    set(RUNTIME_INSTALL_DIR bin)
    set(BUNDLE_INSTALL_DIR "not used!!!")
    set(LIBRARY_INSTALL_DIR bin)
    set(ARCHIVE_INSTALL_DIR bin)
    set(FRAMEWORK_INSTALL_DIR "not used!!!")
    set(INCLUDE_INSTALL_DIR include)
    set(RESOURCE_INSTALL_PREFIX "")
else()
    set(RUNTIME_INSTALL_DIR bin)
    set(BUNDLE_INSTALL_DIR "not used!!!")
    set(LIBRARY_INSTALL_DIR lib)
    set(ARCHIVE_INSTALL_DIR lib)
    set(FRAMEWORK_INSTALL_DIR "not used!!!")
    set(INCLUDE_INSTALL_DIR include)
    set(RESOURCE_INSTALL_PREFIX "")
endif()

# Add library that initializes Python
add_subdirectory(python3interpeter)
# Add application that uses above library
add_subdirectory(app)

# Setup installer packaging
set(CPACK_GENERATOR "ZIP")
include(CPack)