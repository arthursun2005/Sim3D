//
//  World.cpp
//  Sim3D
//
//  Created by Arthur Sun on 5/11/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#include "World.hpp"
#include <iostream>
#include <array>

World::World(int w, int h, int d, int cap) : count(0), width(w), height(h), depth(d), invWidth(1.0f/(float)w), invHeight(1.0f/(float)h), invDepth(1.0f/(float)d), invGrid(1.0f/(float)(w + 1), 1.0f/(float)(h + 1), 1.0f/(float)(d + 1)) {
    root = ceilf(sqrt((double)cap));
    capacity = root * root;
    
    initVAOs();
    initTextures();
    initShaders();
    
    grid.clear();
    gWeights.clear();
    pressures.clear();
    velocities.clear();
    
    std::cout << "Initialized World Object" << std::endl;
    std::cout << "Sim size: " << w << " * " << h <<  " * " << d << std::endl;
    std::cout << "Capacity: " << capacity << std::endl;
}

void World::initTextures() {
    positions.init(GL_NEAREST);
    positions.image(GL_RGB32F, GL_RGB, root, root, GL_FLOAT, 0);
    
    velocities.init(GL_NEAREST);
    velocities.image(GL_RGB32F, GL_RGB, root, root, GL_FLOAT, 0);
    
    pressures.init(GL_NEAREST);
    pressures.image(GL_R32F, GL_RED, width, height, depth, GL_FLOAT, 0);
    
    weights.init(GL_LINEAR);
    weights.image(GL_R32F, GL_RED, width, height, depth, GL_FLOAT, 0);
    
    grid.init(GL_LINEAR);
    grid.image(GL_RGB32F, GL_RGB, width + 1, height + 1, depth + 1, GL_FLOAT, 0);
    
    grid2.init(GL_LINEAR);
    grid2.image(GL_RGB32F, GL_RGB, width + 1, height + 1, depth + 1, GL_FLOAT, 0);
    
    gWeights.init(GL_LINEAR);
    gWeights.image(GL_RGB32F, GL_RGB, width + 1, height + 1, depth + 1, GL_FLOAT, 0);
    
    divergences.init(GL_NEAREST);
    divergences.image(GL_R32F, GL_RED, width, height, depth, GL_FLOAT, 0);
}

void World::initShaders() {
    sphereShader.init("glsl/sphere.vs", "glsl/sphere.fs", "glsl/shared.glsl");
    cubeShader.init("glsl/texture2d.vs", "glsl/cube.fs", "glsl/shared.glsl");
    stepShader.init("glsl/texture2d.vs", "glsl/step.fs", "glsl/shared.glsl");
    pressureShader.init("glsl/layer.vs", "glsl/pressure.fs", "glsl/layer.gs", "glsl/shared.glsl");
    gridForceShader.init("glsl/layer.vs", "glsl/gridForce.fs", "glsl/layer.gs", "glsl/shared.glsl");
    divergenceShader.init("glsl/layer.vs", "glsl/divergence.fs", "glsl/layer.gs", "glsl/shared.glsl");
    weightShader.init("glsl/weight.vs", "glsl/weight.fs", "glsl/weight.gs", "glsl/shared.glsl");
    subtractShader.init("glsl/layer.vs", "glsl/subtract.fs", "glsl/layer.gs", "glsl/shared.glsl");
    advectShader.init("glsl/layer.vs", "glsl/advect.fs", "glsl/layer.gs", "glsl/shared.glsl");
    extendShader.init("glsl/layer.vs", "glsl/extend.fs", "glsl/layer.gs", "glsl/shared.glsl");
    mapToGridShader.init("glsl/mapToGrid.vs", "glsl/mapToGrid.fs", "glsl/mapToGrid.gs", "glsl/shared.glsl");
    velocityShader.init("glsl/texture2d.vs", "glsl/transfer.fs", "glsl/shared.glsl");
    averageShader.init("glsl/layer.vs", "glsl/average.fs", "glsl/layer.gs", "glsl/shared.glsl");
    copyShader.init("glsl/layer.vs", "glsl/copy3.fs", "glsl/layer.gs", "glsl/shared.glsl");
}

