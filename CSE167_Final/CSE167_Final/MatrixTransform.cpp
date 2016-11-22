//
//  MatrixTransform.cpp
//  CSE167_PA3
//
//  Created by Ryan Li on 10/29/16.
//  Copyright © 2016 Ryan Li. All rights reserved.
//

#include "MatrixTransform.h"
#include "Window.h"


using namespace std;

MatrixTransform::MatrixTransform(mat4 M)
{
    this->M = M;
}

MatrixTransform::~MatrixTransform()
{
}

MatrixTransform::MatrixTransform(GLfloat radius, glm::vec3 center){
    M = glm::mat4(1.0f);
    this->radius = radius;
    this->center = center;
    hasCulling = false;
    int SEGMENTS = 20;
    GLfloat r = radius;
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
            points.push_back(center + glm::vec3(x1, y1, 2.0 * r * j / (GLfloat)SEGMENTS));
            points.push_back(center + glm::vec3(x2, y2, 2.0 * r * j / (GLfloat)SEGMENTS));
        }
    }
    //glBindVertexArray(0);
    glGenVertexArrays(1, &robot_cao);
    glGenBuffers(1, &robot_cbo);
    glBindVertexArray(robot_cao);
    glBindBuffer(GL_ARRAY_BUFFER, robot_cbo);
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

void MatrixTransform::draw(mat4 C)
{


}

void MatrixTransform::setM(mat4 m)
{
    M = m;
}

mat4 MatrixTransform::getM(){
    return M;
}

void MatrixTransform::update(){
    for (Node* child : children)
    {
        //std::cout << "update" << endl;
        child->update();
    }
}

