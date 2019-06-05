//
//  World.hpp
//  Sim3D
//
//  Created by Arthur Sun on 5/11/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#ifndef World_hpp
#define World_hpp

#include "setup.hpp"
#include "SphereGeometry.h"

class World
{
    
public:
    
    float w0 = 1.0f;
    
    World(int, int, int, int cap = 1048576);
    
    ~World();
    
    void draw(GLuint t, GLuint x, GLuint y, GLuint w, GLuint h, const glm::mat4& m, float size = 0.5f) {
        positions[1].bind();
        velocities[1].bind();
        
        sphereShader.bind();
        sphereShader.uniform2f("ts", root, root);
        sphereShader.uniform1f("size", size);
        sphereShader.uniform4m("m", m);
        sphereShader.uniform1i("T", positions[1].id);
        
        float power = 70000.0f;
        sphereShader.uniform1f("power", power);
        sphereShader.uniform3f("light", 200.0f, 120.0f, 0.0f);
        sphereShader.uniform4f("lightColour", 1.0f, 1.0f, 1.0f, 1.0f);
        sphereShader.uniform4f("colour", 0.3f, 0.5f, 0.9f, 1.0f);
        sphereShader.uniform1f("minPower", 0.125f/power);
        
        sphereShader.uniform1i("V", velocities[1].id);
        sphereShader.uniform1f("vel", 0.01f);
        
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        
        glBindFramebuffer(GL_FRAMEBUFFER, t);
        glBindVertexArray(sphereVAO);
        
        glViewport(x, y, w, h);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIndices);
        glDrawElementsInstanced(GL_TRIANGLES, (int)sphereGeometry.indices.size(), GL_UNSIGNED_INT, 0, count);
        
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        
        reset_texture_count;
    }
    
    void setSphereGeometry(int stacks, int sectors) {
        sphereGeometry.create(stacks, sectors);

        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
        glBufferData(GL_ARRAY_BUFFER, sphereGeometry.vertices.size() * sizeof(glm::vec3), sphereGeometry.vertices.data(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereIndices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphereGeometry.indices.size() * sizeof(unsigned int), sphereGeometry.indices.data(), GL_STATIC_DRAW);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    inline int createCubeN(const glm::vec3& _p, const glm::vec3& _d, float _s) {
        glm::vec3 d = glm::vec3(width, height, depth);
        return createCube(_p * d, 2.0f * _d * d, _s);
    }
    
    int createCube(const glm::vec3& _p, const glm::vec3& _d, float _s) {
        int w = roundf(_d.x/_s);
        int h = roundf(_d.y/_s);
        int d = roundf(_d.z/_s);
        
        int i = w * h * d;
        
        if(count + i > capacity)
            throw "OUT OF RANGE";
        
        cubeShader.bind();
        cubeShader.uniform1f("sp", _s);
        cubeShader.uniform3f("pos", _p.x, _p.y, _p.z);
        cubeShader.uniform1i("hx", w);
        cubeShader.uniform1i("hy", h);
        cubeShader.uniform1i("hz", d);
        cubeShader.uniform1i("count", count);
        cubeShader.uniform1i("root", root);
        
        positions[1].bind();
        blit(positions[1], count, i);
        
        count += i;
        
        reset_texture_count;
        
        return i;
    }
    
    inline int getCount() const
    {
        return count;
    }
    
    inline void reset() {
        grid.clear();
        gWeights.clear();
        pressures.clear();
        velocities.clear();
        
        count = 0;
    }
    
    inline void step(float dt, int its) {
        float _dt = dt/(float)its;
        for(int i = 0; i < its; ++i)
            step(_dt);
    }
    
    inline void step(float dt) {
        mapToGrid();
                
        extendGrids();
        
        copyToTemp();
        
        gridForces(dt);
        
        weightParticles();
        
        solveDivergence();
                
        for(int n = 0; n < pressure_iterations; ++n)
            solvePressure();
        
        solveGrid();
        
        transfer();
        
        stepParticles(dt);
    }
    
private:
    
    void stepParticles(float dt);
    
    void mapToGrid();
    
    void average();
    
    void transfer();
    
    void copyToTemp();
    
    void solvePressure();
    
    void solveDivergence();
    
    void weightParticles();
    
    void gridForces(float dt);
    
    void solveGrid();
    
    void extendGrids();
    
    void advectGrid(float dt);
    
    inline void blit(Texture& tex, int start, int count) {
        glBindFramebuffer(GL_FRAMEBUFFER, tex.fbo);
        glBindVertexArray(vao);
        glViewport(0, 0, root, root);
        glDrawArrays(GL_POINTS, start, count);
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    inline void blit3(Texture& tex) {
        glBindFramebuffer(GL_FRAMEBUFFER, tex.fbo);
        glBindVertexArray(layerVAO);
        glViewport(0, 0, width, height);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, depth);
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    inline void blit4(Texture& tex) {
        glBindFramebuffer(GL_FRAMEBUFFER, tex.fbo);
        glBindVertexArray(layerVAO);
        glViewport(0, 0, width + 1, height + 1);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, depth + 1);
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
    void initTextures();
    void initVAOs();
    void initShaders();
    
    SphereGeometry sphereGeometry;
    
    int count;
    int capacity;
    
    int root;
    
    int pressure_iterations = 30;
    
    const GLuint width;
    const GLuint height;
    const GLuint depth;
    
    const float invWidth;
    const float invHeight;
    const float invDepth;
    
    const glm::vec3 invGrid;
    
    glm::vec3 gravity = glm::vec3(0.0f, -200.0f, 0.0f);
    
    GLuint vao;
    GLuint vbo;
    
    GLuint layerVAO;
    
    GLuint sphereVAO;
    GLuint sphereVBO;
    GLuint sphereIndices;
    
    DoubleTexture2D positions;
    DoubleTexture2D velocities;
    
    DoubleTexture3D weights;
    
    DoubleTexture3D pressures;
    DoubleTexture3D grid;
    Texture3D grid2;
    Texture3D gWeights;
    Texture3D divergences;
    
    Shader sphereShader;
    Shader cubeShader;
    Shader stepShader;
    Shader pressureShader;
    Shader gridForceShader;
    Shader divergenceShader;
    Shader weightShader;
    Shader subtractShader;
    Shader advectShader;
    Shader extendShader;
    Shader mapToGridShader;
    Shader velocityShader;
    Shader averageShader;
    Shader copyShader;
};

#endif /* World_hpp */
