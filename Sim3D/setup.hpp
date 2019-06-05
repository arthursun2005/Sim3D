//
//  setup.hpp
//  Sim3D
//
//  Created by Arthur Sun on 5/11/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#ifndef setup_hpp
#define setup_hpp

#include <float.h>
#include <cmath>
#include <vector>
#include <unistd.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void initBases();
void freeBases();

GLuint CreateProgram(const char * _vs[], const char * _fs[], const char* m);
GLuint CreateProgram(const char * _vs[], const char * _fs[], const char * _gs[], const char* m);
GLuint CreateProgram(const char * _cs[], const char* m);
GLuint LoadProgram(const char * _vs, const char * _fs, const char * _all);
GLuint LoadProgram(const char * _vs, const char * _fs, const char * _gs, const char * _all);
GLuint LoadProgram(const char * _cs, const char * _all);

void checkShader(GLuint x);
void checkProgram(GLuint x);

static GLuint texture_count = 0;

#define reset_texture_count (texture_count = 0)

void blit(GLuint target, GLuint x, GLuint y, GLuint w, GLuint h);

template <class T>
inline void memswap(T* a, T* b) {
    std::allocator<T> alloc;
    void* tp = alloc.allocate(1);
    size_t n = sizeof(T);
    memcpy(tp, (void*)a, n);
    memcpy((void*)a, (void*)b, n);
    memcpy((void*)b, tp, n);
    alloc.deallocate((T*)tp, 1);
}

struct Shader
{
    GLuint program;
    
    Shader() {}
    
    ~Shader() {
        glDeleteProgram(program);
    }
    
    void init(const char* cs, const char* all) {
        program = LoadProgram(cs, all);
    }
    
    void init(const char** vs, const char** fs) {
        program = CreateProgram(vs, fs, *fs);
    }
    
    void init(const char* vs, const char* fs, const char* all) {
        program = LoadProgram(vs, fs, all);
    }
    
    void init(const char* vs, const char* fs, const char* gs, const char* all) {
        program = LoadProgram(vs, fs, gs, all);
    }
    
    inline void bind() {
        glUseProgram(program);
    }
    
    inline void uniform4m(const char* n, const glm::mat4& m, bool T = GL_FALSE) const {
        glUniformMatrix4fv(glGetUniformLocation(program, n), 1, T, (float*)&m);
    }
    
    inline void uniform1i(const char* n, int i0) const
    {
        glUniform1i(glGetUniformLocation(program, n), i0);
    }
    
    inline void uniform2i(const char* n, int i0, int i1) const
    {
        glUniform2i(glGetUniformLocation(program, n), i0, i1);
    }
    
    inline void uniform3i(const char* n, int i0, int i1, int i2) const
    {
        glUniform3i(glGetUniformLocation(program, n), i0, i1, i2);
    }
    
    inline void uniform4i(const char* n, int i0, int i1, int i2, int i3) const
    {
        glUniform4i(glGetUniformLocation(program, n), i0, i1, i2, i3);
    }
    
    inline void uniform1ui(const char* n, unsigned int i0) const
    {
        glUniform1ui(glGetUniformLocation(program, n), i0);
    }
    
    inline void uniform2ui(const char* n, unsigned int i0, unsigned int i1) const
    {
        glUniform2ui(glGetUniformLocation(program, n), i0, i1);
    }
    
    inline void uniform3ui(const char* n, unsigned int i0, unsigned int i1, unsigned int i2) const
    {
        glUniform3ui(glGetUniformLocation(program, n), i0, i1, i2);
    }
    
    inline void uniform4ui(const char* n, unsigned int i0, unsigned int i1, unsigned int i2, unsigned int i3) const
    {
        glUniform4ui(glGetUniformLocation(program, n), i0, i1, i2, i3);
    }
    
    inline void uniform1f(const char* n, float f0) const
    {
        glUniform1f(glGetUniformLocation(program, n), f0);
    }
    
    inline void uniform2f(const char* n, float f0, float f1) const
    {
        glUniform2f(glGetUniformLocation(program, n), f0, f1);
    }
    
    inline void uniform3f(const char* n, float f0, float f1, float f2) const
    {
        glUniform3f(glGetUniformLocation(program, n), f0, f1, f2);
    }
    
    inline void uniform4f(const char* n, float f0, float f1, float f2, float f3) const
    {
        glUniform4f(glGetUniformLocation(program, n), f0, f1, f2, f3);
    }
};

struct Texture
{
    GLuint id;
    GLuint texture;
    GLuint fbo;
    
    Texture() {}
    
    Texture(const Texture& t) = delete;
    
    Texture& operator = (const Texture& t) = delete;
    
    ~Texture() {
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &texture);
    }
    
    void init(GLenum);
    
    void bind();
    
    inline void clear() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
};

struct Texture2D : public Texture
{
    void init(GLenum mode) {
        glGenFramebuffers(1, &fbo);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    void image(GLenum iformat, GLenum format, GLuint x, GLuint y, GLenum type, void* pixels)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, iformat, x, y, 0, format, type, pixels);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
        
#ifdef DEBUG
        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
#endif
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
    inline void bind() {
        id = texture_count++;
        glActiveTexture(GL_TEXTURE0 + id);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
};

struct Texture3D : public Texture
{
    void init(GLenum mode) {
        glGenFramebuffers(1, &fbo);
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_3D, texture);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, mode);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, mode);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_3D, 0);
    }
    
    void image(GLenum iformat, GLenum format, GLuint x, GLuint y, GLuint z, GLenum type, void* pixels)
    {
        glBindTexture(GL_TEXTURE_3D, texture);
        glTexImage3D(GL_TEXTURE_3D, 0, iformat, x, y, z, 0, format, type, pixels);
        
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
        
#ifdef DEBUG
        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
#endif
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        glBindTexture(GL_TEXTURE_3D, 0);
    }
    
    inline void bind() {
        id = texture_count++;
        glActiveTexture(GL_TEXTURE0 + id);
        glBindTexture(GL_TEXTURE_3D, texture);
    }
};

template <class T>
struct _DoubleT
{
    T textures[2];
    
    void init(GLenum mode) {
        textures[0].init(mode);
        textures[1].init(mode);
    }
    
    inline const T& operator [] (int i) const {
        return textures[i];
    }
    
    inline T& operator [] (int i) {
        return textures[i];
    }
    
    inline void clear() {
        textures[0].clear();
        textures[1].clear();
    }
    
    inline void swap() {
        std::swap(textures[0].id, textures[1].id);
        std::swap(textures[0].texture, textures[1].texture);
        std::swap(textures[0].fbo, textures[1].fbo);
    }
};

struct DoubleTexture2D : public _DoubleT<Texture2D> {
    void image(GLenum iformat, GLenum format, GLuint x, GLuint y, GLenum type, void* pixels) {
        textures[0].image(iformat, format, x, y, type, pixels);
        textures[1].image(iformat, format, x, y, type, pixels);
    }
};

struct DoubleTexture3D : public _DoubleT<Texture3D> {
    void image(GLenum iformat, GLenum format, GLuint x, GLuint y, GLuint z, GLenum type, void* pixels) {
        textures[0].image(iformat, format, x, y, z, type, pixels);
        textures[1].image(iformat, format, x, y, z, type, pixels);
    }
};

extern GLuint baseVAO;
extern GLuint baseVBO;

#endif /* setup_hpp */
