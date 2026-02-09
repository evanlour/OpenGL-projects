#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "showcase3_functions.h"
#include "Camera.h"
#include <cstdlib>

Camera camera(glm::vec3(20.0f, 10.0f, 20.0f));
static bool first_mouse = true;
bool cursor_enabled = false;
static bool space_pressed = false;
static float lastX = (float)WINDOW_X / 2.0f;
static float lastY = (float)WINDOW_Y / 2.0f;

/**
 * @brief Processes keyboard input for the application.
 * @param window The GLFW window.
 * @param frame_time The time elapsed since the last frame.
 */
void process_keyboard_input(GLFWwindow* window, float frame_time);

/**
 * @brief Processes mouse movement input.
 * @param window The GLFW window.
 * @param givenMousePositionX The X position of the mouse.
 * @param givenMousePositionY The Y position of the mouse.
 */
void process_mouse_input(GLFWwindow* window, double givenMousePositionX, double givenMousePositionY);

/**
 * @brief Processes mouse scroll input.
 * @param givenWindow The GLFW window.
 * @param givenScrollOffsetX The scroll offset in the X direction.
 * @param givenScrollOffsetY The scroll offset in the Y direction.
 */
void process_scroll_input(GLFWwindow* givenWindow, double givenScrollOffsetX, double givenScrollOffsetY);

/**
 * @brief Adds a random item (light or cube) to the scene.
 */
void add_random_item();

/**
 * @brief Moves a cube based on its position relative to the matrix floor.
 * @param model The model matrix of the cube.
 * @param position The position vector of the cube.
 * @param frame_time The time elapsed since the last frame.
 * @param matrix_position The position of the matrix floor center.
 * @param current_location The index of the cube in its respective vector.
 * @param option The type of object being moved (0: point light, 1: normal cube, 2: mixed cube, 3: normal map cube).
 */
void move_cube(glm::mat4& model, glm::vec3& position, float frame_time, glm::vec3 matrix_position, int current_location, int option);