void World::initVAOs() {
    glGenVertexArrays(1, &vao);
    glGenVertexArrays(1, &layerVAO);
    glGenBuffers(1, &vbo);
    
    glGenVertexArrays(1, &sphereVAO);
    glGenBuffers(1, &sphereVBO);
    glGenBuffers(1, &sphereIndices);
    
    std::vector<float> texels(2 * capacity);
    
    for(int py = 0; py < root; ++py) {
        for(int px = 0; px < root; ++px) {
            int i = (px + py * root) * 2;
            texels[i + 0] = (px + 0.5f) / (float)root;
            texels[i + 1] = (py + 0.5f) / (float)root;
        }
    }
    
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 2 * capacity * sizeof(float), texels.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(sphereVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    
    glVertexAttribDivisor(0, 1);
    glVertexAttribDivisor(1, 0);
    
    glBindVertexArray(layerVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, baseVBO);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribDivisor(0, 0);
    
    glBindVertexArray(0);
}

World::~World() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &layerVAO);
    
    glDeleteVertexArrays(1, &sphereVAO);
    glDeleteBuffers(1, &sphereVBO);
    glDeleteBuffers(1, &sphereIndices);
}

void World::mapToGrid() {
    grid[1].clear();
    gWeights.clear();
    
    grid[1].bind();
    velocities[1].bind();
    positions[1].bind();
    
    mapToGridShader.bind();
    mapToGridShader.uniform1i("P", positions[1].id);
    mapToGridShader.uniform1i("V", velocities[1].id);
    mapToGridShader.uniform3f("size", width, height, depth);
    
    glBindFramebuffer(GL_FRAMEBUFFER, grid[1].fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, gWeights.texture, 0);
    
#ifdef DEBUG
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
#endif
    
    GLenum bfs[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, bfs);
    
    glEnable(GL_BLEND);
    
    glBindVertexArray(vao);
    glViewport(0, 0, width + 1, height + 1);
    
    mapToGridShader.uniform1i("x", 1);
    mapToGridShader.uniform1i("y", 0);
    mapToGridShader.uniform1i("z", 0);
    
    mapToGridShader.uniform1i("d", 0);
    glDrawArrays(GL_POINTS, 0, count);
    
    mapToGridShader.uniform1i("d", 1);
    glDrawArrays(GL_POINTS, 0, count);
    
    mapToGridShader.uniform1i("x", 0);
    mapToGridShader.uniform1i("y", 1);
    mapToGridShader.uniform1i("z", 0);
    
    mapToGridShader.uniform1i("d", 0);
    glDrawArrays(GL_POINTS, 0, count);
    
    mapToGridShader.uniform1i("d", 1);
    glDrawArrays(GL_POINTS, 0, count);
    
    mapToGridShader.uniform1i("x", 0);
    mapToGridShader.uniform1i("y", 0);
    mapToGridShader.uniform1i("z", 1);
    
    mapToGridShader.uniform1i("d", 0);
    glDrawArrays(GL_POINTS, 0, count);
    
    mapToGridShader.uniform1i("d", 1);
    glDrawArrays(GL_POINTS, 0, count);
    
    glBindVertexArray(0);
    
    glDisable(GL_BLEND);
    
    reset_texture_count;
    
    glDrawBuffers(1, bfs);
    
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, 0, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void World::transfer() {
    velocities[1].bind();
    positions[1].bind();
    grid[1].bind();
    grid2.bind();
    
    velocityShader.bind();
    velocityShader.uniform1i("P", positions[1].id);
    velocityShader.uniform1i("V", velocities[1].id);
    velocityShader.uniform1i("G", grid[1].id);
    velocityShader.uniform1i("G2", grid2.id);
    velocityShader.uniform3f("size", width, height, depth);
    
    velocities[0].bind();
    blit(velocities[0], 0, count);
    
    velocities.swap();
    
    reset_texture_count;
}

void World::stepParticles(float dt) {
    positions[0].bind();
    positions[1].bind();
    grid[1].bind();
    
    stepShader.bind();
    stepShader.uniform1i("P", positions[1].id);
    stepShader.uniform1i("V", grid[1].id);
    stepShader.uniform1f("dt", dt);
    stepShader.uniform3f("size", width, height, depth);
    
    blit(positions[0], 0, count);
    
    positions.swap();
    
    reset_texture_count;
}

void World::solvePressure() {
    pressures[0].bind();
    pressures[1].bind();
    divergences.bind();
    weights[1].bind();
    
    pressureShader.bind();
    pressureShader.uniform1i("P", pressures[1].id);
    pressureShader.uniform1i("D", divergences.id);
    pressureShader.uniform1i("W", weights[1].id);
    pressureShader.uniform3f("invSize", invWidth, invHeight, invDepth);
    
    blit3(pressures[0]);
    
    pressures.swap();
    
    reset_texture_count;
}

void World::solveDivergence() {
    divergences.bind();
    grid[1].bind();
    weights[1].bind();
    
    divergenceShader.bind();
    divergenceShader.uniform1i("V", grid[1].id);
    divergenceShader.uniform1i("W", weights[1].id);
    divergenceShader.uniform1f("w0", w0);
    
    blit3(divergences);
    
    reset_texture_count;
}

void World::weightParticles() {
    weights[1].clear();
    
    weights[1].bind();
    positions[1].bind();
    
    weightShader.bind();
    weightShader.uniform1i("T", positions[1].id);
    weightShader.uniform3f("scl", invWidth, invHeight, depth);
    
    glEnable(GL_BLEND);
    
    glBindFramebuffer(GL_FRAMEBUFFER, weights[1].fbo);
    glBindVertexArray(vao);
    glViewport(0, 0, width, height);
    
    weightShader.uniform1i("z", 0);
    glDrawArrays(GL_POINTS, 0, count);
    
    weightShader.uniform1i("z", 1);
    glDrawArrays(GL_POINTS, 0, count);
    
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    glDisable(GL_BLEND);
    
    reset_texture_count;
}

void World::gridForces(float dt) {
    grid[0].bind();
    grid[1].bind();
    
    gridForceShader.bind();
    gridForceShader.uniform1i("V", grid[1].id);
    gridForceShader.uniform3f("exf", gravity.x, gravity.y, gravity.z);
    gridForceShader.uniform1f("dt", dt);
    gridForceShader.uniform3f("size", width + 1.0f, height + 1.0f, depth + 1.0f);
    
    blit4(grid[0]);
    
    grid.swap();
    
    reset_texture_count;
}

void World::solveGrid() {
    grid[0].bind();
    grid[1].bind();
    pressures[1].bind();
    
    subtractShader.bind();
    subtractShader.uniform1i("V", grid[1].id);
    subtractShader.uniform1i("U", pressures[1].id);
    subtractShader.uniform3f("invSize", invWidth, invHeight, invDepth);
    
    blit4(grid[0]);
    
    grid.swap();
    
    reset_texture_count;
}

void World::advectGrid(float dt) {
    grid[0].bind();
    grid[1].bind();
    
    advectShader.bind();
    advectShader.uniform1i("V", grid[1].id);
    advectShader.uniform1i("T", grid[1].id);
    advectShader.uniform1f("dt", dt);
    advectShader.uniform3f("invSize", invWidth, invHeight, invDepth);
    
    blit4(grid[0]);
    
    grid.swap();
    
    reset_texture_count;
}

void World::extendGrids() {
    grid[0].bind();
    grid[1].bind();
    gWeights.bind();
    
    extendShader.bind();
    extendShader.uniform1i("V", grid[1].id);
    extendShader.uniform1i("W", gWeights.id);
    extendShader.uniform3f("invSize", invGrid.x, invGrid.y, invGrid.z);
    
    blit4(grid[0]);
    
    grid.swap();
    
    reset_texture_count;
}

void World::average() {
    grid[0].bind();
    grid[1].bind();
    gWeights.bind();
    
    averageShader.bind();
    averageShader.uniform1i("V", grid[1].id);
    averageShader.uniform1i("W", gWeights.id);
    averageShader.uniform3f("invSize", invGrid.x, invGrid.y, invGrid.z);
    
    blit4(grid[0]);
    
    grid.swap();
    
    reset_texture_count;
}

void World::copyToTemp() {
    grid2.bind();
    grid[1].bind();
    
    copyShader.bind();
    copyShader.uniform1i("T", grid[1].id);
    
    blit4(grid2);
    
    reset_texture_count;
}
