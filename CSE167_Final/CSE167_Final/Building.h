#ifndef _CUBE_H_
#define _CUBE_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Geode.h"

class Building :public Geode{
protected:
    
    
public:
    Building(int cubeType);
    ~Building();
    
    glm::mat4 toWorld;
    
    void draw(mat4) override;
    void update() override;
    void spin();
    //    void move();
    
    // These variables are needed for the shader program
    GLuint VBO, VAO, EBO;
    GLuint CAO, CBO;
    GLuint uProjection, uModelview;
    GLint CubeShader; 
};


#endif

