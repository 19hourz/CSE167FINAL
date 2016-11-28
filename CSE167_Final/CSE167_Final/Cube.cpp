#include "Cube.h"
#include "Window.h"

//#define VERTEX_CUBESHADER_PATH "CubeShader.vert"
//#define FRAGMENT_CUBESHADER_PATH "CubeShader.frag"

using namespace std;



// Note that GL_QUADS is deprecated in modern OpenGL (and removed from OSX systems).
// This is why we need to draw each face as 2 triangles instead of 1 quadrilateral
GLuint indices[6][6] = {
    // Front face
    {0, 1, 2, 2, 3, 0},
    // Top face
    {1, 5, 6, 6, 2, 1},
    // Back face
    {7, 6, 5, 5, 4, 7},
    // Bottom face
    {4, 0, 3, 3, 7, 4},
    // Left face
    {4, 5, 1, 1, 0, 4},
    // Right face
    {3, 2, 6, 6, 7, 3}
};



Cube::Cube(int type)
{
    GLfloat vertices[8][3] = {
//        // "Front" vertices
//        {-1.0, -1.0,  1.0}, {1.0, -1.0,  1.0}, {1.0,  1.0,  1.0}, {-1.0,  1.0,  1.0},
//        // "Back" vertices
//        {-1.0, -1.0, -1.0}, {1.0, -1.0, -1.0}, {1.0,  1.0, -1.0}, {-1.0,  1.0, -1.0}
        
        
        // "Front" vertices
        {0.0, 0.0,  2.0}, {2.0, 0.0,  2.0}, {2.0,  2.0,  2.0}, {0.0,  2.0,  2.0},
        // "Back" vertices
        {0.0, 0.0, 0.0}, {2.0, 0.0, 0.0}, {2.0, 2.0, 0.0}, {0.0,  2.0, 0.0}
        
        
    };
    this->cubeType = type;
    countSpin = 0;
    angle = 1.0f;
    toWorld = glm::mat4(1.0f);
    if(cubeType == 2) angle = -angle;
//    if(cubeType == 1 || cubeType == 2){
//        for(int i = 0; i < 8; ++i){
//            vertices[i][0] = vertices[i][0] * 0.05;
//            vertices[i][1] = vertices[i][1] * 1.0;
//            vertices[i][2] = vertices[i][2] * 0.05;
//        }
//    }
//
    
    // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
    // Consider the VAO as a container for all your buffers.
    glBindVertexArray(VAO);
    
    // Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
    // you want to draw, such as vertices, normals, colors, etc.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
    // the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Enable the usage of layout location 0 (check the vertex shader to see what this is)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    
    // We've sent the vertex data over to OpenGL, but there's still something missing.
    // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO now so we don't accidentally tamper with it.
    // NOTE: You must NEVER unbind the element array buffer associated with a VAO!
    glBindVertexArray(0);
    
    //Bound Sphere
    int SEGMENTS = 10;
    GLfloat r;
    if (type == 1 || type == 2){
        r = 2;
    }
    else{
        r = 4;
    }
    
    for(int j = -SEGMENTS/2; j < 5 * SEGMENTS/2; j++){
        float dist = glm::abs(2.0 * r * j / (GLfloat)SEGMENTS);
        float tempr = glm::sqrt(r*r - dist*dist);
        for (int i = 0; i <= 4 * SEGMENTS; i++){
            float angle = 2 * glm::pi<float>() * i / SEGMENTS;
            float next_angle = 2 * glm::pi<float>() * (i + 1) / SEGMENTS;
            float x1 = cos(angle) * tempr;
            float y1 = sin(angle) * tempr;
            float x2 = cos(next_angle) * tempr;
            float y2 = sin(next_angle) * tempr;
            points.push_back(glm::vec3(x1, y1, 2.0 * r * j / (GLfloat)SEGMENTS));
            points.push_back(glm::vec3(x2, y2, 2.0 * r * j / (GLfloat)SEGMENTS));
        }
    }
    glBindVertexArray(0);
    glGenVertexArrays(1, &CAO);
    glGenBuffers(1, &CBO);
    glBindVertexArray(CAO);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, points.size()*sizeof(GLfloat), &points[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(GLfloat),
                          (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
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
    //    glDisable(GL_CULL_FACE);
    //    glCullFace(GL_FRONT);
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
    // Now draw the cube. We simply need to bind the VAO associated with it.
    glBindVertexArray(VAO);
    // Tell OpenGL to draw with triangles, using 36 indices, the type of the indices, and the offset to start from
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
    
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

