#include "Cube.h"
#include "Window.h"

//#define VERTEX_CUBESHADER_PATH "CubeShader.vert"
//#define FRAGMENT_CUBESHADER_PATH "CubeShader.frag"

using namespace std;

Cube::Cube(int type)
{
    
    GLfloat vertices[] = {
        // Positions          // Texture Coords
        0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        0.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        0.0f,  0.0f, 0.0f,  0.0f, 0.0f,
        
        0.0f, 0.0f,  1.0f,  0.0f, 0.0f,
        1.0f, 0.0f,  1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
        1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
        0.0f,  1.0f,  1.0f,  0.0f, 1.0f,
        0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
        
        0.0f,  1.0f,  1.0f,  1.0f, 0.0f,
        0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.0f, 0.0f, 0.0f,   0.0f, 1.0f,
        0.0f, 0.0f, 0.0f,   0.0f, 1.0f,
        0.0f, 0.0f,  1.0f,  0.0f, 0.0f,
        0.0f,  1.0f,  1.0f,  1.0f, 0.0f,
        
        1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,   1.0f, 1.0f,
        1.0f,  0.0f, 0.0f,   0.0f, 1.0f,
        1.0f,  0.0f, 0.0f,   0.0f, 1.0f,
        1.0f, 0.0f,  1.0f,  0.0f, 0.0f,
        1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
        
        0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        1.0f,  0.0f, 0.0f,   1.0f, 1.0f,
        1.0f, 0.0f,  1.0f,  1.0f, 0.0f,
        1.0f, 0.0f,  1.0f,  1.0f, 0.0f,
        0.0f, 0.0f,  1.0f,  0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        
        0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
        1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
        0.0f,  1.0f,  1.0f,  0.0f, 0.0f,
        0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    
    this->cubeType = type;
    countSpin = 0;
    angle = 1.0f;
    toWorld = glm::mat4(1.0f);

    
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

    
    // Cubes
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(Window::cubeShader, "ourTexture"), 0);
    if(cubeType == 1) glBindTexture(GL_TEXTURE_2D, Window::buildingTexture);
    else if(cubeType == 2) glBindTexture(GL_TEXTURE_2D, Window::cityTexture1);
    else if(cubeType == 3) glBindTexture(GL_TEXTURE_2D, Window::cityTexture2);
    else if(cubeType == 4) glBindTexture(GL_TEXTURE_2D, Window::cityTexture3);
    else if(cubeType == 5) glBindTexture(GL_TEXTURE_2D, Window::cityTexture4);
    else if(cubeType == 20) glBindTexture(GL_TEXTURE_2D, Window::buildingTexture);
    else if(cubeType == 21) glBindTexture(GL_TEXTURE_2D, Window::buildingTexture1);
    else if(cubeType == 22) glBindTexture(GL_TEXTURE_2D, Window::buildingTexture2);
    else if(cubeType == 23) glBindTexture(GL_TEXTURE_2D, Window::buildingTexture3);
    else if(cubeType == 24) glBindTexture(GL_TEXTURE_2D, Window::buildingTexture4);
    else if(cubeType == 25) glBindTexture(GL_TEXTURE_2D, Window::buildingTexture5);
    else if(cubeType == 26) glBindTexture(GL_TEXTURE_2D, Window::buildingTexture6);
    else if(cubeType == 27) glBindTexture(GL_TEXTURE_2D, Window::buildingTexture7);
    else if(cubeType == 28) glBindTexture(GL_TEXTURE_2D, Window::buildingTexture8);



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

void Cube::setCubeType(int type){
    cubeType = type;
}

//void Cube::move()
//{
//    toWorld = translate(mat4(1.0f), vec3(0.1f,0.0f,0.1f)) * toWorld;
//}

