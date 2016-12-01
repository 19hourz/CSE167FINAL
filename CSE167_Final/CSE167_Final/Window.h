#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "shader.h"
#include "Cube.h"
#include "skybox.h"
#include "Group.h"
#include "MatrixTransform.h"
#include "Building.h"
#include "Plane.h"
#include "Water.h"
class Window
{
public:
    //Variables
    static glm::mat4 worldPos;
	static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
    static bool shouldRebuild;
    static vec3 cam_pos;
    //Shaders
    static GLuint skyboxShader;
    static GLuint cubeShader;
    static GLuint waterShader;

    //Functions
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_move_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);



};

#endif
