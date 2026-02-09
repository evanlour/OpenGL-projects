# SHOWCASE 21

add_executable(Showcase21 Camera.h functions.h showcase21.cpp)
set_target_properties(Showcase21 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Showcase21"
)

# REMOVE CMD
if(MSVC)
    set_target_properties(Showcase21 PROPERTIES 
        LINK_FLAGS "/ENTRY:mainCRTStartup"
    )
endif()

# LINK LIBRARIES
target_link_libraries(Showcase21 PRIVATE glew glfw opengl32 imgui)
target_include_directories(Showcase21 PRIVATE    
    glew
    glfw
    imgui
)

set(SHADERS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(SHADERS_DEST "$<TARGET_FILE_DIR:Showcase21>/res")

# The Command
add_custom_command(TARGET Showcase21 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SHADERS_SRC}
    ${SHADERS_DEST}
    COMMENT "Copying shaders to output directory..."
)

# SHOWCASE 22

add_executable(Showcase22 functions.h showcase22.cpp)
set_target_properties(Showcase22 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Showcase22"
)

# REMOVE CMD
if(MSVC)
    set_target_properties(Showcase22 PROPERTIES 
        LINK_FLAGS "/ENTRY:mainCRTStartup"
    )
endif()

# LINK LIBRARIES
target_link_libraries(Showcase22 PRIVATE glew glfw opengl32 imgui)
target_include_directories(Showcase22 PRIVATE    
    glew
    glfw
    imgui
)

set(SHADERS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(SHADERS_DEST "$<TARGET_FILE_DIR:Showcase22>/res")

# The Command
add_custom_command(TARGET Showcase22 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SHADERS_SRC}
    ${SHADERS_DEST}
    COMMENT "Copying shaders to output directory..."
)

# SHOWCASE 23

add_executable(Showcase23 Camera.h functions.h showcase23.cpp)
set_target_properties(Showcase23 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Showcase23"
)

# REMOVE CMD
if(MSVC)
    set_target_properties(Showcase23 PROPERTIES 
        LINK_FLAGS "/ENTRY:mainCRTStartup"
    )
endif()

# LINK LIBRARIES
target_link_libraries(Showcase23 PRIVATE glew glfw glm opengl32 imgui)
target_include_directories(Showcase23 PRIVATE    
    glew
    glfw
    glm
    imgui
)

set(SHADERS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(SHADERS_DEST "$<TARGET_FILE_DIR:Showcase23>/res")

# The Command
add_custom_command(TARGET Showcase23 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SHADERS_SRC}
    ${SHADERS_DEST}
    COMMENT "Copying shaders to output directory..."
)

# SHOWCASE 24

add_executable(Showcase24 Camera.h functions.h showcase24.cpp)
set_target_properties(Showcase24 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/Showcase24"
)

# REMOVE CMD
if(MSVC)
    set_target_properties(Showcase24 PROPERTIES 
        LINK_FLAGS "/ENTRY:mainCRTStartup"
    )
endif()

# LINK LIBRARIES
target_link_libraries(Showcase24 PRIVATE glew glfw glm opengl32 imgui)
target_include_directories(Showcase24 PRIVATE    
    glew
    glfw
    glm
    imgui
)

set(SHADERS_SRC "${CMAKE_CURRENT_SOURCE_DIR}/res")
set(SHADERS_DEST "$<TARGET_FILE_DIR:Showcase24>/res")

# The Command
add_custom_command(TARGET Showcase24 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SHADERS_SRC}
    ${SHADERS_DEST}
    COMMENT "Copying shaders to output directory..."
)