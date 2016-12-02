//
//  Water.cpp
//  CSE167_Final
//
//  Created by Jiasheng Zhu on 11/30/16.
//  Copyright © 2016 Xinghang Li. All rights reserved.
//

#include "Water.h"
#include "Window.h"


using namespace std;
using namespace glm;

unsigned char* usePPM(const char* filename, int& width, int& height)
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

GLuint useTexture(GLchar* path)
{
    //Generate texture ID and load texture data
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width,height;
    unsigned char* image = usePPM(path, width, height);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // Parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}


Water::Water()
{
    toWorld = mat4(1.0f);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(waterVertices), &waterVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindVertexArray(0);
    GLuint textureID = useTexture("./Textures/waterDUDV.ppm");
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(Window::cubeShader, "dudvTexture"), 1);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

Water::~Water()
{
    
}

void Water::draw(mat4 C)
{
    glUseProgram(Window::waterShader);
    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 modelview = Window::V * C *  toWorld;
    glm::mat4 model = toWorld;
    // We need to calcullate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
    // Consequently, we need to forward the projection, view, and model matrices to the shader programs
    // Get the location of the uniform variables "projection" and "modelview"
    // Now send these values to the shader program
    glUniformMatrix4fv(glGetUniformLocation(Window::waterShader, "projection"), 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(Window::waterShader, "model"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(Window::waterShader, "view"), 1, GL_FALSE, &Window::V[0][0]);
    glUniform3f(glGetUniformLocation(Window::waterShader, "cameraPos"), Window::cam_pos.x, Window::cam_pos.y, Window::cam_pos.z);
    glUniform1i(glGetUniformLocation(Window::waterShader, "dudvTexture"), 1);
    glUniform1f(glGetUniformLocation(Window::waterShader, "move"), move);
    // Now draw the cube. We simply need to bind the VAO associated with it.
    glBindVertexArray(VAO);
    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
}

void Water::update()
{
    move += 0.001;
    if(move >= 1.0) move = 0.0;
}
