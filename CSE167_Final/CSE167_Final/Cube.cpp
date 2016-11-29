#include "Cube.h"
#include "Window.h"

//#define VERTEX_CUBESHADER_PATH "CubeShader.vert"
//#define FRAGMENT_CUBESHADER_PATH "CubeShader.frag"

using namespace std;

GLuint buildingTexture;

// Note that GL_QUADS is deprecated in modern OpenGL (and removed from OSX systems).
// This is why we need to draw each face as 2 triangles instead of 1 quadrilateral
//GLuint indices[6][6] = {
//    // Front face
//    {0, 1, 2, 2, 3, 0},
//    // Top face
//    {1, 5, 6, 6, 2, 1},
//    // Back face
//    {7, 6, 5, 5, 4, 7},
//    // Bottom face
//    {4, 0, 3, 3, 7, 4},
//    // Left face
//    {4, 5, 1, 1, 0, 4},
//    // Right face
//    {3, 2, 6, 6, 7, 3}
//};


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




Cube::Cube(int type)
{
//    GLfloat vertices[8][3] = {
//        
//        // "Front" vertices
//        {0.0, 0.0,  2.0}, {2.0, 0.0,  2.0}, {2.0,  2.0,  2.0}, {0.0,  2.0,  2.0},
//        // "Back" vertices
//        {0.0, 0.0, 0.0}, {2.0, 0.0, 0.0}, {2.0, 2.0, 0.0}, {0.0,  2.0, 0.0}
//        
//    };
    
    GLfloat vertices[] = {
        // Positions          // Texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    
    this->cubeType = type;
    countSpin = 0;
    angle = 1.0f;
    toWorld = glm::mat4(1.0f);
    buildingTexture = loadTexture("textures/Building.ppm");
    
    // Setup cube VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glBindVertexArray(0);
    
//    // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//    glBindVertexArray(VAO);
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
//                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
//                          GL_FLOAT, // What type these components are
//                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
//                          3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
//                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);
    
    
}

Cube::~Cube()
{
    // Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a
    // large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &CAO);
    glDeleteBuffers(1, &CBO);
}

void Cube::draw(mat4 C)
{
    glDisable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glUseProgram(Window::cubeShader);
    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 modelview = Window::V * C * toWorld;
    // We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
    // Consequently, we need to forward the projection, view, and model matrices to the shader programs
    // Get the location of the uniform variables "projection" and "modelview"
    uProjection = glGetUniformLocation(Window::cubeShader, "projection");
    uModelview = glGetUniformLocation(Window::cubeShader, "modelview");
    GLuint utype = glGetUniformLocation(Window::cubeShader, "type");
    
    // Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    glUniform1i(utype, cubeType);
//    // Now draw the cube. We simply need to bind the VAO associated with it.
//    glBindVertexArray(VAO);
//    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
//    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
//    glBindVertexArray(0);
    
    // Cubes
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(Window::cubeShader, "ourTexture"), 0);
    glBindTexture(GL_TEXTURE_2D, buildingTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);

    
}

void Cube::update()
{
    spin();
    //move();
}

void Cube::spin()
{
    if(cubeType == 1 || cubeType == 2){
        if (countSpin == 40){
            countSpin = -40;
            angle = -angle;
        }
        else{
            countSpin++;
        }
        
        mat4 trans = glm::translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f));
        mat4 rev_trans = glm::translate(mat4(1.0f), vec3(0.0f, -2.0f, 0.0f));
        toWorld = trans * glm::rotate(glm::mat4(1.0f), angle / 180.0f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f)) * rev_trans * toWorld;
    }
}

//void Cube::move()
//{
//    toWorld = translate(mat4(1.0f), vec3(0.1f,0.0f,0.1f)) * toWorld;
//}

