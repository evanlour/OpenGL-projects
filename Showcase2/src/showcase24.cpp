#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "functions.h"
#include "Camera.h"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool cursor_enabled = false;
bool first_mouse = true;
static float lastX = (float)WINDOW_X / 2.0f;
static float lastY = (float)WINDOW_Y / 2.0f;

void process_keyboard_input(GLFWwindow* window, float frame_time);
void process_mouse_input(GLFWwindow* window, double givenMousePositionX, double givenMousePositionY);
void process_scroll_input(GLFWwindow* givenWindow, double givenScrollOffsetX, double givenScrollOffsetY);

GLfloat vertices[] =
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

glm::vec3 cube_positions[] =
{
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 light_positions[] = {
    glm::vec3(2.3f, -3.3f, -4.0f),
    glm::vec3(0.0f, 0.0f, -3.0f),
    glm::vec3(0.0f, 10.0f, -10.0f),
    glm::vec3(0.0f, -10.0f, -10.0f),
    glm::vec3(-10.0f, 10.0f, -10.0f),
    glm::vec3(-1.0f, -1.0f, -1.0f) //Numbers here do not matter, this is the rotating cube
};

glm::vec3 light_directions[] = {
    glm::vec3(0.0f, -1.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(1.0f, -1.0f, 0.0f)
};

glm::vec3 ambient_colors[] = {
    glm::vec3(0.0215f, 0.1745f, 0.0215f),//Emerald
    glm::vec3(0.135f, 0.2225f, 0.1575f),//Jade
    glm::vec3(0.05375f, 0.05f, 0.06625f),//Obsidian
    glm::vec3(0.25f, 0.20725f, 0.20725f),//Pearl
    glm::vec3(0.1745f, 0.01175f, 0.01175f),//Ruby
    glm::vec3(0.1f, 0.18725f, 0.1745f),//Turquoise
    glm::vec3(0.329412f, 0.223529f, 0.027451f),//Brass
    glm::vec3(0.2125f, 0.1275f, 0.054f),//Bronze
    glm::vec3(0.25f, 0.25f, 0.25f),//Chrome
    glm::vec3(0.19125f, 0.0735f, 0.0225f)//Copper
};

glm::vec3 diffuse_colors[] = {
    glm::vec3(0.07568f, 0.61424f, 0.07568f),
    glm::vec3(0.54f, 0.89f, 0.63f),
    glm::vec3(0.18275f, 0.17f, 0.22525f),
    glm::vec3(1.0f, 0.829f, 0.829f),
    glm::vec3(0.61424f, 0.04136f, 0.04136f),
    glm::vec3(0.396f, 0.74151f, 0.69102f),
    glm::vec3(0.780392f, 0.568627f, 0.113725f),
    glm::vec3(0.714f, 0.4284f, 0.18144f),
    glm::vec3(0.4f, 0.4f, 0.4f),
    glm::vec3(0.7038f, 0.27048f, 0.0828f)
};

glm::vec3 specular_colors[] = {
    glm::vec3(0.633f, 0.727811f, 0.633f),
    glm::vec3(0.316228f, 0.316228f, 0.316228f),
    glm::vec3(0.332741f, 0.328634f, 0.346435f),
    glm::vec3(0.296648f, 0.296648f, 0.296648f),
    glm::vec3(0.727811f, 0.626959f, 0.626959f),
    glm::vec3(0.297254f, 0.30829f, 0.306678f),
    glm::vec3(0.992157f, 0.941176f, 0.807843f),
    glm::vec3(0.393548f, 0.271906f, 0.166721f),
    glm::vec3(0.774597f, 0.774597f, 0.774597f),
    glm::vec3(0.256777f, 0.137622f, 0.086014f)
};

float cube_shininess[] = {
    0.6f, 0.1f, 0.3f, 0.088f, 0.6f,
    0.1f, 0.21794872, 0.2f, 0.6f, 0.1f
};

float moving_light_radius = 5.0f;
float moving_light_speed = 2.0f;
int ki_option = 4;
float linear_values[] = {0.22f, 0.14f, 0.09f, 0.07f, 0.045f};
float quadratic_values[] = {0.2, 0.07, 0.032, 0.017, 0.0075};
bool light_flags[] = {true, true, true, true, true, true};
bool global_point_light_flag = true;
bool global_direct_light_flag = true;
int main(){
    //Program Setup
    GLFWwindow* window = initiate("Exercise 4");
    GLuint cube_VAO = create_color_VAO(vertices, sizeof(vertices));
    GLuint cube_program = create_shader_program("../Shaders/VertexShader_21.txt", "../Shaders/FragmentShader_21.txt");
    GLuint light_program = create_shader_program("../Shaders/Vertex_light_21.txt", "../Shaders/Fragment_light_21.txt");
    glm::mat4 identity = glm::mat4(1.0f);
    glm::vec3 light_source_color(1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
    glfwSetCursorPosCallback(window, process_mouse_input);
	glfwSetScrollCallback(window, process_scroll_input);
    glfwSwapInterval(1); //VSYNC
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
    float frame_time = 0.0f;
    float current_frame = 0.0f;
    float last_frame = 0.0f;
    float moving_light_degrees = 0.0f;
    while(!glfwWindowShouldClose(window)){
        //Log frames to implement frame-based moving
        current_frame = glfwGetTime();
        frame_time = current_frame - last_frame;
        last_frame = current_frame;
        //Frame Setup
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        process_keyboard_input(window, frame_time);
        // ImGUI Window Render
        ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Sliders Here!");
        ImGui::Checkbox("Point Lights", &global_point_light_flag);
        if(global_point_light_flag){
            ImGui::Text("Point light distance: "); ImGui::SameLine();
            ImGui::RadioButton("20", &ki_option, 0); ImGui::SameLine();
            ImGui::RadioButton("32", &ki_option, 1); ImGui::SameLine();
            ImGui::RadioButton("50", &ki_option, 2); ImGui::SameLine();
            ImGui::RadioButton("65", &ki_option, 3); ImGui::SameLine();
		    ImGui::RadioButton("100", &ki_option, 4);
            ImGui::Checkbox("Point Light 1", &light_flags[0]);
            ImGui::Checkbox("Point Light 2", &light_flags[1]);
            ImGui::Checkbox("Moving Point Light", &light_flags[5]);
            if(light_flags[5]){
                ImGui::SliderFloat("Moving Point Light Radius", &moving_light_radius, 2.0f, 10.0f);
                ImGui::SliderFloat("Moving Point Light Speed (Degrees)", &moving_light_speed, 1.0f, 6.0f);
            }
        }
        ImGui::Checkbox("Directional Lights", &global_direct_light_flag);
        if(global_direct_light_flag){
            ImGui::Checkbox("Directional Light 1", &light_flags[2]);
            ImGui::Checkbox("Directional Light 2", &light_flags[3]);
            ImGui::Checkbox("Directional Light 3", &light_flags[4]);
        }
        ImGui::Text("FPS: %.2f, Frametime: %.3f", 1.0 / frame_time, frame_time);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //Program here
        glUseProgram(cube_program);
        glBindVertexArray(cube_VAO);
        //Set the position of the moving light;
        moving_light_degrees = moving_light_degrees + frame_time * moving_light_speed;
        light_positions[5] = glm::vec3(moving_light_radius * cos(moving_light_degrees), moving_light_radius * sin(moving_light_degrees), 0);
        //View position
        GLuint camera_position = glGetUniformLocation(cube_program, "camera_position");
        glUniform3f(camera_position, camera.Position.x, camera.Position.y, camera.Position.z);
        glm::mat4 view = camera.GetViewMatrix();
        GLuint view_location = glGetUniformLocation(cube_program, "view");
        glUniformMatrix4fv(view_location, 1, GL_FALSE, &view[0][0]);
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_X / (float)WINDOW_Y, 0.3f, 100.0f);
        GLuint projection_location = glGetUniformLocation(cube_program, "projection");
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, &projection[0][0]);
        for(int i = 0; i < 10; i++){
            //Material Positions
            GLuint ambient_color = glGetUniformLocation(cube_program, "material.ambient_color");
            glUniform3f(ambient_color, ambient_colors[i].x, ambient_colors[i].y, ambient_colors[i].z);
            GLuint diffuse_color = glGetUniformLocation(cube_program, "material.diffuse_color");
            glUniform3f(diffuse_color, diffuse_colors[i].x, diffuse_colors[i].y, diffuse_colors[i].z);
            GLuint specular_color = glGetUniformLocation(cube_program, "material.specular_color");
            glUniform3f(specular_color, specular_colors[i].x, specular_colors[i].y, specular_colors[i].z);
            GLuint shininess = glGetUniformLocation(cube_program, "material.shininess");
            glUniform1f(shininess, cube_shininess[i]);
            for(int i = 0; i < 6; i++){
                //Light positions
                std::string light_position_name = "light_sources[" + std::to_string(i) + "].position";
                GLuint light_position = glGetUniformLocation(cube_program, light_position_name.c_str());
                glUniform3f(light_position, light_positions[i].x, light_positions[i].y, light_positions[i].z);

                std::string source_type_name = "light_sources[" + std::to_string(i) + "].type";
                std::string source_direction_name = "light_sources[" + std::to_string(i) + "].direction";
                GLuint light_type = glGetUniformLocation(cube_program, source_type_name.c_str());
                GLuint light_direction = glGetUniformLocation(cube_program, source_direction_name.c_str());                
                if(i > 1 && i != 5){
                    glUniform1i(light_type, 1);
                    glUniform3f(light_direction, light_directions[i - 2].x, light_directions[i - 2].y, light_directions[i - 2].z);
                }else{
                    glUniform1i(light_type, 0);
                }

                if(light_flags[i] == false){
                    glUniform1i(light_type, 2);
                }

                if(global_point_light_flag == false && (i < 2 || i == 5)){
                    glUniform1i(light_type, 2);
                }

                if(global_direct_light_flag == false && i > 1 && i != 5){
                    glUniform1i(light_type, 2);
                }

                std::string source_ambient_name = "light_sources[" + std::to_string(i) + "].ambient_color";
                GLuint ambient_light = glGetUniformLocation(cube_program, source_ambient_name.c_str());
                glUniform3f(ambient_light, 1.0f, 1.0f, 1.0f);

                std::string source_diffuse_name = "light_sources[" + std::to_string(i) + "].diffuse_color";
                GLuint diffuse_light = glGetUniformLocation(cube_program, source_diffuse_name.c_str());
                glUniform3f(ambient_light, 1.0f, 1.0f, 1.0f);

                std::string source_specular_color = "light_sources[" + std::to_string(i) + "].specular_color";
                GLuint specular_light = glGetUniformLocation(cube_program, source_specular_color.c_str());
                glUniform3f(specular_light, 1.0f, 1.0f, 1.0f);

                std::string source_constant = "light_sources[" + std::to_string(i) + "].constant";
                GLuint light_constant = glGetUniformLocation(cube_program, source_constant.c_str());
                glUniform1f(light_constant, 1.0f);

                std::string source_linear = "light_sources[" + std::to_string(i) + "].linear";
                GLuint light_linear = glGetUniformLocation(cube_program, source_linear.c_str());
                glUniform1f(light_linear, linear_values[ki_option]);
                std::string source_quadratic = "light_sources[" + std::to_string(i) + "].quadratic";
                GLuint light_quadratic = glGetUniformLocation(cube_program, source_quadratic.c_str());
                glUniform1f(light_quadratic, quadratic_values[ki_option]);
            }

            glm::mat4 model = glm::translate(identity, cube_positions[i]);
            model = glm::rotate(model, glm::radians(20.0f) * i, glm::vec3(1.0f, 0.3f, 0.5f));
            glm::mat3 normal_transformation = glm::transpose(glm::inverse(glm::mat3(model)));
            GLuint model_location = glGetUniformLocation(cube_program, "model");
            GLuint normal_transformation_location = glGetUniformLocation(cube_program, "normal_transformation");
            glUniformMatrix4fv(model_location, 1, GL_FALSE, &model[0][0]);
            glUniformMatrix3fv(normal_transformation_location, 1, GL_FALSE, &normal_transformation[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //Now for the lights
        glUseProgram(light_program);
        view_location = glGetUniformLocation(light_program, "view");
        glUniformMatrix4fv(view_location, 1, GL_FALSE, &view[0][0]);
        projection_location = glGetUniformLocation(light_program, "projection");
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, &projection[0][0]);
        for(int i = 0; i < 6; i++){
            GLuint active_light_location = glGetUniformLocation(light_program, "active_light");
            if(light_flags[i] == true){
                if(global_direct_light_flag == true && i > 1 && i != 5){
                    glUniform1i(active_light_location, 1);
                }else if(global_point_light_flag == true && (i < 2 || i == 5)){
                    glUniform1i(active_light_location, 1);
                }else{
                    glUniform1i(active_light_location, 0);
                }
            }else{
                glUniform1i(active_light_location, 0);
            }
            glm::mat4 model = glm::translate(identity, light_positions[i]);
            GLuint model_location = glGetUniformLocation(light_program, "model");
            glUniformMatrix4fv(model_location, 1, GL_FALSE, &model[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
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
    	//if shift is pressed, double speed
	if ((glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)) {
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
			camera.ProcessKeyboard(UPWARDS, frame_time);
        }
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
			camera.ProcessKeyboard(DOWNWARDS, frame_time);
        }
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
		camera.ProcessKeyboard(UPWARDS, frame_time);
    }
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
		camera.ProcessKeyboard(DOWNWARDS, frame_time);
    }

    // if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
	// 	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //     //first_mouse = true;
	// }else{
    //     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	// }
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
	camera.ProcessMouseScroll(givenScrollOffsetY);
}