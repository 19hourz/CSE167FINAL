#ifndef OBJOBJECT_H
#define OBJOBJECT_H

/*
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif


//#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>*/

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
#include <vector>


class OBJObject
{
private:


public:
    //Public variables
    std::vector<unsigned int> indices;
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    glm::mat4 toWorld;
    float angle;
    float pointSize;
    int square_point_size;
    
    unsigned int target_obj;
    // These variables are needed for the shader program
    GLuint VBO, VAO, EBO, VBO_Normals;
    GLuint uProjection, uModelview, uModel;
    GLint matAmbientLoc,matDiffuseLoc,matSpecularLoc,matShineLoc;
    GLint lightPosLoc;
    GLint viewPosLoc;
    GLint lightAmbientLoc, lightDiffuseLoc, lightSpecularLoc, lightDirectionLoc;
        
    //Functions
	OBJObject(const char* filepath);
    ~OBJObject();
	void parse(const char* filepath);
	void draw(glm::mat4);
    void update();
    void spin(float);
    void move(float,float,float);
    void scale(float);
    void orbit(float,glm::vec3);
    void reset();
    
    void smaller_pointSize();
    void larger_pointSize();
   
   
    void reset_pointSize();
};

#endif
