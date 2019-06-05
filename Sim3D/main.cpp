//
//  main.cpp
//  Sim3D
//
//  Created by Arthur Sun on 5/11/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#include <iostream>
#include "World.hpp"

GLFWwindow *window;
const GLFWvidmode* mode;

double mouseX, mouseY;
double pmouseX = mouseX, pmouseY = mouseY;
GLuint width = 1280;
GLuint height = 840;
float dt = 0.016f;
float scl = 0.01f;

#ifdef DEBUG

int framesPerSecond = 0;
float lastSecondTime = glfwGetTime();

#endif

World* world;

float angleY = 0.5f;
float angleX = -M_PI * 0.5f;

float dMouseX = 0.0f;
float dMouseY = 0.0f;

#define mouseFriction 0.91f

glm::vec3 eye(cosf(angleY) * cosf(angleX), sinf(angleY), cosf(angleY) * sinf(angleX));

void addFromMouse() {
    float k = 2.0f / scl;
}

void moveCameraFromMouse() {
    angleY += dMouseY * 0.01f;
    angleX += dMouseX * 0.01f;
    float p2 = M_PI * 0.5f;
    if(angleY >= p2) angleY = p2 - FLT_EPSILON;
    if(angleY <= -p2) angleY = -p2 + FLT_EPSILON;
    float cy = cos(angleY);
    eye = glm::vec3(cy * cosf(angleX), sinf(angleY), cy * sinf(angleX));
}

void mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
}

float sp = 1.0f;
float siz = sp * 0.5f;
float w0 = 8.0f / (sp * sp * sp);

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    float x = 0.7f, y = 0.6f;
    
    if(action == GLFW_RELEASE) {
        if(key == GLFW_KEY_A)
            addFromMouse();
        
        if(key == GLFW_KEY_K)
            world->createCubeN(glm::vec3(0.0f, 0.9f, 0.0f), glm::vec3(1.0f, 0.1f, 1.0f), sp);
        
        if(key == GLFW_KEY_G)
            world->createCubeN(glm::vec3(0.0f, -0.9f, 0.0f), glm::vec3(1.0f, 0.1f, 1.0f), sp);
        
        if(key == GLFW_KEY_D)
            world->createCubeN(glm::vec3(x, y, 0.0f), glm::vec3(1.0f - x, 1.0f - y, 1.0f), sp);
        
        if(key == GLFW_KEY_S)
            world->createCubeN(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.2f), sp);
        
        if(key == GLFW_KEY_M)
            world->createCubeN(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), sp);
        
        if(key == GLFW_KEY_R)
            world->reset();
        
        if(key == GLFW_KEY_N) {
            std::cout << world->getCount() << std::endl;
        }
    }
}

void initialize() {
    initBases();
    
    world = new World(64, 128, 64);
    world->setSphereGeometry(4, 4);
    
    world->w0 = w0;
}

void free() {
    freeBases();
    
    delete world;
}

void draw() {
    world->step(dt, 1);
    
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), width/(float)height, 0.5f, 10000.0f);

    glm::mat4 view = glm::lookAt(eye / scl, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    glm::mat4 finalMatrix = projection * view;

    world->draw(0, 0, 0, width * 2, height * 2, finalMatrix, siz);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    scl *= exp(yoffset * 0.001f);
}

int main(int argc, const char * argv[]) {
    srand((unsigned int)time(NULL));
    if(!glfwInit()) return 1;
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    mode = glfwGetVideoMode(monitor);

    window = glfwCreateWindow(width, height, "Sim3D", NULL, NULL);
    
    glfwMakeContextCurrent(window);
    
    glewInit();
    
    const unsigned char* version = glGetString(GL_VERSION);
    
    printf("%s\n", version);
    
    initialize();
    
    glfwSetMouseButtonCallback(window, mouseCallback);
    
    GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
    
    glfwSetCursor(window, cursor);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);

    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    
    glDisable(GL_BLEND);
    
    glDepthFunc(GL_LESS);
    
    mouseX = width * 0.5f;
    mouseY = height * 0.5f;
    
    do {
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glfwGetCursorPos(window, &mouseX, &mouseY);
        
        float currentTime = glfwGetTime();
        bool press = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        
#ifdef DEBUG
        ++framesPerSecond;
        if(currentTime - lastSecondTime >= 1.0f) {
            printf("%f ms/frame \n", 1000.0f * (currentTime - lastSecondTime)/(float)framesPerSecond);
            framesPerSecond = 0;
            lastSecondTime = currentTime;
        }
#endif
        
        if(press) {
            dMouseX = (mouseX - pmouseX);
            dMouseY = (mouseY - pmouseY);
        }else{
            dMouseX *= mouseFriction;
            dMouseY *= mouseFriction;
        }
        
        moveCameraFromMouse();
        
        draw();
        
        glfwPollEvents();
        glfwSwapBuffers(window);
        
        pmouseX = mouseX;
        pmouseY = mouseY;
    } while (glfwWindowShouldClose(window) == GL_FALSE && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS);
    free();
    glfwDestroyCursor(cursor);
    glfwTerminate();
    return 0;
}
