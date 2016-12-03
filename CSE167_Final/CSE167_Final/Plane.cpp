#include "Plane.h"
#include "Window.h"

using namespace std;
using namespace glm;
OBJObject *propeller;
Cube *propeller1;
Cube *propeller2;
mat4 propeller1Setup;
mat4 propeller2Setup;
mat4 rotation = mat4(1.0f);
vec3 oriDirec = vec3(0.0f, 0.0f, -1.0f);
GLfloat horizontalDeg = 0.0f;
GLfloat verticalDeg = 0.0f;
Plane::Plane()
{
    deg = 0.0;
    toWorld = mat4(1.0f);
    direction = normalize(vec3(0.0f, 0.0f, -1.0f));
    upDirection = vec3(0.0f, 1.0f, 0.0f);
    center = vec3(0.0f, 1.5f, 0.0f);
    propeller1 = new Cube(10);
    propeller2 = new Cube(10);
    direction = normalize(direction);
    propeller = new OBJObject("./Glider.obj");
}

void Plane::draw(mat4 C)
{
    mat4 transform1 = translate(mat4(1.0f), vec3(0.005f, 0.075f, 0.0025f)) * rotate(mat4(1.0f), deg, oriDirec) * scale(mat4(1.0f), vec3(0.01,0.15,0.005)) * translate(mat4(1.0f), vec3(-0.5f, -0.5f, -0.5f));
    mat4 transform2 = translate(mat4(1.0f), vec3(0.005f, 0.075f, 0.0025f)) * rotate(mat4(1.0f), deg, oriDirec) * scale(mat4(1.0f), vec3(0.01,0.15,0.005)) * translate(mat4(1.0f), vec3(-0.5f, -0.5f, -0.5f));
    vec3 rotAxis = cross(upDirection, direction);
    rotAxis = normalize(rotAxis);
    transform1 =  translate(mat4(1.0f), vec3(0.2f*rotAxis.x + 0.145f*direction.x - 0.03*upDirection.x, 0.2f*rotAxis.y + 0.145f*direction.y - 0.03*upDirection.y, 0.2f*rotAxis.z + 0.145f*direction.z - 0.03*upDirection.z)) * translate(mat4(1.0f), center) * rotation * transform1;
    transform2 =  translate(mat4(1.0f), vec3(-0.2f*rotAxis.x + 0.145f*direction.x - 0.03*upDirection.x, -0.2f*rotAxis.y + 0.145f*direction.y - 0.03*upDirection.y, -0.2f*rotAxis.z + 0.145f*direction.z - 0.03*upDirection.z)) * translate(mat4(1.0f), center) * rotation * transform2;
    mat4 movement = translate(mat4(1.0f), center) * rotation;
    
    propeller->draw(movement);
    propeller1->draw(transform1);
    propeller2->draw(transform2);
}

void Plane::update()
{
    if (deg < 360) {
        deg = deg+0.5*propellerSpeed;
    }
    else{
        deg = 0.0;
    }
    center += airSpeed * direction;
}

GLfloat Plane::speedUp()
{
    airSpeed += 0.01;
    if (airSpeed>=0.2) {
        airSpeed = 0.2;
    }
    return airSpeed;
}

GLfloat Plane::speedDown()
{
    airSpeed -= 0.1;
    if (airSpeed<=0.0) {
        airSpeed = 0.0;
    }
    return airSpeed;
}

void Plane::up()
{
    GLfloat updeg = 1;
    verticalDeg+=updeg;
    vec3 right = cross(direction, upDirection);
    vec4 tempup = vec4(upDirection,0.0);
    vec4 tempdirec = vec4(direction, 0.0);
    tempup = rotate(mat4(1.0f), updeg * pi<GLfloat>() /180.0f, right) * tempup;
    tempdirec = rotate(mat4(1.0f), updeg * pi<GLfloat>() /180.0f, right) * tempdirec;
    direction = vec3(tempdirec.x, tempdirec.y, tempdirec.z);
    direction = normalize(direction);
    upDirection = vec3(tempup.x, tempup.y, tempup.z);
    vec3 rotAxis = cross(direction, upDirection);
    rotation = rotate(mat4(1.0f), updeg * pi<GLfloat>() /180.0f, rotAxis) * rotation;
}

void Plane::down()
{
    GLfloat updeg = 1;
    verticalDeg-=updeg;
    vec3 left = cross(upDirection, direction);
    vec4 tempup = vec4(upDirection,0.0);
    vec4 tempdirec = vec4(direction, 0.0);
    tempup = rotate(mat4(1.0f), updeg * pi<GLfloat>() /180.0f, left) * tempup;
    tempdirec = rotate(mat4(1.0f), updeg * pi<GLfloat>() /180.0f, left) * tempdirec;
    direction = vec3(tempdirec.x, tempdirec.y, tempdirec.z);
    direction = normalize(direction);
    upDirection = vec3(tempup.x, tempup.y, tempup.z);
    vec3 rotAxis = cross(upDirection, direction);
    rotation = rotate(mat4(1.0f), updeg * pi<GLfloat>() /180.0f, rotAxis) * rotation;
}

void Plane::rightRot()
{
    GLfloat rdeg = 1;
    horizontalDeg+=rdeg;
    vec4 tempup = vec4(upDirection,0.0);
    tempup = rotate(mat4(1.0f), rdeg * pi<GLfloat>() /180.0f, direction) * tempup;
    upDirection = vec3(tempup.x, tempup.y, tempup.z);
    rotation = rotate(mat4(1.0f), rdeg * pi<GLfloat>() /180.0f, direction) * rotation;
}

void Plane::leftRot()
{
    GLfloat rdeg = 1;
    horizontalDeg-=rdeg;
    vec4 tempup = vec4(upDirection,0.0);
    tempup = rotate(mat4(1.0f), rdeg * pi<GLfloat>() /180.0f, -direction) * tempup;
    upDirection = vec3(tempup.x, tempup.y, tempup.z);
    rotation = rotate(mat4(1.0f), -rdeg * pi<GLfloat>() /180.0f, direction) * rotation;
}