GLfloat light_source_vertices[] =
{
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,


    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

float texture_cube_vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

float normal_map_vertices[] = {
    -0.500000f, -0.500000f, -0.500000f, 0.000000f, 0.000000f, -1.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, -0.000000f, 1.000000f, -0.000000f,
    0.500000f, -0.500000f, -0.500000f, 0.000000f, 0.000000f, -1.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, -0.000000f, 1.000000f, -0.000000f,
    0.500000f, 0.500000f, -0.500000f, 0.000000f, 0.000000f, -1.000000f, 1.000000f, 1.000000f, 1.000000f, 0.000000f, 0.000000f, -0.000000f, 1.000000f, -0.000000f,
    0.500000f, 0.500000f, -0.500000f, 0.000000f, 0.000000f, -1.000000f, 1.000000f, 1.000000f, 1.000000f, 0.000000f, 0.000000f, -0.000000f, 1.000000f, -0.000000f,
    -0.500000f, 0.500000f, -0.500000f, 0.000000f, 0.000000f, -1.000000f, 0.000000f, 1.000000f, 1.000000f, 0.000000f, 0.000000f, -0.000000f, 1.000000f, -0.000000f,
    -0.500000f, -0.500000f, -0.500000f, 0.000000f, 0.000000f, -1.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, -0.000000f, 1.000000f, -0.000000f,
    -0.500000f, -0.500000f, 0.500000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f,
    0.500000f, -0.500000f, 0.500000f, 0.000000f, 0.000000f, 1.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f,
    0.500000f, 0.500000f, 0.500000f, 0.000000f, 0.000000f, 1.000000f, 1.000000f, 1.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f,
    0.500000f, 0.500000f, 0.500000f, 0.000000f, 0.000000f, 1.000000f, 1.000000f, 1.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f,
    -0.500000f, 0.500000f, 0.500000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 1.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f,
    -0.500000f, -0.500000f, 0.500000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f,
    -0.500000f, 0.500000f, 0.500000f, -1.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, -1.000000f,
    -0.500000f, 0.500000f, -0.500000f, -1.000000f, 0.000000f, 0.000000f, 1.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, -1.000000f,
    -0.500000f, -0.500000f, -0.500000f, -1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, -1.000000f,
    -0.500000f, -0.500000f, -0.500000f, -1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, -1.000000f,
    -0.500000f, -0.500000f, 0.500000f, -1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, -1.000000f,
    -0.500000f, 0.500000f, 0.500000f, -1.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, -1.000000f,
    0.500000f, 0.500000f, 0.500000f, 1.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, -0.000000f, -0.000000f, -1.000000f,
    0.500000f, 0.500000f, -0.500000f, 1.000000f, 0.000000f, 0.000000f, 1.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, -0.000000f, -0.000000f, -1.000000f,
    0.500000f, -0.500000f, -0.500000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, -0.000000f, -0.000000f, -1.000000f,
    0.500000f, -0.500000f, -0.500000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, -0.000000f, -0.000000f, -1.000000f,
    0.500000f, -0.500000f, 0.500000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, -0.000000f, -0.000000f, -1.000000f,
    0.500000f, 0.500000f, 0.500000f, 1.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, -0.000000f, -0.000000f, -1.000000f,
    -0.500000f, -0.500000f, -0.500000f, 0.000000f, -1.000000f, 0.000000f, 0.000000f, 1.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, -0.000000f, -1.000000f,
    0.500000f, -0.500000f, -0.500000f, 0.000000f, -1.000000f, 0.000000f, 1.000000f, 1.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, -0.000000f, -1.000000f,
    0.500000f, -0.500000f, 0.500000f, 0.000000f, -1.000000f, 0.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, -0.000000f, -1.000000f,
    0.500000f, -0.500000f, 0.500000f, 0.000000f, -1.000000f, 0.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, -0.000000f, -1.000000f,
    -0.500000f, -0.500000f, 0.500000f, 0.000000f, -1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, -0.000000f, -1.000000f,
    -0.500000f, -0.500000f, -0.500000f, 0.000000f, -1.000000f, 0.000000f, 0.000000f, 1.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, -0.000000f, -1.000000f,
    -0.500000f, 0.500000f, -0.500000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 1.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -1.000000f,
    0.500000f, 0.500000f, -0.500000f, 0.000000f, 1.000000f, 0.000000f, 1.000000f, 1.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -1.000000f,
    0.500000f, 0.500000f, 0.500000f, 0.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -1.000000f,
    0.500000f, 0.500000f, 0.500000f, 0.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -1.000000f,
    -0.500000f, 0.500000f, 0.500000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -1.000000f,
    -0.500000f, 0.500000f, -0.500000f, 0.000000f, 1.000000f, 0.000000f, 0.000000f, 1.000000f, 1.000000f, 0.000000f, 0.000000f, 0.000000f, 0.000000f, -1.000000f,
};



glm::vec3 directional_light_directions[] = {
    glm::vec3(0.0f, -0.5f, -1.0f),
    glm::vec3(-1.0f, -0.5f, 0.0f),
    glm::vec3(0.0f, -0.5f, 1.0f),
    glm::vec3(1.0f, -0.5f, 1.0f)
};

glm::vec3 directional_light_positions[] = {
    glm::vec3(0.0f, -5.0f, -10.0f),
    glm::vec3(-10.0f, -5.0f, 0.0f),
    glm::vec3(0.0f, -5.0f, 10.0f),
    glm::vec3(10.0f, -5.0f, 10.0f)
};

std::vector<point_light_source> point_lights_vec;
std::vector<directional_light_source> dir_lights_vec;
std::vector<normal_textured_cube> normal_cube_vec;
std::vector<mixed_textured_cube> mixed_cube_vec;
std::vector<normal_map_cube> normal_map_cube_vec;

float matrix_speed = 5.0f;
float matrix_direction_x = 1.0f;
float matrix_direction_z = 0.0f;
float cube_speed = 8.0f;
float cube_direction = 5.0f;
bool dir_lights_flag = true;
bool dir_lights_flag_arr[] = {true, true, true, true};

unsigned int container_texture;
unsigned int container2_texture;
unsigned int container2_specular_texture;
unsigned int matrix_texture;
unsigned int brickwall_texture;
unsigned int brickwall_normal_texture;
unsigned int awesome_face_texture;

int main(){
    GLFWwindow* window = initiate("Final Showcase");
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
    stbi_set_flip_vertically_on_load(true);
    glfwSetCursorPosCallback(window, process_mouse_input);
	glfwSetScrollCallback(window, process_scroll_input);
    glfwSwapInterval(1); //VSYNC

    container_texture = generate_texture("./res/Images/container.jpg");
    container2_texture = generate_texture("./res/Images/container2.png");
	container2_specular_texture = generate_texture("./res/Images/container2_specular.png");
    matrix_texture = generate_texture("./res/Images/matrix.jpg");
    brickwall_texture = generate_texture("./res/Images/brickwall.jpg");
    brickwall_normal_texture = generate_texture("./res/Images/brickwall_normal.jpg");
    awesome_face_texture = generate_texture("./res/Images/awesomeface.png");
    //ImGUI Setup
    const char* glsl_version = "#version 330";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); 
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange; 
    (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
    // For frame time calculations
    double frame_time = 0.0f;
    double current_frame = 0.0f;
    double last_frame = 0.0f;
    //Initiating and setting the objects
    //First assigning the directional lights
    directional_light_source dir_lights[4];
    for(int i = 0; i < 4; i++){
        directional_light_source dir_light(glm::vec3(0.25f), glm::vec3(0.25f), glm::vec3(0.25f), directional_light_directions[i]);
        dir_light.toggle_light(true);
        dir_light.set_position(directional_light_positions[i]);
        dir_light.set_VAO(light_source_vertices, sizeof(light_source_vertices));
        dir_light.set_program("./res/Shaders/VertexShader1_31.txt", "./res/Shaders/FragmentShader1_31.txt");
        dir_lights_vec.push_back(dir_light);
    }
    //Template on how to render a point light
    point_light_source demo_point_light(glm::vec3(0.25f), glm::vec3(0.25f), glm::vec3(0.25f), 1.0f, 0.045f, 0.0075f);
    demo_point_light.toggle_light(true);
    demo_point_light.set_position(glm::vec3(-25.0f, 15.0f, 0.0f));
    demo_point_light.set_VAO(light_source_vertices, sizeof(light_source_vertices));
    demo_point_light.set_program("./res/Shaders/VertexShader1_31.txt", "./res/Shaders/FragmentShader1_31.txt");

    //Template on how to render a normal texture cube
    normal_textured_cube demo_tex_cube;
    demo_tex_cube.set_position(glm::vec3(-20.0f, 15.0f, 0.0f));
    demo_tex_cube.set_VAO(texture_cube_vertices, sizeof(texture_cube_vertices));
    demo_tex_cube.set_program("./res/Shaders/VertexShader2_31.txt", "./res/Shaders/FragmentShader2_31.txt");
    demo_tex_cube.assign_textures(container2_texture, container2_specular_texture);

    //Template on how to render a mixed texture cube
    mixed_textured_cube demo_mixed_cube;
    demo_mixed_cube.set_position(glm::vec3(-30.0f, 15.0f, 0.0f));
    demo_mixed_cube.set_VAO(texture_cube_vertices, sizeof(texture_cube_vertices));
    demo_mixed_cube.set_program("./res/Shaders/VertexShader3_31.txt", "./res/Shaders/FragmentShader3_31.txt");
    demo_mixed_cube.assign_textures(container_texture, awesome_face_texture);

    //Template on how to render a normal map cube
    normal_map_cube demo_normal_mapped_cube;
    demo_normal_mapped_cube.set_position(glm::vec3(-35.0f, 15.0f, 0.0f));
    demo_normal_mapped_cube.set_normal_map_VAO(normal_map_vertices, sizeof(normal_map_vertices));
    demo_normal_mapped_cube.set_program("./res/Shaders/VertexShader4_31.txt", "./res/Shaders/FragmentShader4_31.txt");
    demo_normal_mapped_cube.assign_textures(brickwall_texture, brickwall_normal_texture);

    //Generating the container floor
    quad_object main_floor;
    main_floor.set_position(glm::vec3(-20.0f, 0.0f, 20.0f), glm::vec3(-20.0f, 0.0f, -20.0f), glm::vec3(20.0f, 0.0f, -20.0f), glm::vec3(20.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    main_floor.assign_textures(container2_texture, container2_specular_texture);
    main_floor.set_coordinates(glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f));
    main_floor.set_VAO();
    main_floor.set_program("./res/Shaders/VertexShader4_31.txt", "./res/Shaders/FragmentShader4_31.txt");

    //Generating the matrix floor
    simple_quad matrix_floor;
    matrix_floor.set_position(glm::vec3(-5.0f, 0.01f, 5.0f), glm::vec3(-5.0f, 0.01f, -5.0f), glm::vec3(5.0f, 0.01f, -5.0f), glm::vec3(5.0f, 0.01f, 5.0f), glm::vec3(0.0f, 0.01f, 0.0f));
    matrix_floor.assign_textures(matrix_texture, matrix_texture);
    matrix_floor.set_coordinates(glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f));
    matrix_floor.set_simple_VAO();
    matrix_floor.set_program("./res/Shaders/VertexShader5_31.txt", "./res/Shaders/FragmentShader5_31.txt");

    while(!glfwWindowShouldClose(window)){
        //Log frames to implement frame-based moving
        current_frame = glfwGetTime();
        frame_time = current_frame - last_frame;
        last_frame = current_frame;
        //Frame Setup
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
        process_keyboard_input(window, float(frame_time));
        //ImGui stuff
        ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Sliders Here!");
        ImGui::Checkbox("Toggle directional lights", &dir_lights_flag);
        if(dir_lights_flag){
            ImGui::Checkbox("Directional Light 1", &dir_lights_flag_arr[0]);
            ImGui::Checkbox("Directional Light 2", &dir_lights_flag_arr[1]);
            ImGui::Checkbox("Directional Light 3", &dir_lights_flag_arr[2]);
            ImGui::Checkbox("Directional Light 4", &dir_lights_flag_arr[3]);
        }
        ImGui::SliderFloat("Cube speed", &cube_speed, 3.0f, 20.0f);
        ImGui::SliderFloat("Matrix speed", &matrix_speed, 3.0f, 20.0f);
        ImGui::Text("FPS: %.2f, Frametime: %.3f", 1.0 / frame_time, frame_time);
		ImGui::End();

        //PROGRAM HERE
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_X / (float)WINDOW_Y, 0.3f, 100.0f);
        //Rendering the directional lights
        for(int i = 0; i < dir_lights_vec.size(); i++){
            if(dir_lights_flag && dir_lights_flag_arr[i]){
                dir_lights_vec[i].enabled = true;
            }else{
                dir_lights_vec[i].enabled = false;
            }
            dir_lights_vec[i].render(view, projection);
        }
        //Rendering the point lights
        for(int i = 0; i < point_lights_vec.size(); i++){
            if(int(glfwGetTime()) % 2 == 0){
                point_lights_vec[i].enabled = true;
            }else{
                point_lights_vec[i].enabled = false;
            }
            point_lights_vec[i].render(view, projection);
            move_cube(point_lights_vec[i].model, point_lights_vec[i].pos, float(frame_time), matrix_floor.center, i, 0);
        }
        //Rendering the normal cubes
        for(int i = 0; i < normal_cube_vec.size(); i++){
            normal_cube_vec[i].render(view, projection, camera.Position, dir_lights_vec, point_lights_vec);
            move_cube(normal_cube_vec[i].model, normal_cube_vec[i].pos, float(frame_time), matrix_floor.center, i, 1);
        }
        //Rendering the mixed cubes
        for(int i = 0; i < mixed_cube_vec.size(); i++){
            mixed_cube_vec[i].render(view, projection, camera.Position, 0.3f, dir_lights_vec, point_lights_vec);
            move_cube(mixed_cube_vec[i].model, mixed_cube_vec[i].pos, float(frame_time), matrix_floor.center, i, 2);
        }
        //Rendering the normal mapped cubes
        for(int i = 0; i < normal_map_cube_vec.size(); i++){
            normal_map_cube_vec[i].render(view, projection, camera.Position, dir_lights_vec, point_lights_vec);
            move_cube(normal_map_cube_vec[i].model, normal_map_cube_vec[i].pos, float(frame_time), matrix_floor.center, i, 3);
        }
        //Rendering the quads, first the main floor
        main_floor.render(view, projection, camera.Position, dir_lights_vec, point_lights_vec);
        //Rendering the matrix quad and its movement patern
        matrix_floor.simple_render(view, projection);
        matrix_floor.set_position(matrix_floor.pos1 + glm::vec3(frame_time*matrix_speed*matrix_direction_x, 0.0f, frame_time*matrix_speed*matrix_direction_z), 
        matrix_floor.pos2 + glm::vec3(frame_time*matrix_speed*matrix_direction_x, 0.0f, frame_time*matrix_speed*matrix_direction_z), 
        matrix_floor.pos3 + glm::vec3(frame_time*matrix_speed*matrix_direction_x, 0.0f, frame_time*matrix_speed*matrix_direction_z), 
        matrix_floor.pos4 + glm::vec3(frame_time*matrix_speed*matrix_direction_x, 0.0f, frame_time*matrix_speed*matrix_direction_z),
        matrix_floor.center + glm::vec3(frame_time*matrix_speed*matrix_direction_x, 0.0f, frame_time*matrix_speed*matrix_direction_z));

        if(matrix_floor.center.x >= 17.5 && matrix_direction_x == 1.0f){
            matrix_direction_z = 1.0f;
            matrix_direction_x = 0.0f;
        }
        if(matrix_floor.center.z >= 17.5 && matrix_direction_z == 1.0f){
            matrix_direction_z = 0.0f;
            matrix_direction_x = -1.0f;
        }
        if(matrix_floor.center.x <= -17.5 && matrix_direction_x == -1.0f){
            matrix_direction_z = -1.0f;
            matrix_direction_x = 0.0f;
        }
        if(matrix_floor.center.z <= -17.5 && matrix_direction_z == -1.0f){
            matrix_direction_z = 0.0f;
            matrix_direction_x = 1.0f;
        }

        //Rendering the demo objects
        demo_normal_mapped_cube.render(view, projection, camera.Position, dir_lights_vec, point_lights_vec);
        demo_mixed_cube.render(view, projection, camera.Position, 0.3f, dir_lights_vec, point_lights_vec);
        demo_tex_cube.render(view, projection, camera.Position, dir_lights_vec, point_lights_vec);
        if(int(glfwGetTime()) % 2 == 0){
            demo_point_light.enabled = true;
        }else{
            demo_point_light.enabled = false;
        }
        demo_point_light.render(view, projection);

        // Now render imgui
        ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    terminate(window);
    return 0;
}

void process_keyboard_input(GLFWwindow* window, float frame_time){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
	    camera.ProcessKeyboard(FORWARD, frame_time);
    }   
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		camera.ProcessKeyboard(BACKWARD, frame_time);
    }
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		camera.ProcessKeyboard(LEFT, frame_time);
    }
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		camera.ProcessKeyboard(RIGHT, frame_time);
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !space_pressed){
        add_random_item();
        space_pressed = true;
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) != GLFW_PRESS){
        space_pressed = false;
    }
}

