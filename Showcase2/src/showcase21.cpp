#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "functions.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void process_input(GLFWwindow* window);

GLfloat polygon6_vertices[] = {
0.8f, 0.0f, 0.0f,
0.4f, 0.7f, 0.0f,
-0.4f, 0.7f, 0.0f,
-0.8f, 0.0f, 0.0f,
-0.4f, -0.7f, 0.0f,
0.4f, -0.7f, 0.0f
};

GLfloat polygon10_vertices[] = {
     0.8f,      0.0f,      0.0f,
     0.6472f,   0.4702f,   0.0f,
     0.2472f,   0.7608f,   0.0f,
    -0.2472f,   0.7608f,   0.0f,
    -0.6472f,   0.4702f,   0.0f,
    -0.8f,      0.0f,      0.0f,
    -0.6472f,  -0.4702f,   0.0f,
    -0.2472f,  -0.7608f,   0.0f,
     0.2472f,  -0.7608f,   0.0f,
     0.6472f,  -0.4702f,   0.0f
};

GLfloat polygon10_vertices0[] =
{
	-0.95f, 0.0f, 0.0f,
	-0.85f, 0.4f, 0.0f,
	-0.65f, 0.6f, 0.0f,
	-0.35f, 0.6f, 0.0f,
	-0.15f, 0.4f, 0.0f,
	-0.05f, 0.0f, 0.0f,
	-0.15f, -0.4f, 0.0f,
	-0.35f, -0.6f, 0.0f,
	-0.65f, -0.6f, 0.0f,
	-0.85f, -0.4f, 0.0f,
};

GLfloat polygon6_vertices0[] = {
	0.1f, 0.0f, 0.0f,
	0.3f, 0.5f, 0.0f,
	0.6f, 0.5f, 0.0f,
	0.8f, 0.0f, 0.0f,
	0.6f, -0.5f, 0.0f,
	0.3f, -0.5f, 0.0f,
};

int polygon_mode = 0;
bool wireframe_mode = false;
bool caps_flag = false;
bool w_flag = false;

int main(){
    //Program Setup
    GLFWwindow* window = initiate("Exercise 1");
    GLuint polygon_program = create_shader_program("../Shaders/VertexShader_11.txt", "../Shaders/FragmentShader_11.txt");

    GLuint polygon6_VAO = create_VAO(polygon6_vertices, sizeof(polygon6_vertices));
    GLuint polygon10_VAO = create_VAO(polygon10_vertices, sizeof(polygon10_vertices));
    GLuint polygon6_0_VAO = create_VAO(polygon6_vertices0, sizeof(polygon6_vertices0));
    GLuint polygon10_0_VAO = create_VAO(polygon10_vertices0, sizeof(polygon10_vertices0));
    ImVec4 polygon_color = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    ImVec4 second_polygon_color = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);
    GLint color_location;
    //ImGUI Setup
    const char* glsl_version = "#version 330";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

    while(!glfwWindowShouldClose(window)){
        //Frame setup
        glfwPollEvents();
        process_input(window);
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //ImGUI Window Render
        ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Press CAPS LOCK to change polygon!");
		ImGui::SliderFloat("R", &polygon_color.x, 0.0f, 1.0f);
		ImGui::SliderFloat("G", &polygon_color.y, 0.0f, 1.0f);
		ImGui::SliderFloat("B", &polygon_color.z, 0.0f, 1.0f);
        if(polygon_mode == 0){
            ImGui::SliderFloat("R1", &second_polygon_color.x, 0.0f, 1.0f);
		    ImGui::SliderFloat("G1", &second_polygon_color.y, 0.0f, 1.0f);
		    ImGui::SliderFloat("B1", &second_polygon_color.z, 0.0f, 1.0f);
        }
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //Program here
        glUseProgram(polygon_program);
        color_location = glGetUniformLocation(polygon_program, "color");
        if(polygon_mode == 0){
            glUniform3f(color_location, polygon_color.x, polygon_color.y, polygon_color.z);
            glBindVertexArray(polygon6_0_VAO);
            glDrawArrays(GL_POLYGON, 0, 6);
            glUniform3f(color_location, second_polygon_color.x, second_polygon_color.y, second_polygon_color.z);
            glBindVertexArray(polygon10_0_VAO);
            glDrawArrays(GL_POLYGON, 0, 10);
        }
        else if(polygon_mode == 1){
            glUniform3f(color_location, polygon_color.x, polygon_color.y, polygon_color.z);
            glBindVertexArray(polygon10_VAO);
            glDrawArrays(GL_POLYGON, 0, 10);
        }else{
            glUniform3f(color_location, polygon_color.x, polygon_color.y, polygon_color.z);
            glBindVertexArray(polygon6_VAO);
            glDrawArrays(GL_POLYGON, 0, 6);
        }
        glfwSwapBuffers(window);
    }
    terminate(window);
    return 0;
}

void process_input(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
        if(!w_flag && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        if(!wireframe_mode){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }else{
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        wireframe_mode = !wireframe_mode;
        w_flag = true;
    }
    if(!caps_flag && glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS){
        polygon_mode += 1;
        if(polygon_mode >= 3){
            polygon_mode = 0;
        }
        caps_flag = true;
    }
    if(w_flag && glfwGetKey(window, GLFW_KEY_W) != GLFW_PRESS){
        w_flag = false;
    }
    if(caps_flag && glfwGetKey(window, GLFW_KEY_CAPS_LOCK) != GLFW_PRESS){
        caps_flag = false;
    }
}