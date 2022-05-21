project(python3-interpeter)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

set(PYTHON_VERSION ${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR})
set(PYTHON_VER_NAME python${PYTHON_VERSION})
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 
    # Mac specific:
    # Xcode code signing does not allow us to put Python in the binary directory.
    # Put Python in the Resources folder instead
    set(PY_RUNTIME_DIR "$<TARGET_BUNDLE_CONTENT_DIR:python3-embedded-app>/Resources/python")
    set(PY_INSTALL_DIR "${RESOURCE_INSTALL_PREFIX}/python")
else()
    set(PY_RUNTIME_DIR "$<TARGET_FILE_DIR:python3-embedded-app>/python")
    set(PY_INSTALL_DIR "${RUNTIME_INSTALL_DIR}/python")
endif()


message("Python_FOUND: ${Python3_FOUND}")
message("Python_EXECUTABLE: ${Python3_EXECUTABLE}")
message("Python3_RUNTIME_LIBRARY_DIRS: ${Python3_RUNTIME_LIBRARY_DIRS}")
message("Python3_RUNTIME_LIBRARY_RELEASE: ${Python3_RUNTIME_LIBRARY_RELEASE}")
message("Python3_RUNTIME_LIBRARY_DEBUG: ${Python3_RUNTIME_LIBRARY_DEBUG}")
message("Python3_STDLIB: ${Python3_STDLIB}")
message("Python3_STDARCH: ${Python3_STDARCH}")
message("Python version: ${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}")
get_target_property(py_imported_loc Python3::Python IMPORTED_LOCATION)
message("Python IMPORTED_LOCATION: ${py_imported_loc}")
get_target_property(py_imported_lib Python3::Python IMPORTED_IMPLIB)
message("Python IMPORTED_IMPLIB: ${py_imported_lib}")

# Add source files
set(SOURCE_FILES
    src/pythoninterpreter.cpp
    src/embeddedmoduleexample.cpp
)
set(HEADER_FILES
    include/python3interpreter/embeddedmoduleexample.h
    include/python3interpreter/pythoninterpreter.h
    include/python3interpreter/pythoninterpreterdefines.h
)

set(PY_SCRIPTS 
    PyScripts/calc.py
)

# Create library
add_library(${PROJECT_NAME} 
    ${SOURCE_FILES} 
    ${HEADER_FILES} 
)
target_link_libraries(${PROJECT_NAME} PUBLIC 
    pybind11::embed
)
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
# Set defines to allow for finding Python
target_compile_definitions(${PROJECT_NAME} PUBLIC 
    PYTHON_VERSION_MAJOR=${Python3_VERSION_MAJOR}
    PYTHON_VERSION_MINOR=${Python3_VERSION_MINOR}
    PYTHON_EXECUTABLE_NAME=${Python3_EXECUTABLE}
)
# Define the symbol stating we are using the declspec(dllexport) when
# building on windows
target_compile_definitions(${PROJECT_NAME} PRIVATE "EXPORTING_PYTHON_INTERPRETER")

# Make sure pip exists
execute_process(COMMAND ${Python3_EXECUTABLE} -m ensurepip)
# Install desired modules via pip
# Here, you could use requirements.txt files...
execute_process(COMMAND ${Python3_EXECUTABLE} -m pip install numpy --isolated)

# Copy custom Python files to generated binary directory.
# The Python interpreter must be set to search for files in this directory
foreach(pyFile IN ITEMS ${PY_SCRIPTS})
    add_custom_command(
                TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                "${CMAKE_CURRENT_SOURCE_DIR}/${pyFile}"
                "${PY_RUNTIME_DIR}/${pyFile}")
endforeach()
# Copy Python modules
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${Python3_STDLIB} ${PY_RUNTIME_DIR}/lib/${PYTHON_VER_NAME})
# Copy executable (not necessary if only embedding)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_if_different
                   ${Python3_EXECUTABLE} "${PY_RUNTIME_DIR}/bin/${PYTHON_VER_NAME}")


# Setup files for packaging the installer 
install(FILES ${PY_SCRIPTS} DESTINATION ${PY_INSTALL_DIR}/PyScripts)
install(PROGRAMS ${Python3_EXECUTABLE} DESTINATION "${PY_INSTALL_DIR}/bin/.")
if("${CMAKE_HOST_SYSTEM_NAME}" MATCHES "Windows")
  install(
    FILES "${Python3_RUNTIME_LIBRARY_RELEASE}"
    DESTINATION ${PY_RUNTIME_DIR}/bin
    CONFIGURATIONS Release;RelWithDebInfo;MinSizeRel
  )
  install(
    FILES "${Python3_RUNTIME_LIBRARY_DEBUG}"
    DESTINATION ${PY_RUNTIME_DIR}/bin
    CONFIGURATIONS Debug
  )
endif()


install(
    DIRECTORY ${Python3_STDLIB}
    DESTINATION ${PY_INSTALL_DIR}/lib
    # Exclude unneded files. You may need to adapt this for your project
    PATTERN "__pycache__" EXCLUDE               # * any cache *
    PATTERN "config-${PYTHON_VERSION}m/*.a" EXCLUDE  # static lib
    PATTERN "lib2to3" EXCLUDE                   
    PATTERN "tkinter" EXCLUDE                   
    PATTERN "lib-dynload/_tkinter.*" EXCLUDE    
    PATTERN "idlelib" EXCLUDE                   
    PATTERN "test" EXCLUDE                      
    PATTERN "turtledemo" EXCLUDE               
    PATTERN "turtle.py" EXCLUDE                 
    PATTERN "wininst*.exe" EXCLUDE              # from distutils, avoid malware false positive
  )

# Needed for distutils/pip
install(
    FILES ${Python3_INCLUDE_DIRS}/pyconfig.h
    DESTINATION ${PY_INSTALL_DIR}/include/${PYTHON_VER_NAME}
)

install(TARGETS ${PROJECT_NAME}
            RUNTIME        # DLLs, Exes, 
                DESTINATION ${RUNTIME_INSTALL_DIR}
            BUNDLE         # Targets marked as BUNDLE
                DESTINATION ${BUNDLE_INSTALL_DIR}
            ARCHIVE        # Static libs, .libs 
                DESTINATION ${ARCHIVE_INSTALL_DIR}
            LIBRARY        # Shared libs - DLLs   
                DESTINATION ${LIBRARY_INSTALL_DIR}
            FRAMEWORK      # Targets marked as FRAMEWORK
                DESTINATION ${FRAMEWORK_INSTALL_DIR}
            PUBLIC_HEADER  # Public headers
                DESTINATION ${INCLUDE_INSTALL_DIR}
            RESOURCE       # Resource files
                DESTINATION ${RESOURCE_INSTALL_PREFIX}
        )

