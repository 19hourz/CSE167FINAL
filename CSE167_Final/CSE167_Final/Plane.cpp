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
GLfloat totalE = 0.0;
irrklang::ISoundEngine* planeEngine;
irrklang::ISound* idleSound;
irrklang::ISound* steadySound;
bool hasPlayedExplosionSound = false;
Plane::Plane()
{
    deg = 0.0;
    toWorld = mat4(1.0f);
    direction = normalize(vec3(0.0f, 0.0f, -1.0f));
    velocity = normalize(direction);
    upDirection = vec3(0.0f, 1.0f, 0.0f);
    center = vec3(0.0f, 0.2f, 0.0f);
    propeller1 = new Cube(10);
    propeller2 = new Cube(10);
    direction = normalize(direction);
    propeller = new OBJObject("./Glider.obj");
    planeEngine = irrklang::createIrrKlangDevice();
    idleSound = planeEngine->play2D("./media/idle.wav", true, false, true);
    steadySound = planeEngine->play2D("./media/steady.wav", true, false, true);
    idleSound->setVolume((3.0f-propellerSpeed)/3.0f);
    steadySound->setVolume(propellerSpeed/3.0f);
}

void Plane::draw(mat4 C)
{
    mat4 transform1 = translate(mat4(1.0f), vec3(0.005f, 0.075f, 0.0025f)) * rotate(mat4(1.0f), deg, oriDirec) * scale(mat4(1.0f), vec3(0.01,0.15,0.005)) * translate(mat4(1.0f), vec3(-0.5f, -0.5f, -0.5f));
    mat4 transform2 = translate(mat4(1.0f), vec3(0.005f, 0.075f, 0.0025f)) * rotate(mat4(1.0f), deg, oriDirec) * scale(mat4(1.0f), vec3(0.01,0.15,0.005)) * translate(mat4(1.0f), vec3(-0.5f, -0.5f, -0.5f));
    vec3 rotAxis = cross(upDirection, direction);
    rotAxis = normalize(rotAxis);
    transform1 =  translate(mat4(1.0f), vec3(0.2f*rotAxis.x + 0.145f*direction.x - 0.027*upDirection.x, 0.2f*rotAxis.y + 0.145f*direction.y - 0.027*upDirection.y, 0.2f*rotAxis.z + 0.145f*direction.z - 0.027*upDirection.z)) * translate(mat4(1.0f), center) * rotation * transform1;
    transform2 =  translate(mat4(1.0f), vec3(-0.2f*rotAxis.x + 0.145f*direction.x - 0.027*upDirection.x, -0.2f*rotAxis.y + 0.145f*direction.y - 0.027*upDirection.y, -0.2f*rotAxis.z + 0.145f*direction.z - 0.027*upDirection.z)) * translate(mat4(1.0f), center) * rotation * transform2;
    mat4 movement = translate(mat4(1.0f), center) * rotation;
    
    propeller->draw(movement);
    propeller1->draw(transform1);
    propeller2->draw(transform2);
}

void Plane::update()
{
    if (!pause) {
        idleSound->setVolume((3.0f-propellerSpeed)/4.0f+0.25f);
        steadySound->setVolume(propellerSpeed/3.0f);
        if (center.y < 0.2f) {
            center.y = 0.2;
        }
        GLfloat seudoHeight = 0.0;
        GLfloat seudoFriction = 0.0;
        if (center.y > 0.3f) {
            seudoHeight = 0.01 * direction.y;
        }
        if (center.y < 0.21f) {
            seudoFriction  = airSpeed * 0.5;
        }
        else{
            seudoFriction = airSpeed * airSpeed;
        }
        GLfloat delta = propellerSpeed * 0.01 - seudoFriction - seudoHeight;
        airSpeed += delta;
        if (airSpeed < 0) {
            airSpeed = 0.0;
        }
        if (deg < 360) {
            deg = deg+0.5*propellerSpeed;
        }
        else{
            deg = 0.0;
        }
        center += airSpeed * direction;
    }
    else{
        if (!hasPlayedExplosionSound) {
            planeEngine->play2D("./media/explosion.wav", false);
            hasPlayedExplosionSound = true;
        }
        idleSound->setVolume(0.0f);
        steadySound->setVolume(0.0f);
    }
}

GLfloat Plane::speedUp(GLfloat d)
{
    propellerSpeed += d;
    if (propellerSpeed >= MAX_THROTTLE) {
        propellerSpeed = MAX_THROTTLE;
    }
    return propellerSpeed;
}

GLfloat Plane::speedDown(GLfloat d)
{
    propellerSpeed -= d;
    
    if (propellerSpeed <= 0.0) {
        propellerSpeed = 0.0;
    }
    return propellerSpeed;
}

void Plane::up(GLfloat updeg)
{
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

void Plane::down(GLfloat updeg)
{
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

void Plane::rightRot(GLfloat rdeg)
{
    horizontalDeg+=rdeg;
    vec4 tempup = vec4(upDirection,0.0);
    tempup = rotate(mat4(1.0f), rdeg * pi<GLfloat>() /180.0f, direction) * tempup;
    upDirection = vec3(tempup.x, tempup.y, tempup.z);
    rotation = rotate(mat4(1.0f), rdeg * pi<GLfloat>() /180.0f, direction) * rotation;
}

void Plane::leftRot(GLfloat rdeg)
{
    horizontalDeg-=rdeg;
    vec4 tempup = vec4(upDirection,0.0);
    tempup = rotate(mat4(1.0f), rdeg * pi<GLfloat>() /180.0f, -direction) * tempup;
    upDirection = vec3(tempup.x, tempup.y, tempup.z);
    rotation = rotate(mat4(1.0f), -rdeg * pi<GLfloat>() /180.0f, direction) * rotation;
}

vec3 Plane::getCenter(){
    return center;

}

void Plane::reset(){
    hasPlayedExplosionSound = false;
    pause = false;
    propellerSpeed = 0.0;
    airSpeed = 0.0;
    deg = 0.0;
    toWorld = mat4(1.0f);
    direction = normalize(vec3(0.0f, 0.0f, -1.0f));
    velocity = normalize(direction);
    upDirection = vec3(0.0f, 1.0f, 0.0f);
    center = vec3(0.0f, 0.2f, 0.0f);
    rotation = mat4(1.0f);
    oriDirec = vec3(0.0f, 0.0f, -1.0f);
}








