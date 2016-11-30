#ifndef _BUILDING_H_
#define _BUILDING_H_

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
#include "Cube.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <vector>

class Cube;

class Building :public Geode{
protected:
    std::vector<glm::mat4> blocksPos;
    
public:
    Building();
    ~Building();
    
    glm::mat4 toWorld;

    void draw(mat4) override;
    void update() override;
    void randBuilding();
    

};


#endif