void process_mouse_input(GLFWwindow* window, double xpos, double ypos) {

    if (first_mouse) {
        lastX = (float)xpos;
        lastY = (float)ypos;
        first_mouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos; // reversed since y-coordinates go from bottom to top

    lastX = (float)xpos;
    lastY = (float)ypos;
    //camera.ProcessMouseMovement(xoffset, yoffset);
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) != GLFW_PRESS) {
        if(cursor_enabled){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cursor_enabled = false;
        }
		camera.ProcessMouseMovement(xoffset, yoffset);
	}else{
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        cursor_enabled = true;
    }
}

void process_scroll_input(GLFWwindow* givenWindow, double givenScrollOffsetX, double givenScrollOffsetY)
{
	camera.ProcessMouseScroll(float(givenScrollOffsetY));
}

void add_random_item(){
    int random_num = rand() % 4;
    int random_x = -15 + (rand() % 30);
    int random_z = -15 + (rand() % 30);
    if(random_num == 0){
        point_light_source point_light(glm::vec3(0.25f), glm::vec3(0.25f), glm::vec3(0.25f), 1.0f, 0.045f, 0.0075f);
        point_light.toggle_light(true);
        point_light.set_position(glm::vec3(float(random_x), 15.0f, float(random_z)));
        point_light.set_VAO(light_source_vertices, sizeof(light_source_vertices));
        point_light.set_program("./res/Shaders/VertexShader1_31.txt", "./res/Shaders/FragmentShader1_31.txt");
        point_lights_vec.push_back(point_light);
    }else if(random_num == 1){
        normal_textured_cube tex_cube;
        tex_cube.set_position(glm::vec3(float(random_x), 15.0f, float(random_z)));
        tex_cube.set_VAO(texture_cube_vertices, sizeof(texture_cube_vertices));
        tex_cube.set_program("./res/Shaders/VertexShader2_31.txt", "./res/Shaders/FragmentShader2_31.txt");
        tex_cube.assign_textures(container2_texture, container2_specular_texture);
        normal_cube_vec.push_back(tex_cube);
    }else if(random_num == 2){
        mixed_textured_cube mixed_cube;
        mixed_cube.set_position(glm::vec3(float(random_x), 15.0f, float(random_z)));
        mixed_cube.set_VAO(texture_cube_vertices, sizeof(texture_cube_vertices));
        mixed_cube.set_program("./res/Shaders/VertexShader3_31.txt", "./res/Shaders/FragmentShader3_31.txt");
        mixed_cube.assign_textures(container_texture, awesome_face_texture);
        mixed_cube_vec.push_back(mixed_cube);
    }else{
        normal_map_cube normal_mapped_cube;
        normal_mapped_cube.set_position(glm::vec3(float(random_x), 15.0f, float(random_z)));
        normal_mapped_cube.set_normal_map_VAO(normal_map_vertices, sizeof(normal_map_vertices));
        normal_mapped_cube.set_program("./res/Shaders/VertexShader4_31.txt", "./res/Shaders/FragmentShader4_31.txt");
        normal_mapped_cube.assign_textures(brickwall_texture, brickwall_normal_texture);
        normal_map_cube_vec.push_back(normal_mapped_cube);
    }
}

