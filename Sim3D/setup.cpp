//
//  setup.cpp
//  Sim3D
//
//  Created by Arthur Sun on 5/11/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#include "setup.hpp"

#include <fstream>
#include <sstream>

GLuint baseVAO;
GLuint baseVBO;

const GLfloat baseQuad[] = {
    -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f
};

GLuint CreateProgram(const char * _vs[], const char * _fs[], const char* m) {
    GLuint programID = glCreateProgram();
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vs, 1, _vs, nullptr);
    glShaderSource(fs, 1, _fs, nullptr);
    glCompileShader(vs);
    glCompileShader(fs);
    checkShader(vs);
    checkShader(fs);
    glAttachShader(programID, vs);
    glAttachShader(programID, fs);
    glLinkProgram(programID);
    checkProgram(programID);
    glDetachShader(programID, vs);
    glDetachShader(programID, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    printf("compiled %s \n", m);
    
    return programID;
}

GLuint CreateProgram(const char * _vs[], const char * _fs[], const char * _gs[], const char* m) {
    GLuint programID = glCreateProgram();
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint gs = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(vs, 1, _vs, nullptr);
    glShaderSource(fs, 1, _fs, nullptr);
    glShaderSource(gs, 1, _gs, nullptr);
    glCompileShader(vs);
    glCompileShader(fs);
    glCompileShader(gs);
    checkShader(vs);
    checkShader(fs);
    checkShader(gs);
    glAttachShader(programID, vs);
    glAttachShader(programID, fs);
    glAttachShader(programID, gs);
    glLinkProgram(programID);
    checkProgram(programID);
    glDetachShader(programID, vs);
    glDetachShader(programID, fs);
    glDetachShader(programID, gs);
    glDeleteShader(vs);
    glDeleteShader(fs);
    glDeleteShader(gs);
    
    printf("compiled %s \n", m);
    
    return programID;
}

GLuint CreateProgram(const char * _cs[], const char* m) {
    GLuint programID = glCreateProgram();
    GLuint cs = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(cs, 1, _cs, nullptr);
    glCompileShader(cs);
    checkShader(cs);
    glAttachShader(programID, cs);
    glLinkProgram(programID);
    glDetachShader(programID, cs);
    glDeleteShader(cs);
    
    checkProgram(programID);
    printf("compiled %s \n", m);
    
    return programID;
}

GLuint LoadProgram(const char * _vs, const char * _fs, const char * _all) {
    std::ifstream vs(_vs, std::ios::in);
    std::ifstream fs(_fs, std::ios::in);
    std::ifstream all(_all, std::ios::in);
    
    std::stringstream vstr, fstr, astr;
    
    bool failed = false;
    
    if(vs.is_open()) {
        vstr << vs.rdbuf();
        vs.close();
    }else{
        printf("%s can't be opened \n", _vs);
        failed = true;
    }
    
    if(fs.is_open()) {
        fstr << fs.rdbuf();
        fs.close();
    }else{
        printf("%s can't be opened \n", _fs);
        failed = true;
    }
    
    if(_all && all.is_open()) {
        astr << all.rdbuf();
        all.close();
    }else if(_all) {
        printf("%s can't be opened \n", _all);
        failed = true;
    }
    
    if(failed)
        return 0;
    
    std::string __v = astr.str();
    std::string __f(__v);
    std::string __vs = vstr.str();
    std::string __fs = fstr.str();
    
    __v.append(__vs);
    __f.append(__fs);
    
    const char* fvs = __v.c_str();
    const char* ffs = __f.c_str();
    
    return CreateProgram(&fvs, &ffs, _fs);
}

GLuint LoadProgram(const char * _vs, const char * _fs, const char * _gs, const char * _all) {
    std::ifstream vs(_vs, std::ios::in);
    std::ifstream fs(_fs, std::ios::in);
    std::ifstream gs(_gs, std::ios::in);
    std::ifstream all(_all, std::ios::in);
    
    std::stringstream vstr, fstr, gstr, astr;
    
    bool failed = false;
    
    if(vs.is_open()) {
        vstr << vs.rdbuf();
        vs.close();
    }else{
        printf("%s can't be opened \n", _vs);
        failed = true;
    }
    
    if(fs.is_open()) {
        fstr << fs.rdbuf();
        fs.close();
    }else{
        printf("%s can't be opened \n", _fs);
        failed = true;
    }
    
    if(gs.is_open()) {
        gstr << gs.rdbuf();
        gs.close();
    }else{
        printf("%s can't be opened \n", _gs);
        failed = true;
    }
    
    if(_all && all.is_open()) {
        astr << all.rdbuf();
        all.close();
    }else if(_all) {
        printf("%s can't be opened \n", _all);
        failed = true;
    }
    
    if(failed)
        return 0;
    
    std::string __v = astr.str();
    std::string __f(__v);
    std::string __g(__v);
    std::string __vs = vstr.str();
    std::string __fs = fstr.str();
    std::string __gs = gstr.str();
    
    __v.append(__vs);
    __f.append(__fs);
    __g.append(__gs);
    
    const char* fvs = __v.c_str();
    const char* ffs = __f.c_str();
    const char* fgs = __g.c_str();
    
    return CreateProgram(&fvs, &ffs, &fgs, _fs);
}

GLuint LoadProgram(const char * _cs, const char * _all) {
    std::ifstream cs(_cs, std::ios::in);
    std::ifstream all(_all, std::ios::in);
    
    std::stringstream cstr, astr;
    
    bool failed = false;
    
    if(cs.is_open()) {
        cstr << cs.rdbuf();
        cs.close();
    }else{
        printf("%s can't be opened \n", _cs);
        failed = true;
    }
    
    if(_all && all.is_open()) {
        astr << all.rdbuf();
        all.close();
    }else if(_all) {
        printf("%s can't be opened \n", _all);
        failed = true;
    }
    
    if(failed)
        return 0;
    
    std::string __c = astr.str();
    std::string __cs = cstr.str();
    
    __c.append(__cs);
    
    const char* fcs = __c.c_str();
    
    return CreateProgram(&fcs, _cs);
}

void checkShader(GLuint x) {
    int status;
    glGetShaderiv(x, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        int length;
        glGetShaderiv(x, GL_INFO_LOG_LENGTH, &length);
        GLchar log[length];
        glGetShaderInfoLog(x, length, nullptr, log);
        printf("%s \n", log);
    }
}

void checkProgram(GLuint x) {
    int status;
    glGetProgramiv(x, GL_LINK_STATUS, &status);
    if(status == GL_FALSE) {
        int length;
        glGetProgramiv(x, GL_INFO_LOG_LENGTH, &length);
        GLchar log[length];
        glGetProgramInfoLog(x, length, nullptr, log);
        printf("%s \n", log);
    }
}

void initBases() {
    glGenVertexArrays(1, &baseVAO);
    glGenBuffers(1, &baseVBO);
    
    glBindVertexArray(baseVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, baseVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(baseQuad), baseQuad, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindVertexArray(0);
}

void freeBases() {
    glDeleteVertexArrays(1, &baseVAO);
    glDeleteBuffers(1, &baseVBO);
}

void blit(GLuint target, GLuint x, GLuint y, GLuint w, GLuint h) {
    glBindFramebuffer(GL_FRAMEBUFFER, target);
    glBindVertexArray(baseVAO);
    glViewport(x, y, w, h);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
