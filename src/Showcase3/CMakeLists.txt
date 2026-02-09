add_executable(Showcase3 Camera.h showcase3_functions.h showcase3.cpp)
set_target_properties(Showcase3 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Showcase3"
)

# REMOVE CMD
if(MSVC)
    set_target_properties(Showcase3 PROPERTIES 
        LINK_FLAGS "/ENTRY:mainCRTStartup"
    )
endif()

# LINK LIBRARIES
target_link_libraries(Showcase3 PRIVATE glew glfw glm opengl32 imgui stb_image)
target_include_directories(Showcase1 PRIVATE    
    glew
    glfw
    glm
    imgui
    stb_image
)

set(SHADERS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(SHADERS_DEST "$<TARGET_FILE_DIR:Showcase3>/res")

# The Command
add_custom_command(TARGET Showcase3 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SHADERS_SRC}
    ${SHADERS_DEST}
    COMMENT "Copying shaders to output directory..."
)