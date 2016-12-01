#include "window.h"
using namespace glm;
using namespace std;

const char* window_title = "GLFW Starter Project";
//Texture variables
GLuint Window::buildingTexture,Window::buildingTexture1,Window::buildingTexture2,Window::buildingTexture3,Window::buildingTexture4;
GLuint Window::buildingTexture5,Window::buildingTexture6,Window::buildingTexture7,Window::buildingTexture8;
GLuint Window::cityTexture1,Window::cityTexture2,Window::cityTexture3,Window::cityTexture4;
//Object variables
Group* world;
Cube * cube;
Cube * ground1;
Cube * ground2;
Cube * ground3;
Cube * ground4;

Building* building;
Skybox* skybox;
Plane* plane;
Water* water;
bool up = false;
bool down = false;
bool rightRot = false;
bool leftRot = false;
bool accelerate = false;
bool deccelerate = false;
bool planeView = true;
bool godInit = false;
//Shaders
GLuint Window::skyboxShader;
GLuint Window::cubeShader;
GLuint Window::waterShader;
#define SKYBOX_VERTEX_SHADER_PATH "./Shaders/skybox.vert"
#define SKYBOX_FRAGMENT_SHADER_PATH "./Shaders/skybox.frag"
#define CUBE_VERTEX_SHADER_PATH "./Shaders/cube.vert"
#define CUBE_FRAGMENT_SHADER_PATH "./Shaders/cube.frag"
#define WATER_VERTEX_SHADER_PATH "./Shaders/water.vert"
#define WATER_FRAGMENT_SHADER_PATH "./Shaders/water.frag"


// Default camera parameters
vec3 Window::cam_pos(0.0f, 0.5f, 0.0f);		// e  | Position of camera
vec3 cam_look_at(-0.7071f, 0.0f, -0.7071f);	// d  | This is where the camera looks at
vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;
glm::mat4 Window::worldPos;
glm::mat4 Window::P;
glm::mat4 Window::V;

//Other Variables
double lastX;
double lastY;
bool camShouldMove;
bool Window::shouldRebuild;
mat4 groundPos1,groundPos2,groundPos3,groundPos4;



/** Load a ppm file from disk.
 @input filename The location of the PPM file.  If the file is not found, an error message
 will be printed and this function will return 0
 @input width This will be modified to contain the width of the loaded image, or 0 if file not found
 @input height This will be modified to contain the height of the loaded image, or 0 if file not found
 @return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
 **/
