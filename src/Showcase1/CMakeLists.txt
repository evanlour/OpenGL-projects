add_executable(Showcase1 showcase1.cpp)
set_target_properties(Showcase1 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Showcase1"
)

# REMOVE CMD
if(MSVC)
    set_target_properties(Showcase1 PROPERTIES 
        LINK_FLAGS "/ENTRY:mainCRTStartup"
    )
endif()

# LINK LIBRARIES
target_link_libraries(Showcase1 PRIVATE OpenGL::GL glew glfw)
target_include_directories(Showcase1 PRIVATE    
    glew
    glfw
)

set(SHADERS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(SHADERS_DEST "$<TARGET_FILE_DIR:Showcase1>/res")

# The Command
add_custom_command(TARGET Showcase1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SHADERS_SRC}
    ${SHADERS_DEST}
    COMMENT "Copying shaders to output directory..."
)
