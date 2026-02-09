#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "functions.h"

void process_input(GLFWwindow* window);

GLfloat vertices[] = {
    //position            //color             
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0,
    0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0,
    0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0,

    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0,
    0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0,
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0,
    0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0,

    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

    0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 0.5f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.5f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f, 0.5f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.5f,
    0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.5f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.5f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.5f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.5f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.5f,

    -0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.5f, 0.0f, 0.0f
};

glm::vec3 cube_positions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(3.0f,  3.0f, 0.0f),
    glm::vec3(-2.5f, -2.5f, 0.0f),
};


int main(){
    //Program Setup
    GLFWwindow* window = initiate("Showcase 23");
    GLuint program = create_shader_program("./res/VertexShader_13.txt", "./res/FragmentShader_13.txt");
    GLuint cube_VAO = create_color_VAO(vertices, sizeof(vertices));
    glm::mat4 identity = glm::mat4(1.0f);
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

    //ImGUI Setup
    const char* glsl_version = "#version 330";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
    //Variables that will be used
    float radius_limit_min = 0.5f;
    float radius_limit_max = 5.0f;
    float speed_limit = 20.0f;
    static bool rotation_flags[] = {false, false, false, false, false};
    float radius[] = {3.0f, 1.0f};
    float rotation_speeds[] = {10.0f, 10.0f, 10.0f, 10.0f, 10.0f};
    float previous_degrees[] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    float frame_time = 0.0f;
    float current_frame = 0.0f;
    float last_frame = 0.0f;
    while(!glfwWindowShouldClose(window)){
        //Log frames to implement frame-based moving
        current_frame = float(glfwGetTime());
        frame_time = current_frame - last_frame;
        last_frame = current_frame;
        //Frame Setup
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        //ImGUI Window Render
        ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Sliders Here!");

        ImGui::SliderFloat("radius12", &radius[0], radius_limit_min, radius_limit_max);
		ImGui::SliderFloat("radius23", &radius[1], radius_limit_min, radius_limit_max);
		ImGui::Checkbox("rotation1", &rotation_flags[0]); ImGui::SameLine();
		ImGui::SliderFloat("speed1", &rotation_speeds[0], 0.0f, speed_limit);
		ImGui::Checkbox("rotation2", &rotation_flags[1]); ImGui::SameLine();
		ImGui::SliderFloat("speed2", &rotation_speeds[1], 0.0f, speed_limit);
		ImGui::Checkbox("rotation3", &rotation_flags[2]); ImGui::SameLine();
		ImGui::SliderFloat("speed3", &rotation_speeds[2], 0.0f, speed_limit);
		ImGui::Checkbox("rotation4", &rotation_flags[3]); ImGui::SameLine();
		ImGui::SliderFloat("speed4", &rotation_speeds[3], 0.0f, speed_limit);
		ImGui::Checkbox("rotation5", &rotation_flags[4]); ImGui::SameLine();
		ImGui::SliderFloat("speed5", &rotation_speeds[4], 0.0f, speed_limit);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //Program here
        glUseProgram(program);
        glBindVertexArray(cube_VAO);
        process_input(window);
        int model_location = glGetUniformLocation(program, "model");
        int view_location = glGetUniformLocation(program, "view");
        int projection_location = glGetUniformLocation(program, "projection");
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), WINDOW_X / (float)WINDOW_Y, 0.3f, 100.0f);
        glm::mat4 view = glm::translate(identity, glm::vec3(0.0f, 0.0f, -8.0f));
        glUniformMatrix4fv(view_location, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, &projection[0][0]);
        //First Cube
        glm::mat4 cube1 = glm::translate(identity, cube_positions[0]);
        if(rotation_flags[0]){
            previous_degrees[0] = previous_degrees[0] + frame_time * rotation_speeds[0];
        }
        cube1 = glm::rotate(cube1, previous_degrees[0], glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(model_location, 1, GL_FALSE, &cube1[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Second Cube
        glm::vec3 cube2_position = glm::vec3(-sqrt(radius[0]), sqrt(radius[0]), sqrt(radius[0]));
        glm::mat4 cube2 = glm::translate(identity, cube2_position);
        if(rotation_flags[3]){
            previous_degrees[3] = previous_degrees[3] + frame_time * rotation_speeds[3];
        }
        if(rotation_flags[1]){
            previous_degrees[1] = previous_degrees[1] + frame_time * rotation_speeds[1];
        }
        cube2 = glm::rotate(cube2, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        cube2 = glm::translate(cube2, -cube2_position);
        cube2 = glm::rotate(cube2, previous_degrees[1], glm::vec3(1.0f, 1.0f, 0.0f));
        cube2 = glm::translate(cube2, cube2_position);
        cube2 = glm::rotate(cube2, previous_degrees[3], glm::vec3(0.0f, 1.0f, 0.0f));
        cube2 = glm::scale(cube2, glm::vec3(1 / 1.7f, 1 / 1.7f, 1 / 1.7f));
        glUniformMatrix4fv(model_location, 1, GL_FALSE, &cube2[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //Third Cube
        glm::vec3 cube3_position = glm::vec3(-sqrt(radius[1]), sqrt(radius[1]), sqrt(radius[1]));
        glm::mat4 cube3 = glm::translate(identity, glm::vec3(cube2[3]));
        cube3 = glm::translate(cube3, cube3_position);

        if(rotation_flags[2]){
            previous_degrees[2] = previous_degrees[2] + frame_time * rotation_speeds[2];
        }
        if(rotation_flags[4]){
            previous_degrees[4] = previous_degrees[4] + frame_time * rotation_speeds[4];
        }
        cube3 = glm::rotate(cube3, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        cube3 = glm::translate(cube3, -cube3_position);
        cube3 = glm::rotate(cube3, previous_degrees[2], glm::vec3(1.0f, 1.0f, 0.0f));
        cube3 = glm::translate(cube3, cube3_position);
        cube3 = glm::rotate(cube3, previous_degrees[4], glm::vec3(0.0f, 0.0f, 1.0f));
        cube3 = glm::scale(cube3, glm::vec3(1 / 1.9f, 1 / 1.9f, 1 / 1.9f));
        glUniformMatrix4fv(model_location, 1, GL_FALSE, &cube3[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glfwSwapBuffers(window);
    }
    terminate(window);
    return 0;
}

void process_input(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}