unsigned char* readPPM(const char* filename, int& width, int& height)
{
    const int BUFSIZE = 128;
    FILE* fp;
    unsigned int read;
    unsigned char* rawData;
    char buf[3][BUFSIZE];
    char* retval_fgets;
    size_t retval_sscanf;
    
    if ( (fp=fopen(filename, "rb")) == NULL)
    {
        std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
        width = 0;
        height = 0;
        return NULL;
    }
    
    // Read magic number:
    retval_fgets = fgets(buf[0], BUFSIZE, fp);
    
    // Read width and height:
    do
    {
        retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');
    retval_sscanf=sscanf(buf[0], "%s %s", buf[1], buf[2]);
    width  = atoi(buf[1]);
    height = atoi(buf[2]);
    
    // Read maxval:
    do
    {
        retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');
    
    // Read image data:
    rawData = new unsigned char[width * height * 3];
    read = fread(rawData, width * height * 3, 1, fp);
    fclose(fp);
    if (read != 1)
    {
        std::cerr << "error parsing ppm file, incomplete data" << std::endl;
        delete[] rawData;
        width = 0;
        height = 0;
        return NULL;
    }
    
    return rawData;
}


// This function loads a texture from file. Note: texture loading functions like these are usually
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio).
// For learning purposes we'll just define it as a utility function.
GLuint loadTexture(GLchar* path)
{
    //Generate texture ID and load texture data
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width,height;
    unsigned char* image = readPPM(path, width, height);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}


void Window::initialize_objects()
{
    //Load Texture
    Window::buildingTexture = loadTexture("./textures/Building.ppm");
    Window::buildingTexture1 = loadTexture("./textures/Building1.ppm");
    Window::buildingTexture2 = loadTexture("./textures/Building2.ppm");
    Window::buildingTexture3 = loadTexture("./textures/Building3.ppm");
    Window::buildingTexture4 = loadTexture("./textures/Building4.ppm");
    Window::buildingTexture5 = loadTexture("./textures/Building5.ppm");
    Window::buildingTexture6 = loadTexture("./textures/Building6.ppm");
    Window::buildingTexture7 = loadTexture("./textures/Building7.ppm");
    Window::buildingTexture8 = loadTexture("./textures/Building8.ppm");
    Window::cityTexture1 = loadTexture("./textures/city1.ppm");
    Window::cityTexture2 = loadTexture("./textures/city2.ppm");
    //cityTexture3 = loadTexture("./textures/city3.ppm");
    //cityTexture4 = loadTexture("./textures/city3.ppm");
    
    
    srand (1);//Random seed
    camShouldMove = false;
    Window::shouldRebuild = false;
    Window::worldPos = translate(mat4(1.0f), vec3(-50,0,-50));
    //worldPos = mat4(1.0f);
    mat4 flip = rotate(mat4(1.0f), glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f)) * translate(mat4(1.0f), vec3(-50.0f,0.0f,0.0f));
    groundPos1 = worldPos * translate(mat4(1.0f), vec3(0,0,50)) * flip * scale(mat4(1.0f), vec3(50,0.1,50));
    groundPos2 = worldPos * translate(mat4(1.0f), vec3(0,0,0)) * flip * scale(mat4(1.0f), vec3(50,0.1,50));
    //groundPos3 = worldPos * translate(mat4(1.0f), vec3(0,0,50)) * flip * scale(mat4(1.0f), vec3(50,0.1,50));
    //groundPos4 = worldPos * translate(mat4(1.0f), vec3(50,0,50)) * flip *scale(mat4(1.0f), vec3(50,0.1,50));

    
	// Load the shader program. Make sure you have the correct filepath up top
    Window::skyboxShader = LoadShaders(SKYBOX_VERTEX_SHADER_PATH, SKYBOX_FRAGMENT_SHADER_PATH);
    Window::cubeShader = LoadShaders(CUBE_VERTEX_SHADER_PATH, CUBE_FRAGMENT_SHADER_PATH);
    Window::waterShader = LoadShaders(WATER_VERTEX_SHADER_PATH, WATER_FRAGMENT_SHADER_PATH);
    
    world = new Group();
    cube = new Cube(1);
    ground1 = new Cube(2);
    ground2 = new Cube(3);
    ground3 = new Cube(4);
    ground4 = new Cube(5);

    water = new Water();
    skybox = new Skybox();
    building = new Building();
    plane = new Plane();
    world->addChild(plane);
    world->addChild(water);
    //plane->propellerSpeed = 0.05;
    // Build world
//    world->addChild(cube);
    //world->addChild(building);
    
    //Construct City
    MatrixTransform* shift = new MatrixTransform(translate(mat4(1.0f), vec3(1.5f, 0.0, 0.5f)));
    world->addChild(shift);
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 11; j++){
            Group* community = new Group();
            for(int a = 0; a < 2; a++){
                for(int b = 0; b < 2; b++){
                    MatrixTransform *trans = new MatrixTransform(translate(mat4(1.0f), vec3(5.0f*a, 0.0, 3.0f*b)));
                    trans->addChild(new Building());
                    community->addChild(trans);
                }
            }
            float extra;
            (j < 4)? extra = 0.0f:extra = (j-3)*1.5f;
            MatrixTransform *pos = new MatrixTransform(translate(mat4(1.0f), vec3(13.0f*i, 0.0, 8.0*j + extra)));
            shift->addChild(pos);
            pos->addChild(community);
        }
    }

	
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	delete(cube);
	glDeleteProgram(skyboxShader);
    glDeleteProgram(cubeShader);
    glDeleteProgram(waterShader);
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
    if(planeView){
        vec3 tempDirec = plane->direction;
        cam_pos = plane->center;
        cam_look_at = cam_pos + tempDirec;
        tempDirec = normalize(tempDirec);
        GLfloat pos = 0.2;
        cam_pos = plane->center - pos * tempDirec;
        cam_up = plane->upDirection;
        V = glm::lookAt(cam_pos, cam_look_at, cam_up);
    }else if(!godInit){
        cam_pos = vec3(20.0f, 0.5f, 20.0f);
        cam_look_at = normalize(vec3(-1.0f, 0.0f, -1.0f));
        cam_up = vec3(0.0f, 1.0f, 0.0f);
        V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        godInit = true;
    }
    world->update();
    if (up) plane->up();
    if (down) plane->down();
    if (leftRot) plane->leftRot();
    if (rightRot) plane->rightRot();
    if (accelerate) plane->speedUp();
    if (deccelerate) plane->speedDown();
}

void Window::display_callback(GLFWwindow* window)
{
    // Clear the color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Draw skybox first
    //glEnable(GL_CLIP_DISTANCE0);
    glUseProgram(skyboxShader);
    glDepthMask(GL_FALSE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    skybox->draw(skyboxShader);
	
    //Draw 4 pieces of ground
    ground1->draw(groundPos1);
    ground2->draw(groundPos2);
    //ground3->draw(groundPos3);
    //ground4->draw(groundPos4);

	// Render the world
    world->draw(Window::worldPos);

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
            up = true;
        }
        else if (key == GLFW_KEY_W){
            down = true;
        }
        else if (key == GLFW_KEY_A){
            leftRot = true;
        }
        else if (key == GLFW_KEY_D){
            rightRot = true;
        }
        else if (key == GLFW_KEY_DOWN){
            deccelerate = true;
        }
        else if (key == GLFW_KEY_UP){
            accelerate = true;
        }
        
        //Regenerate buildings
        else if (key == GLFW_KEY_B){
            Window::shouldRebuild = true;
        }
        else if (key == GLFW_KEY_V){
            if (planeView) {
                planeView = false;
            }
            else{
                planeView = true;
                godInit = false;
            }
        }
        
        else if (key == GLFW_KEY_E){
            glm::vec3 direction = glm::normalize(cam_look_at - cam_pos);
            cam_pos = cam_pos - direction;
            V = glm::lookAt(cam_pos, cam_look_at, cam_up);
        }
        else if (key == GLFW_KEY_Q){
            glm::vec3 direction = glm::normalize(cam_look_at - cam_pos);
            cam_pos = cam_pos + direction;
            Window::V = glm::lookAt(cam_pos, cam_look_at, cam_up);
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
        if (key == GLFW_KEY_S){
            up = false;
        }
        else if (key == GLFW_KEY_W){
            down = false;
        }
        else if (key == GLFW_KEY_A){
            leftRot = false;
        }
        else if (key == GLFW_KEY_D){
            rightRot = false;
        }
        else if (key == GLFW_KEY_DOWN){
            deccelerate = false;
        }
        else if (key == GLFW_KEY_UP){
            accelerate = false;
        }
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
