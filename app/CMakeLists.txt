project(python3-embedded-app)

set(SOURCE_FILES
    main.cpp
)
# Create application
add_executable(${PROJECT_NAME} MACOSX_BUNDLE 
    ${SOURCE_FILES} 
)
target_link_libraries(${PROJECT_NAME} PUBLIC 
    python3-interpeter
)

# rename the executable to make it consistent with install paths
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})

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


