#include "window.h"
using namespace glm;

const char* window_title = "GLFW Starter Project";
//Object variables
Group* world;
Cube * cube;
Building* building;
Skybox* skybox;

//Shaders
GLuint Window::skyboxShader;
GLuint Window::cubeShader;
#define SKYBOX_VERTEX_SHADER_PATH "./Shaders/skybox.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "./Shaders/skybox.frag"
#define CUBE_VERTEX_SHADER_PATH "./Shaders/cube.vert"
#define CUBE_FRAGMENT_SHADER_PATH "./Shaders/cube.frag"


// Default camera parameters
glm::vec3 cam_pos(0.0f, 0.0f, -40.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

//Other Variables
double lastX;
double lastY;
bool camShouldMove;
bool Window::shouldRebuild;

void Window::initialize_objects()
{
    srand (1);//Random seed
    camShouldMove = false;
    Window::shouldRebuild = false;
    
	// Load the shader program. Make sure you have the correct filepath up top
    Window::skyboxShader = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
    Window::cubeShader = LoadShaders(CUBE_VERTEX_SHADER_PATH, CUBE_FRAGMENT_SHADER_PATH);

    
    world = new Group();
    cube = new Cube(1);
    skybox = new Skybox();
    building = new Building();
    
    
    // Build world
//    world->addChild(cube);
    //world->addChild(building);
    
    //Construct City
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            MatrixTransform *pos = new MatrixTransform(translate(mat4(1.0f), vec3(5.0f*i, 0.0, 5.0f*j)));
            world->addChild(pos);
            pos->addChild(new Building());
            //pos->addChild(new Cube(1));

        }
    }

	
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
	glDeleteProgram(skyboxShader);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
	//cube->update();
}

void Window::display_callback(GLFWwindow* window)
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Draw skybox first
    glUseProgram(skyboxShader);
    glDepthMask(GL_FALSE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    skybox->draw(skyboxShader);
	
    
	// Render the world
    world->draw(mat4(1.0f));

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}


void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
        if (key == GLFW_KEY_S){
            glm::vec3 direction = glm::normalize(cam_look_at - cam_pos);
            cam_pos = cam_pos - direction;
            V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
        else if (key == GLFW_KEY_W){
            glm::vec3 direction = glm::normalize(cam_look_at - cam_pos);
            cam_pos = cam_pos + direction;
            Window::V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
        else if (key == GLFW_KEY_A){
            glm::vec3 direction = glm::normalize(cam_look_at - cam_pos);
            glm::vec3 left = glm::cross(cam_up, direction);
            cam_pos = cam_pos + left;
            V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
        else if (key == GLFW_KEY_D){
            glm::vec3 direction = glm::normalize(cam_look_at - cam_pos);
            glm::vec3 right = glm::cross(-cam_up, direction);
            cam_pos = cam_pos + right;
            V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
        
        //Regenerate buildings
        else if (key == GLFW_KEY_B){
            Window::shouldRebuild = true;
        }
        
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
	}
    else if(action == GLFW_RELEASE){
        Window::shouldRebuild = false;
    }
}


void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        lastX = xpos;
        lastY = ypos;
        camShouldMove = true;
    }
    
    else if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
        camShouldMove = false;
    }
    
}


// Camera movement when mouse moved
void Window::mouse_move_callback(GLFWwindow* window, double xpos, double ypos){
    if(camShouldMove){
        float angle;
        // Perform horizontal (y-axis) rotation
        angle = (float) (lastX - xpos) / 100.0f;
        cam_pos = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(cam_pos, 1.0f));
        cam_up = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(cam_up, 1.0f));
        //Now rotate vertically based on current orientation
        angle = (float) (ypos - lastY) / 100.0f;
        glm::vec3 axis = glm::cross((cam_pos - cam_look_at), cam_up);
        cam_pos = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, axis) * glm::vec4(cam_pos, 1.0f));
        cam_up = glm::vec3(glm::rotate(glm::mat4(1.0f), angle, axis) * glm::vec4(cam_up, 1.0f));
        // Now update the camera
        V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        lastX = xpos;
        lastY = ypos;
    }
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    
}
