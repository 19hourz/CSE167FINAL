#include "skybox.h"
#include "Window.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>


using namespace std;
using namespace glm;


GLuint cubemapTexture;

/** Load a ppm file from disk.
 @input filename The location of the PPM file.  If the file is not found, an error message
 will be printed and this function will return 0
 @input width This will be modified to contain the width of the loaded image, or 0 if file not found
 @input height This will be modified to contain the height of the loaded image, or 0 if file not found
 @return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
 **/
unsigned char* loadPPM(const char* filename, int& width, int& height)
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

// load image file into texture object
GLuint loadCubemap(vector<const GLchar*> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    
    int width,height;
    unsigned char* image;
    
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint i = 0; i < faces.size(); i++)
    {
        //image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        image = loadPPM(faces[i], width, height);
        glTexImage2D(
                     GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                     GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
                     );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    
    return textureID;
}


skybox::skybox()
{
    toWorld = glm::mat4(1.0f);
    
    // Setup cube VAO
    GLuint cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glBindVertexArray(0);
    // Setup skybox VAO
    
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
    
#pragma endregion
    
    // Cubemap (Skybox)
    vector<const GLchar*> faces;
    faces.push_back("right.ppm");
    faces.push_back("left.ppm");
    faces.push_back("top.ppm");
    faces.push_back("bottom.ppm");
    faces.push_back("back.ppm");
    faces.push_back("front.ppm");
    cubemapTexture = loadCubemap(faces);
    
}

skybox::~skybox()
{
    // Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a
    // large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

}



void skybox::draw(GLuint shaderProgram)
{
    
    // Draw skybox first
    glUseProgram(shaderProgram);
    glDepthMask(GL_FALSE);// Remember to turn depth writing off
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 view = glm::mat4(glm::mat3(Window::V));
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &Window::P[0][0]);
    
    
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shaderProgram, "skybox"), 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    // Make sure no bytes are padded:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // Select GL_MODULATE to mix texture with polygon color for shading:
    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    // Use bilinear interpolation:
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Use clamp to edge to hide skybox edges:
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);
    
   
}

//void OBJObject::update()
//{
//    move();
//}

////Additional Feature Functions
//void OBJObject::orbit(float deg,vec3 axis)
//{
//    Window::V =  glm::rotate(glm::mat4(1.0f), deg/180.0f * glm::pi<float>(), glm::vec3(axis.x,axis.y, 0.0f)) * Window::V ;
//}
//
//void OBJObject::zoom(float factor)
//{
//    //Window::V =  translate(mat4(1.0f), vec3(0.0f,0.0f,factor)) * Window::V;
////    glm::vec3 direction = glm::normalize(Window::cam_look_at - Window::cam_pos);
////    Window::cam_pos = Window::cam_pos + direction/100.0f;
////    Window::V = glm::lookAt(Window::cam_pos, Window::cam_look_at, Window::cam_up);
////    cout << Window::V[0][0] << Window::V[0][1] << Window::V[0][0] << Window::V[0][2] << Window::V[0][3] << endl;
////     cout << Window::V[1][0] << Window::V[1][1] << Window::V[1][2] << Window::V[1][3] << endl;
////     cout << Window::V[2][0] << Window::V[2][1] << Window::V[2][2] << Window::V[2][3] << endl;
////     cout << Window::V[3][0] << Window::V[3][1] << Window::V[3][2] << Window::V[3][3] << endl;
//}
//
//void OBJObject::moveCam(float distance)
//{
//    Window::V =  translate(mat4(1.0f), vec3(distance,0.0f,0.0f)) * Window::V;
//}
//
//void OBJObject::move()
//{
//    toWorld = translate(mat4(1.0f), vec3(0.1f, 0.0f, 0.1f)) * toWorld;
//}
