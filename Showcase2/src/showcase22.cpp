#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "functions.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void process_input(GLFWwindow* window, ImVec4 &position, float frame_time);

GLfloat polygon10_vertices[] =
{
    0.5f, 0.0f, 0.0f,
    0.4f, 0.25f, 0.0f,
    0.15f, 0.5f, 0.0f,
    -0.15f, 0.5f, 0.0f,
    -0.4f, 0.25f, 0.0f,
    -0.5f, 0.0f, 0.0f,
    -0.4f, -0.25f, 0.0f,
    -0.15f, -0.5f, 0.0f,
    0.15f, -0.5f, 0.0f,
    0.4f, -0.25f, 0.0f
};

GLfloat polygon6_vertices[] = {
	0.5f, 0.0f, 0.0f,
	0.25f, 0.5f, 0.0f,
	-0.25f, 0.5f, 0.0f,
	-0.5f, 0.0f, 0.0f,
	-0.25f, -0.5f, 0.0f,
	0.25f, -0.5f, 0.0f,
};

int polygon_mode = 0;
bool wireframe_mode = false;
bool caps_flag = false;
bool tab_flag = false;

int main(){
    //Program Setup
    GLFWwindow* window = initiate("Exercise 2 (Press CAPS LOCK to change polygon and TAB for wireframe!)");
    GLuint polygon_program = create_shader_program("../Shaders/VertexShader_12.txt", "../Shaders/FragmentShader_12.txt");
    GLuint polygon6_VAO = create_VAO(polygon6_vertices, sizeof(polygon6_vertices));
    GLuint polygon10_VAO = create_VAO(polygon10_vertices, sizeof(polygon10_vertices));
    
    GLint position_location;
    ImVec4 position_offset = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    float frame_time = 0.0f;
    float current_frame = 0.0f;
    float last_frame = 0.0f;
    while(!glfwWindowShouldClose(window)){
        //Log frames to implement frame-based moving
        current_frame = glfwGetTime();
        frame_time = current_frame - last_frame;
        last_frame = current_frame;
        //Frame setup
        glfwPollEvents();
        process_input(window, position_offset, frame_time);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //Program here
        glUseProgram(polygon_program);
        position_location = glGetUniformLocation(polygon_program, "position_offset");
        glUniform3f(position_location, position_offset.x, position_offset.y, position_offset.z);
        if(polygon_mode == 0){
            glBindVertexArray(polygon10_VAO);
            glDrawArrays(GL_POLYGON, 0, 10);
        }else{
            glBindVertexArray(polygon6_VAO);
            glDrawArrays(GL_POLYGON, 0, 6);
        }
        
        glfwSwapBuffers(window);
    }
    terminate(window);
    return 0;
}

void process_input(GLFWwindow* window, ImVec4 &position, float frame_time){
    float move_speed = 0.5f * frame_time;
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
        if(!tab_flag && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS){
        if(!wireframe_mode){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }else{
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        wireframe_mode = !wireframe_mode;
        tab_flag = true;
    }
    if(!caps_flag && glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS){
        polygon_mode += 1;
        if(polygon_mode >= 2){
            polygon_mode = 0;
        }
        caps_flag = true;
    }
    if(tab_flag && glfwGetKey(window, GLFW_KEY_TAB) != GLFW_PRESS){
        tab_flag = false;
    }
    if(caps_flag && glfwGetKey(window, GLFW_KEY_CAPS_LOCK) != GLFW_PRESS){
        caps_flag = false;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position.y = position.y - move_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position.y = position.y + move_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position.x = position.x + move_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position.x = position.x - move_speed;
	}
    if(position.x > 0.5){position.x = 0.5;};
    if(position.x < -0.5){position.x = -0.5;};
    if(position.y > 0.5){position.y = 0.5;};
    if(position.y < -0.5){position.y = -0.5;};
}