//
//  MatrixTransform.hpp
//  CSE167_PA3
//
//  Created by Ryan Li on 10/29/16.
//  Copyright © 2016 Ryan Li. All rights reserved.
//

#ifndef MatrixTransform_hpp
#define MatrixTransform_hpp

#include <stdio.h>
#include "Group.h"
#include <glm/mat4x4.hpp>
#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Node.h"


class MatrixTransform :
public Group
{
protected:
    mat4 M;
    GLuint robot_cao, robot_cbo;
    std::vector<glm::vec3> points;
public:
    glm::vec3 center;
    GLfloat radius;
    bool hasCulling;
    MatrixTransform(mat4);
    MatrixTransform(GLfloat radius, glm::vec3 center);
    ~MatrixTransform();
    void draw(mat4) override;
    void update() override;
    void setM(mat4);
    
    mat4 getM();
};







#endif /* MatrixTransform_hpp */
