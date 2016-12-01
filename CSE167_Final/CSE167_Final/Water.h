//
//  Water.hpp
//  CSE167_Final
//
//  Created by Jiasheng Zhu on 11/30/16.
//  Copyright © 2016 Xinghang Li. All rights reserved.
//

#ifndef Water_h
#define Water_h

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
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Geode.h"
#include "Cube.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <vector>


class Water :public Geode{
private:
    GLuint ududv;
    GLfloat waterLevel = 0.0f;
    const GLfloat waterVertices[18] = {
        // Positions
        0.0f, 0.0f, 0.0f,
        100.0f,  0.0f, 0.0f,
        100.0f, 0.0f,  100.0f,
        100.0f, 0.0f,  100.0f,
        0.0f, 0.0f,  100.0f,
        0.0f, 0.0f, 0.0f
    };
public:
    GLuint VBO, VAO;
    Water();
    ~Water();
    glm::mat4 toWorld;
    void draw(mat4) override;
    void update() override;
};


#endif
