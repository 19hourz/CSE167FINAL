//
//  Geode.hpp
//  CSE167_PA3
//
//  Created by Ryan Li on 10/29/16.
//  Copyright © 2016 Ryan Li. All rights reserved.
//

#ifndef Geode_hpp
#define Geode_hpp

#include <stdio.h>
#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Node.h"

using namespace glm;



class Geode :public Node
{
protected:
    char* name;
    
public:
    double boundRadius;
    glm::vec4 center;
    GLint shaderProgram;
    bool skip;
    Geode();
    ~Geode();
    virtual void draw(mat4) = 0;
};

#endif /* Geode_hpp */
