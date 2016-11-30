#ifndef _PLANE_H_
#define _PLANE_H_

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


class Plane :public Geode{
protected:
    glm::vec3 center;
    glm::vec3 direction;
public:
    Plane();
    ~Plane();
    
    glm::mat4 toWorld;
    
    void draw(mat4) override;
    void update() override;
};


#endif
