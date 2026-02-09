#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

const int OPENGL_TARGET_MAJOR = 3;
const int OPENGL_TARGET_MINOR = 3;
const int WINDOW_X = 1280;
const int WINDOW_Y = 720;

/**
 * @brief Callback function for when the window is resized.
 * @param window The window that was resized.
 * @param width The new width of the window.
 * @param height The new height of the window.
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

/**
 * @brief Initiates the GLFW window and OpenGL context.
 * @param WINDOW_NAME The title of the window.
 * @return A pointer to the created GLFWwindow.
 */
GLFWwindow* initiate(const std::string WINDOW_NAME){
    if(!glfwInit()){
        std::cout << "GLFW could not be initialized! Terminating...\n";
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_TARGET_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_TARGET_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
 * @brief Terminates the GLFW window and context.
 * @param window The window to destroy.
 */
void terminate(GLFWwindow* window){
    glfwDestroyWindow(window);
    glfwTerminate();
}

/**
 * @brief Reads a shader file from the given path.
 * @param path The path to the shader file.
 * @return The content of the shader file as a string.
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
 * @brief Compiles a shader from source code.
 * @param type The type of shader (e.g., GL_VERTEX_SHADER, GL_FRAGMENT_SHADER).
 * @param source The source code of the shader.
 * @return The ID of the compiled shader.
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
 * @brief Creates a shader program from vertex and fragment shader paths.
 * @param vertex_path Path to the vertex shader file.
 * @param fragment_path Path to the fragment shader file.
 * @return The ID of the created shader program.
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
 * @brief Sets an integer uniform in the shader program.
 * @param program The shader program ID.
 * @param name The name of the uniform.
 * @param value The integer value to set.
 */
void program_set_1i(GLuint &program, std::string name, int value){
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniform1i(location, value);
}

/**
 * @brief Sets a float uniform in the shader program.
 * @param program The shader program ID.
 * @param name The name of the uniform.
 * @param value The float value to set.
 */
void program_set_1f(GLuint &program, std::string name, float value){
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniform1f(location, value);
}

/**
 * @brief Sets a vec3 uniform in the shader program.
 * @param program The shader program ID.
 * @param name The name of the uniform.
 * @param value1 The first component of the vector.
 * @param value2 The second component of the vector.
 * @param value3 The third component of the vector.
 */
void program_set_3f(GLuint &program, std::string name, float value1, float value2, float value3){
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniform3f(location, value1, value2, value3);

}

/**
 * @brief Sets a mat3 uniform in the shader program.
 * @param program The shader program ID.
 * @param name The name of the uniform.
 * @param value The mat3 value to set.
 */
void program_set_M3fv(GLuint &program, std::string name, glm::mat3 value){
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, &value[0][0]);
}

/**
 * @brief Sets a mat4 uniform in the shader program.
 * @param program The shader program ID.
 * @param name The name of the uniform.
 * @param value The mat4 value to set.
 */
void program_set_M4fv(GLuint &program, std::string name, glm::mat4 value){
    GLuint location = glGetUniformLocation(program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

/**
 * @brief Generates a texture from an image file.
 * @param givenTextureFilePath Path to the image file.
 * @return The ID of the generated texture.
 */
unsigned int generate_texture(const char* givenTextureFilePath)
{
	unsigned int textureId;
	glGenTextures(1, &textureId);

	stbi_set_flip_vertically_on_load(true);

	int imageWidth, imageHeight, numberOfchannels;
	unsigned char* imageData;
	imageData = stbi_load(givenTextureFilePath, &imageWidth, &imageHeight, &numberOfchannels, 0);

	int imageFormat = 3;

	if (numberOfchannels == 1)
		imageFormat = GL_RED;
	else if (numberOfchannels == 3)
		imageFormat = GL_RGB;
	else if (numberOfchannels == 4)
		imageFormat = GL_RGBA;

	if (imageData)
	{
		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, imageWidth, imageHeight, 0, imageFormat, GL_UNSIGNED_BYTE, imageData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
		std::cout << "Could not read the image!!!" << std::endl;

	stbi_image_free(imageData);
	return textureId;
}

/**
 * @brief Calculates tangents and bitangents for a cube mesh.
 * @param verts Pointer to the vertex data.
 * @param vertexCount The number of vertices.
 * @return Pointer to the calculated tangent and bitangent data.
 */
float* calculate_cube_tangents(const float* verts, size_t vertexCount)
{
    // Allocate accumulators
    glm::vec3* tanAccum = new glm::vec3[vertexCount]();
    glm::vec3* bitanAccum = new glm::vec3[vertexCount]();

    // Iterate triangles
    for (size_t i = 0; i + 2 < vertexCount; i += 3)
    {
        const float* v0 = verts + (i + 0) * 8;
        const float* v1 = verts + (i + 1) * 8;
        const float* v2 = verts + (i + 2) * 8;

        glm::vec3 p0(v0[0], v0[1], v0[2]);
        glm::vec3 p1(v1[0], v1[1], v1[2]);
        glm::vec3 p2(v2[0], v2[1], v2[2]);

        glm::vec2 uv0(v0[6], v0[7]);
        glm::vec2 uv1(v1[6], v1[7]);
        glm::vec2 uv2(v2[6], v2[7]);

        glm::vec3 dPos1 = p1 - p0;
        glm::vec3 dPos2 = p2 - p0;
        glm::vec2 dUV1  = uv1 - uv0;
        glm::vec2 dUV2  = uv2 - uv0;

        float denom = dUV1.x * dUV2.y - dUV2.x * dUV1.y;
        float r = denom != 0.0f ? 1.0f / denom : 0.0f;

        glm::vec3 tangent  = (dPos1 * dUV2.y - dPos2 * dUV1.y) * r;
        glm::vec3 bitangent = (dPos2 * dUV1.x - dPos1 * dUV2.x) * r;

        // accumulate for each triangle corner
        for (int j = 0; j < 3; ++j) {
            tanAccum[i + j]    += tangent;
            bitanAccum[i + j]  += bitangent;
        }
    }

    // Output array: [Tx,Ty,Tz, Bx,By,Bz] * vertexCount
    float* out = new float[vertexCount * 6];

    // Orthonormalize and compute final bitangent handedness
    for (size_t i = 0; i < vertexCount; ++i)
    {
        const float* v = verts + i * 8;
        glm::vec3 n(v[3], v[4], v[5]);

        // Gram�Schmidt tangent
        glm::vec3 t = tanAccum[i];
        t = glm::normalize(t - n * glm::dot(n, t));

        // Gram�Schmidt bitangent (optional, but we'll recompute handed bitangent)
        glm::vec3 b = bitanAccum[i];
        // Compute handedness
        float w = (glm::dot(glm::cross(n, t), b) < 0.0f) ? -1.0f : 1.0f;
        // Final bitangent orthogonal to n and t
        glm::vec3 finalB = glm::cross(n, t) * w;

        out[i*6 + 0] = t.x;
        out[i*6 + 1] = t.y;
        out[i*6 + 2] = t.z;
        out[i*6 + 3] = finalB.x;
        out[i*6 + 4] = finalB.y;
        out[i*6 + 5] = finalB.z;
    }

    delete[] tanAccum;
    delete[] bitanAccum;
    return out;
}

/**
 * @brief Base class for light sources.
 */
class light_source{
public:
    bool enabled;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    GLuint VAO;
    GLuint program;
    glm::mat4 model;
    glm::vec3 pos;
    /**
     * @brief Toggles the light on or off.
     * @param status True to enable, false to disable.
     */
    void toggle_light(bool status);
    /**
     * @brief Sets the Vertex Array Object (VAO) for the light source.
     * @param vertices Pointer to the vertex data.
     * @param size Size of the vertex data in bytes.
     */
    void set_VAO(float* vertices, int size);
    /**
     * @brief Sets the shader program for the light source.
     * @param vertex_path Path to the vertex shader.
     * @param fragment_path Path to the fragment shader.
     */
    void set_program(std::string vertex_path, std::string fragment_path);
    /**
     * @brief Renders the light source.
     * @param view The view matrix.
     * @param projection The projection matrix.
     */
    void render(glm::mat4 view, glm::mat4 projection);
private:
};

void light_source::toggle_light(bool status){
    enabled = status;
}

void light_source::set_VAO(float* vertices, int size){
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void light_source::set_program(std::string vertex_path, std::string fragment_path){
    program = create_shader_program(vertex_path.c_str(), fragment_path.c_str());
}

void light_source::render(glm::mat4 view, glm::mat4 projection){
    glUseProgram(program);
    glBindVertexArray(VAO);
    program_set_M4fv(program, "view", view);
    program_set_M4fv(program, "projection", projection);
    program_set_M4fv(program, "model", model);
    if(enabled){
        program_set_1i(program, "active_light", 1);
    }else{
        program_set_1i(program, "active_light", 0);
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glUseProgram(0);
    glBindVertexArray(0);
}

/**
 * @brief Class representing a directional light source.
 */
class directional_light_source : public light_source{
public:
    glm::vec3 direction;
    directional_light_source(){};
    directional_light_source(glm::vec3 ambient_val, glm::vec3 diffuse_val, glm::vec3 specular_val, glm::vec3 direction_val);
    /**
     * @brief Sets the position of the directional light source model.
     * @param new_position The new position vector.
     */
    void set_position(glm::vec3 new_position);
private:
};

directional_light_source::directional_light_source(glm::vec3 ambient_val, glm::vec3 diffuse_val, glm::vec3 specular_val, glm::vec3 direction_val){
    enabled = true;
    ambient = ambient_val;
    diffuse = diffuse_val;
    specular = specular_val;
    direction = direction_val;
    glGenVertexArrays(1, &VAO);
}

void directional_light_source::set_position(glm::vec3 new_position){
    model = glm::translate(glm::mat4(1.0f), new_position);
    pos = new_position;
}

/**
 * @brief Class representing a point light source.
 */
class point_light_source: public light_source{
public:
    float constant;
    float linear;
    float quadratic;
    point_light_source(){};
    point_light_source(glm::vec3 ambient_val, glm::vec3 diffuse_val, glm::vec3 specular_val, float constant_val, float linear_val, float quadratic_val);
    /**
     * @brief Sets the position of the point light source.
     * @param new_position The new position vector.
     */
    void set_position(glm::vec3 new_position);
private:
};

point_light_source::point_light_source(glm::vec3 ambient_val, glm::vec3 diffuse_val, glm::vec3 specular_val, float constant_val, float linear_val, float quadratic_val){
    enabled = true;
    ambient = ambient_val;
    diffuse = diffuse_val;
    specular = specular_val;
    constant = constant_val;
    linear = linear_val;
    quadratic = quadratic_val;
    glGenVertexArrays(1, &VAO);
}

void point_light_source::set_position(glm::vec3 new_position){
    model = glm::translate(glm::mat4(1.0f), new_position);
    pos = new_position;
}

/**
 * @brief Base class for textured cubes.
 */
class textured_cube{
public:
    GLuint VAO;
    GLuint program;
    glm::mat4 model;
    glm::vec3 pos;
    unsigned int texture1;
    unsigned int texture2;
    textured_cube(){};
    /**
     * @brief Sets the position of the cube.
     * @param new_position The new position vector.
     */
    void set_position(glm::vec3 new_position);
    /**
     * @brief Sets the Vertex Array Object (VAO) for the cube.
     * @param vertices Pointer to the vertex data.
     * @param size Size of the vertex data in bytes.
     */
    void set_VAO(float* vertices, int size);
    /**
     * @brief Sets the shader program for the cube.
     * @param vertex_path Path to the vertex shader.
     * @param fragment_path Path to the fragment shader.
     */
    void set_program(std::string vertex_path, std::string fragment_path);
    /**
     * @brief Assigns textures to the cube.
     * @param tex1 ID of the first texture.
     * @param tex2 ID of the second texture.
     */
    void assign_textures(unsigned int tex1, unsigned int tex2);
private:
};

void textured_cube::set_position(glm::vec3 new_position){
    model = glm::translate(glm::mat4(1.0f), new_position);
    pos = new_position;
}

void textured_cube::set_VAO(float* vertices, int size){
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void textured_cube::set_program(std::string vertex_path, std::string fragment_path){
    program = create_shader_program(vertex_path.c_str(), fragment_path.c_str());
}

void textured_cube::assign_textures(unsigned int tex1, unsigned int tex2){
    texture1 = tex1;
    texture2 = tex2;
}

/**
 * @brief Class for a cube with normal mapping and textures.
 */
class normal_textured_cube : public textured_cube{
public:
    /**
     * @brief Renders the cube.
     * @param view The view matrix.
     * @param projection The projection matrix.
     * @param camera_position The position of the camera.
     * @param dir_sources Vector of directional light sources.
     * @param point_sources Vector of point light sources.
     */
    void render(glm::mat4 view, glm::mat4 projection, glm::vec3 camera_position, const std::vector<directional_light_source>& dir_sources, const std::vector<point_light_source>& point_sources);
};

void normal_textured_cube::render(glm::mat4 view, glm::mat4 projection, glm::vec3 camera_position, const std::vector<directional_light_source>& dir_sources, const std::vector<point_light_source>& point_sources){
    glUseProgram(program);
    glBindVertexArray(VAO);
    glm::mat3 normal_transformation = glm::transpose(glm::inverse(glm::mat3(model)));
    program_set_M4fv(program, "view", view);
    program_set_M4fv(program, "projection", projection);
    program_set_M4fv(program, "model", model);
    program_set_M3fv(program, "normal", normal_transformation);
    program_set_3f(program, "camera_position", camera_position.x, camera_position.y, camera_position.z);
    program_set_1i(program, "dir_lights", int(dir_sources.size()));
    program_set_1i(program, "point_lights", int(point_sources.size()));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    program_set_1i(program, "material.ambient_specular_texture", 0);
    program_set_1i(program, "material.diffuse_texture", 1);
    program_set_1f(program, "material.shininess", 64.0f);
    for(int i = 0; i < dir_sources.size(); i++){
        std::string enabled = "dir_sources[" + std::to_string(i) + "].enabled";
        if(i >= 16){break;};
        if(!dir_sources[i].enabled){
            program_set_1i(program, enabled, 0);
            continue;
        }else{
            program_set_1i(program, enabled, 1);
        }
        std::string direction = "dir_sources[" + std::to_string(i) + "].direction";
        std::string ambient_color = "dir_sources[" + std::to_string(i) + "].ambient_color";
        std::string diffuse_color = "dir_sources[" + std::to_string(i) + "].diffuse_color";
        std::string specular_color = "dir_sources[" + std::to_string(i) + "].specular_color";
        program_set_3f(program, direction, dir_sources[i].direction.x, dir_sources[i].direction.y, dir_sources[i].direction.z);
        program_set_3f(program, ambient_color, dir_sources[i].ambient.x, dir_sources[i].ambient.y, dir_sources[i].ambient.z);
        program_set_3f(program, diffuse_color, dir_sources[i].diffuse.x, dir_sources[i].diffuse.y, dir_sources[i].diffuse.z);
        program_set_3f(program, specular_color, dir_sources[i].specular.x, dir_sources[i].specular.y, dir_sources[i].specular.z);
    }
    for(int i = 0; i < point_sources.size(); i++){
        std::string enabled = "point_sources[" + std::to_string(i) + "].enabled";
        if(i >= 16){break;};
        if(!point_sources[i].enabled){
            program_set_1i(program, enabled, 0);
            continue;
        }else{
            program_set_1i(program, enabled, 1);
        }
        std::string position = "point_sources[" + std::to_string(i) + "].position";
        std::string ambient_color = "point_sources[" + std::to_string(i) + "].ambient_color";
        std::string diffuse_color = "point_sources[" + std::to_string(i) + "].diffuse_color";
        std::string specular_color = "point_sources[" + std::to_string(i) + "].specular_color";
        std::string constant = "point_sources[" + std::to_string(i) + "].constant";
        std::string linear = "point_sources[" + std::to_string(i) + "].linear";
        std::string quadratic = "point_sources[" + std::to_string(i) + "].quadratic";
        program_set_3f(program, position, point_sources[i].pos.x, point_sources[i].pos.y, point_sources[i].pos.z);
        program_set_3f(program, ambient_color, point_sources[i].ambient.x, point_sources[i].ambient.y, point_sources[i].ambient.z);
        program_set_3f(program, diffuse_color, point_sources[i].diffuse.x, point_sources[i].diffuse.y, point_sources[i].diffuse.z);
        program_set_3f(program, specular_color, point_sources[i].specular.x, point_sources[i].specular.y, point_sources[i].specular.z);
        program_set_1f(program, constant, point_sources[i].constant);
        program_set_1f(program, linear, point_sources[i].linear);
        program_set_1f(program, quadratic, point_sources[i].quadratic);
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glUseProgram(0);
    glBindVertexArray(0);
}

/**
 * @brief Class for a cube with mixed textures.
 */
class mixed_textured_cube : public textured_cube{
public:
    /**
     * @brief Renders the mixed textured cube.
     * @param view The view matrix.
     * @param projection The projection matrix.
     * @param camera_position The position of the camera.
     * @param mix_percentage The percentage to mix the textures.
     * @param dir_sources Vector of directional light sources.
     * @param point_sources Vector of point light sources.
     */
    void render(glm::mat4 view, glm::mat4 projection, glm::vec3 camera_position, float mix_percentage, const std::vector<directional_light_source>& dir_sources, const std::vector<point_light_source>& point_sources);
};

void mixed_textured_cube::render(glm::mat4 view, glm::mat4 projection, glm::vec3 camera_position, float mix_percentage, const std::vector<directional_light_source>& dir_sources, const std::vector<point_light_source>& point_sources){
    glUseProgram(program);
    glBindVertexArray(VAO);
    glm::mat3 normal_transformation = glm::transpose(glm::inverse(glm::mat3(model)));
    program_set_M4fv(program, "view", view);
    program_set_M4fv(program, "projection", projection);
    program_set_M4fv(program, "model", model);
    program_set_M3fv(program, "normal", normal_transformation);
    program_set_3f(program, "camera_position", camera_position.x, camera_position.y, camera_position.z);
    program_set_1i(program, "dir_lights", int(dir_sources.size()));
    program_set_1i(program, "point_lights", int(point_sources.size()));
    program_set_1f(program, "mix_percentage", mix_percentage);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    program_set_1i(program, "material.ambient_specular_texture", 0);
    program_set_1i(program, "material.diffuse_texture", 1);
    program_set_1f(program, "material.shininess", 64.0f);
    for(int i = 0; i < dir_sources.size(); i++){
        std::string enabled = "dir_sources[" + std::to_string(i) + "].enabled";
        if(i >= 16){break;};
        if(!dir_sources[i].enabled){
            program_set_1i(program, enabled, 0);
            continue;
        }else{
            program_set_1i(program, enabled, 1);
        }
        std::string direction = "dir_sources[" + std::to_string(i) + "].direction";
        std::string ambient_color = "dir_sources[" + std::to_string(i) + "].ambient_color";
        std::string diffuse_color = "dir_sources[" + std::to_string(i) + "].diffuse_color";
        std::string specular_color = "dir_sources[" + std::to_string(i) + "].specular_color";
        program_set_3f(program, direction, dir_sources[i].direction.x, dir_sources[i].direction.y, dir_sources[i].direction.z);
        program_set_3f(program, ambient_color, dir_sources[i].ambient.x, dir_sources[i].ambient.y, dir_sources[i].ambient.z);
        program_set_3f(program, diffuse_color, dir_sources[i].diffuse.x, dir_sources[i].diffuse.y, dir_sources[i].diffuse.z);
        program_set_3f(program, specular_color, dir_sources[i].specular.x, dir_sources[i].specular.y, dir_sources[i].specular.z);
    }
    for(int i = 0; i < point_sources.size(); i++){
        std::string enabled = "point_sources[" + std::to_string(i) + "].enabled";
        if(i >= 16){break;};
        if(!point_sources[i].enabled){
            program_set_1i(program, enabled, 0);
            continue;
        }else{
            program_set_1i(program, enabled, 1);
        }
        std::string position = "point_sources[" + std::to_string(i) + "].position";
        std::string ambient_color = "point_sources[" + std::to_string(i) + "].ambient_color";
        std::string diffuse_color = "point_sources[" + std::to_string(i) + "].diffuse_color";
        std::string specular_color = "point_sources[" + std::to_string(i) + "].specular_color";
        std::string constant = "point_sources[" + std::to_string(i) + "].constant";
        std::string linear = "point_sources[" + std::to_string(i) + "].linear";
        std::string quadratic = "point_sources[" + std::to_string(i) + "].quadratic";
        program_set_3f(program, position, point_sources[i].pos.x, point_sources[i].pos.y, point_sources[i].pos.z);
        program_set_3f(program, ambient_color, point_sources[i].ambient.x, point_sources[i].ambient.y, point_sources[i].ambient.z);
        program_set_3f(program, diffuse_color, point_sources[i].diffuse.x, point_sources[i].diffuse.y, point_sources[i].diffuse.z);
        program_set_3f(program, specular_color, point_sources[i].specular.x, point_sources[i].specular.y, point_sources[i].specular.z);
        program_set_1f(program, constant, point_sources[i].constant);
        program_set_1f(program, linear, point_sources[i].linear);
        program_set_1f(program, quadratic, point_sources[i].quadratic);
    }
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glUseProgram(0);
    glBindVertexArray(0);
}

/**
 * @brief Class for a cube with normal mapping.
 */
class normal_map_cube : public textured_cube{
public:
    /**
     * @brief Sets the VAO for the normal map cube.
     * @param vertices Pointer to vertex data.
     * @param size Size of vertex data.
     */
    void set_normal_map_VAO(float* vertices, int size);
    /**
     * @brief Renders the normal map cube.
     * @param view The view matrix.
     * @param projection The projection matrix.
     * @param camera_position The position of the camera.
     * @param dir_sources Vector of directional light sources.
     * @param point_sources Vector of point light sources.
     */
    void render(glm::mat4 view, glm::mat4 projection, glm::vec3 camera_position, const std::vector<directional_light_source>& dir_sources, const std::vector<point_light_source>& point_sources);
};

void normal_map_cube::set_normal_map_VAO(float* vertices, int size){
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void normal_map_cube::render(glm::mat4 view, glm::mat4 projection, glm::vec3 camera_position, const std::vector<directional_light_source>& dir_sources, const std::vector<point_light_source>& point_sources){
    glUseProgram(program);
    glBindVertexArray(VAO);
    program_set_M4fv(program, "view", view);
    program_set_M4fv(program, "projection", projection);
    program_set_M4fv(program, "model", model);
    program_set_3f(program, "camera_position", camera_position.x, camera_position.y, camera_position.z);
    program_set_1i(program, "dir_lights", int(dir_sources.size()));
    program_set_1i(program, "point_lights", int(point_sources.size()));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    program_set_1i(program, "material.diffuse_map", 0);
    program_set_1i(program, "material.normal_map", 1);

    for(int i = 0; i < dir_sources.size(); i++){
        std::string enabled = "lightDirEnabled[" + std::to_string(i) + "]";
        if(i >= 16){break;};
        if(!dir_sources[i].enabled){
            program_set_1i(program, enabled, 0);
            continue;
        }else{
            program_set_1i(program, enabled, 1);
        }
        std::string dir_light = "lightSourceDirection[" + std::to_string(i) + "]";
        program_set_3f(program, dir_light, dir_sources[i].direction.x, dir_sources[i].direction.y, dir_sources[i].direction.z);
    }
    for(int i = 0; i < point_sources.size(); i++){
        std::string enabled = "lightPointEnabled[" + std::to_string(i) + "]";
        if(i >= 16){break;};
        if(!point_sources[i].enabled){
            program_set_1i(program, enabled, 0);
            continue;
        }else{
            program_set_1i(program, enabled, 1);
        }
        std::string point_light = "lightSourcePosition[" + std::to_string(i) + "]";
        program_set_3f(program, point_light, point_sources[i].pos.x, point_sources[i].pos.y, point_sources[i].pos.z);
    }

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glUseProgram(0);
    glBindVertexArray(0);
}

/**
 * @brief Class representing a quad object.
 */
class quad_object{
public:
    glm::vec3 pos1, pos2, pos3, pos4, center;
    glm::vec2 uv1, uv2, uv3, uv4;
    unsigned int texture1, texture2;
    GLuint VAO;
    GLuint program;
    glm::mat4 model;
    quad_object(){};
    /**
     * @brief Sets the position of the quad vertices and center.
     * @param val1 Position of vertex 1.
     * @param val2 Position of vertex 2.
     * @param val3 Position of vertex 3.
     * @param val4 Position of vertex 4.
     * @param cent Center position of the quad.
     */
    void set_position(glm::vec3 val1, glm::vec3 val2, glm::vec3 val3, glm::vec3 val4, glm::vec3 cent);
    /**
     * @brief Assigns textures to the quad.
     * @param tex1 ID of the first texture.
     * @param tex2 ID of the second texture.
     */
    void assign_textures(unsigned int tex1, unsigned int tex2);
    /**
     * @brief Sets the UV coordinates for the quad vertices.
     * @param val1 UV coordinate for vertex 1.
     * @param val2 UV coordinate for vertex 2.
     * @param val3 UV coordinate for vertex 3.
     * @param val4 UV coordinate for vertex 4.
     */
    void set_coordinates(glm::vec2 val1, glm::vec2 val2, glm::vec2 val3, glm::vec2 val4);
    /**
     * @brief Sets up the Vertex Array Object (VAO) for the quad.
     */
    void set_VAO();
    /**
     * @brief Sets the shader program for the quad.
     * @param vertex_path Path to the vertex shader.
     * @param fragment_path Path to the fragment shader.
     */
    void set_program(std::string vertex_path, std::string fragment_path);
    /**
     * @brief Renders the quad.
     * @param view The view matrix.
     * @param projection The projection matrix.
     * @param camera_position The position of the camera.
     * @param dir_sources Vector of directional light sources.
     * @param point_sources Vector of point light sources.
     */
    void render(glm::mat4 view, glm::mat4 projection, glm::vec3 camera_position, const std::vector<directional_light_source>& dir_sources, const std::vector<point_light_source>& point_sources);
};

void quad_object::set_position(glm::vec3 val1, glm::vec3 val2, glm::vec3 val3, glm::vec3 val4, glm::vec3 cent){
    pos1 = val1;
    pos2 = val2;
    pos3 = val3;
    pos4 = val4;
    center = cent;
    model = glm::translate(glm::mat4(1.0f), cent);
}
void quad_object::set_coordinates(glm::vec2 val1, glm::vec2 val2, glm::vec2 val3, glm::vec2 val4){
    uv1 = val1;
    uv2 = val2;
    uv3 = val3;
    uv4 = val4;
}
void quad_object::set_VAO(){
    // normal vector
    glm::vec3 nm(0.0f, 0.0f, 1.0f);

    // calculate tangent/bitangent vectors of both triangles
    glm::vec3 tangent1, bitangent1;
    glm::vec3 tangent2, bitangent2;
    // triangle 1
    // ----------
    glm::vec3 edge1 = pos2 - pos1;
    glm::vec3 edge2 = pos3 - pos1;
    glm::vec2 deltaUV1 = uv2 - uv1;
    glm::vec2 deltaUV2 = uv3 - uv1;

    float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

    bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

    // triangle 2
    // ----------
    edge1 = pos3 - pos1;
    edge2 = pos4 - pos1;
    deltaUV1 = uv3 - uv1;
    deltaUV2 = uv4 - uv1;

    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

    tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
    tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
    tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


    bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
    bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
    bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


    float vertices[] = {
        // positions            // normal         // texcoords  // tangent                          // bitangent
        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
        pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
    };
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void quad_object::render(glm::mat4 view, glm::mat4 projection, glm::vec3 camera_position, const std::vector<directional_light_source>& dir_sources, const std::vector<point_light_source>& point_sources){
    glUseProgram(program);
    glBindVertexArray(VAO);
    program_set_M4fv(program, "view", view);
    program_set_M4fv(program, "projection", projection);
    program_set_M4fv(program, "model", model);
    program_set_3f(program, "camera_position", camera_position.x, camera_position.y, camera_position.z);
    program_set_1i(program, "dir_lights", int(dir_sources.size()));
    program_set_1i(program, "point_lights", int(point_sources.size()));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    program_set_1i(program, "material.diffuse_map", 0);
    program_set_1i(program, "material.normal_map", 1);

    for(int i = 0; i < dir_sources.size(); i++){
        std::string enabled = "lightDirEnabled[" + std::to_string(i) + "]";
        if(i >= 16){break;};
        if(!dir_sources[i].enabled){
            program_set_1i(program, enabled, 0);
            continue;
        }else{
            program_set_1i(program, enabled, 1);
        }
        std::string dir_light = "lightSourceDirection[" + std::to_string(i) + "]";
        program_set_3f(program, dir_light, dir_sources[i].direction.x, dir_sources[i].direction.y, dir_sources[i].direction.z);
    }
    for(int i = 0; i < point_sources.size(); i++){
        std::string enabled = "lightPointEnabled[" + std::to_string(i) + "]";
        if(i >= 16){break;};
        if(!point_sources[i].enabled){
            program_set_1i(program, enabled, 0);
            continue;
        }else{
            program_set_1i(program, enabled, 1);
        }
        std::string point_light = "lightSourcePosition[" + std::to_string(i) + "]";
        program_set_3f(program, point_light, point_sources[i].pos.x, point_sources[i].pos.y, point_sources[i].pos.z);
    }

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUseProgram(0);
    glBindVertexArray(0);
}

void quad_object::set_program(std::string vertex_path, std::string fragment_path){
    program = create_shader_program(vertex_path.c_str(), fragment_path.c_str());
}

void quad_object::assign_textures(unsigned int tex1, unsigned int tex2){
    texture1 = tex1;
    texture2 = tex2;
}

/**
 * @brief Class representing a simple quad.
 */
class simple_quad : public quad_object{
public:
    /**
     * @brief Sets up the simple VAO for the quad.
     */
    void set_simple_VAO();
    /**
     * @brief Renders the simple quad.
     * @param view The view matrix.
     * @param projection The projection matrix.
     */
    void simple_render(glm::mat4 view, glm::mat4 projection);
};

void simple_quad::set_simple_VAO(){
    // normal vector
    glm::vec3 nm(0.0f, 0.0f, 1.0f);

    float vertices[] = {
        // positions            // normal         // texcoords
        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y,
        pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y,

        pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y,
        pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y,
        pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y
    };
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void simple_quad::simple_render(glm::mat4 view, glm::mat4 projection){
    glUseProgram(program);
    glBindVertexArray(VAO);
    program_set_M4fv(program, "view", view);
    program_set_M4fv(program, "projection", projection);
    program_set_M4fv(program, "model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glUseProgram(0);
    glBindVertexArray(0);
}