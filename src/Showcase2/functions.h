#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <fstream>
#include <sstream>

const int OPENGL_TARGET_MAJOR = 3;
const int OPENGL_TARGET_MINOR = 3;
const int WINDOW_X = 1280;
const int WINDOW_Y = 720;

/**
 * @brief This Function is triggered on resizing the window and is used to receive the new size of the framebuffer.
 * @tparam window The window that this callback is applied.
 * @tparam width The width of the window.
 * @tparam height The height of the window.
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
/**
 * @brief This function is used to initialize GLFW and GLEW as well as setup and return the window in which any graphics will take place.
 */
GLFWwindow* initiate(const std::string WINDOW_NAME){
    if(!glfwInit()){
        std::cout << "GLFW could not be initialized! Terminating...\n";
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_TARGET_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_TARGET_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_X, WINDOW_Y, WINDOW_NAME.c_str(), NULL, NULL);
    if(window == NULL){
        std::cout << "Window could not be initialized! Terminating...\n";
        exit(1);
    }
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK){
        std::cout << "GLEW could not be initialized! Terminating...\n";
        glfwTerminate();
        exit(1);
    }

    glViewport(0, 0, WINDOW_X, WINDOW_Y);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return window;
}
/**
 * @brief This function is used to terminate the window, as well as the GLFW and GLEW instances.
 * @tparam window The window that will be terminated.
 */
void terminate(GLFWwindow* window){
    glfwDestroyWindow(window);
    glfwTerminate();
}
/**
 * @brief This function is used to read a shader file.
 * @tparam path The path where the file is.
 */
std::string read_shader(const char* path){
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error: Shader file '" << path << "' not found or failed to open!\n";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    if (buffer.str().empty()) {
        std::cerr << "Error: Shader file '" << path << "' is empty!" << std::endl;
    }

    return buffer.str();
}
/**
 * @brief This function compiles a given shader with the corresponding type(vertex, fragment etc).
 * @tparam source The source of the shader.
 */
GLuint compile_shader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Shader compilation failed:\n" << infoLog << "\n";
    }

    return shader;
}
/**
 * @brief This function creates and compiles a shader program.
 * @tparam vertex_path The path to the vertex shader.
 * @tparam fragment_path The path to the fragment shader.
 */
GLuint create_shader_program(const char* vertex_path, const char* fragment_path) {
    std::string s1 = read_shader(vertex_path);
    std::string s2 = read_shader(fragment_path);
    const char* vertex_code = s1.c_str();
    const char* fragment_code = s2.c_str();

    GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_code);
    GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_code);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cout << "Shader linking failed:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return program;
}
/**
 * @brief This function creates a VAO with the given vertices. The colors are decided on runtime.
 * @tparam vertices The vertices of the polygon.
 * @tparam size The size, usually sizeof(vertices).
 */
GLuint create_VAO(float* vertices, int size){
    GLuint VBO, VAO;  
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return VAO;
}
/**
 * @brief This function creates a VAO with the given vertices. The colors are decided on creation and are given along with the vertices.
 * @tparam vertices The vertices of the polygon.
 * @tparam size The size, usually sizeof(vertices).
 * @tparam colors The color of the polygon.
 * @tparam color_size The color size, usually sizeof(color_vertices)
 */
GLuint create_color_VAO(float* vertices, int size){
    GLuint VBO, VAO;  
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return VAO;
}