void move_cube(glm::mat4& model, glm::vec3& position, float frame_time, glm::vec3 matrix_position, int current_location, int option){
    if(position.y > 0.01){
        position = glm::vec3(position.x, position.y - frame_time * cube_speed, position.z);
        model = glm::translate(glm::mat4(1.0f), glm::vec3(position));
    }else{
        float x_dir, z_dir;
        if(matrix_position.x > position.x){
            x_dir = 1.0f;
        }else{
            x_dir = -1.0f;
        }
        if(matrix_position.z > position.z){
            z_dir = 1.0f;
        }else{
            z_dir = -1.0f;
        }
        position = glm::vec3(position.x + cube_speed * frame_time * x_dir, position.y, position.z + cube_speed * frame_time * z_dir);
        model = glm::translate(glm::mat4(1.0f), glm::vec3(position));
        if(abs(position.x - matrix_position.x) < 0.5f && abs(position.z - matrix_position.z) < 0.5f){
            if(option == 0){
                point_lights_vec.erase(point_lights_vec.begin() + current_location);
            }else if(option == 1){
                normal_cube_vec.erase(normal_cube_vec.begin() + current_location);
            }else if(option == 2){
                mixed_cube_vec.erase(mixed_cube_vec.begin() + current_location);
            }else{
                normal_map_cube_vec.erase(normal_map_cube_vec.begin() + current_location);
            }
        }
    }
}