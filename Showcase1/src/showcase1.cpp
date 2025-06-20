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
const std::string WINDOW_NAME = "Exercise_1";

GLfloat default_colors1[] = {0.8f, 0.7f, 0.0f};
GLfloat default_colors2[] = {0.9f, 0.8f, 0.7f};
GLfloat default_colors1_og[] = {
    0.8f, 0.0f, 0.0f,
    0.0f, 0.7f, 0.0f,
    0.0f, 0.0f, 0.0f};
GLfloat default_colors2_og[] = {
    0.9f, 0.0f, 0.0f,
    0.0f, 0.8f, 0.0f,
    0.0f, 0.0f, 0.7f};

GLfloat vertices1[] = {
    -0.7f, 0.7f, 0.0f,
    0.3f, 0.7f, 0.0f,
    -0.7f, -0.7f, 0.0f,
};
GLfloat vertices2[] = {
    0.7f, 0.7f, 0.0f,
    0.7f, -0.7f, 0.0f,
    -0.3f, -0.7f, 0.0f
};

bool alternative_fragments = true;
bool polygon_mode = false;
bool alternative_colors = false;
bool tab_flag = false;
bool caps_flag = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* initiate();
void terminate(GLFWwindow* window);
void process_input(GLFWwindow* window);
std::string read_shader(const char* path);
GLuint compile_shader(GLenum type, const char* source);
GLuint create_shader_program(const char* vertex_path, const char* fragment_path);
GLuint create_triangle_VAO(float* vertices, int size);
GLuint create_triangle_VAO_OG(float* vertices, int size, float* colors, int color_size);

int main(){
    float t;
    GLFWwindow* window = initiate();
    GLuint program;
    if(alternative_fragments){
        program = create_shader_program("../res/vertex_shader.glsl", "../res/fragment_shader.glsl");
    }else{
        program = create_shader_program("../res/vertex_shader2.glsl", "../res/fragment_shader2.glsl");
    }
    GLuint triangle1_VAO;
    GLuint triangle2_VAO;
    if(alternative_fragments){
        triangle1_VAO = create_triangle_VAO(vertices1, sizeof(vertices1));
        triangle2_VAO = create_triangle_VAO(vertices2, sizeof(vertices2));
    }else{
        triangle1_VAO = create_triangle_VAO_OG(vertices1, sizeof(vertices1), default_colors1_og, sizeof(default_colors1_og));
        triangle2_VAO = create_triangle_VAO_OG(vertices2, sizeof(vertices2), default_colors2_og, sizeof(default_colors2_og));
    }

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        process_input(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glUseProgram(program);
        glBindVertexArray(triangle1_VAO);
        GLint color_location;
        if(alternative_fragments){
            color_location = glGetUniformLocation(program, "color");
            if(alternative_colors){
                t = glfwGetTime();
                glUniform3f(color_location, 0.5f + 0.5 * cos(3 * t), 0.5f - 0.5 * sin(2 * t), 0.15f);
            }else{
                glUniform3fv(color_location, 1, default_colors1);
            }
        } 
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(triangle2_VAO);
        if(alternative_fragments){
            if(alternative_colors){
                t = glfwGetTime();
                glUniform3f(color_location, 0.6f + 0.4 * cos(1 * t), 0.5f - 0.5 * sin(0.5 * t), 0.5f);
            }else{
                glUniform3fv(color_location, 1, default_colors2);
            }
        }
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
    }
    terminate(window);
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

GLFWwindow* initiate(){
    if(!glfwInit()){
        std::cout << "GLFW could not be initialized! Terminating...\n";
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_TARGET_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_TARGET_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

void terminate(GLFWwindow* window){
    glfwDestroyWindow(window);
    glfwTerminate();
}

void process_input(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if(!tab_flag && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
        if(!polygon_mode){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }else{
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        polygon_mode = !polygon_mode;
        tab_flag = true;
    }
    if(!caps_flag && glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS){
        alternative_colors = !alternative_colors;
        caps_flag = true;
    }
    if(tab_flag && glfwGetKey(window, GLFW_KEY_TAB) != GLFW_PRESS){
        tab_flag = false;
    }
    if(caps_flag && glfwGetKey(window, GLFW_KEY_CAPS_LOCK) != GLFW_PRESS){
        caps_flag = false;
    }
}

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

GLuint create_triangle_VAO(float* vertices, int size){
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

GLuint create_triangle_VAO_OG(float* vertices, int size, float* colors, int color_size){
    GLuint VBO_pos, VBO_col, VAO;  
    glGenBuffers(1, &VBO_pos);
    glGenBuffers(1, &VBO_col);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_col);
    glBufferData(GL_ARRAY_BUFFER, color_size, colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return VAO;
}