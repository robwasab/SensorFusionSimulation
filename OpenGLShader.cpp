//
//  OpenGLShader.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 12/24/18.
//  Copyright © 2018 Robby Tong. All rights reserved.
//

#include "OpenGLShader.hpp"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

static GLuint compile_shader_source(const char * filename, GLenum shader_source_type);
static void read_shader_file(const char * filename, char * buffer, size_t len);
static GLuint link_shaders_to_program(GLuint vertex_shader, GLuint fragment_shader);


OpenGLShader::OpenGLShader(const char vertex_shader_filename[], const char fragme_shader_filename[])
{
    GLuint vertex_shader;
    GLuint fragme_shader;
    
    vertex_shader = compile_shader_source(vertex_shader_filename, GL_VERTEX_SHADER);
    fragme_shader = compile_shader_source(fragme_shader_filename, GL_FRAGMENT_SHADER);
    
    mShaderProgram = link_shaders_to_program(vertex_shader, fragme_shader);
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragme_shader);
}

OpenGLShader::~OpenGLShader()
{
    glDeleteProgram(mShaderProgram);
}

void OpenGLShader::use(void)
{
    glUseProgram(mShaderProgram);
}
void OpenGLShader::setBool(const char name[], bool  value) const
{
    GLint uniform_id = glGetUniformLocation(mShaderProgram, name);
    glUniform1i(uniform_id, value);
}

void OpenGLShader::setInt(const char name[], int   value) const
{
    GLint uniform_id = glGetUniformLocation(mShaderProgram, name);
    glUniform1i(uniform_id, value);
}

void OpenGLShader::setFloat(const char name[], float value) const
{
    GLint uniform_id = glGetUniformLocation(mShaderProgram, name);
    glUniform1f(uniform_id, value);
}

void OpenGLShader::setVec3(const char name[], float x, float y, float z) const
{
    GLint uniform_id = glGetUniformLocation(mShaderProgram, name);
    glUniform3f(uniform_id, x, y, z);
}

void OpenGLShader::setMat4(const char name[], glm::mat4 & value) const
{
    GLint uniform_id = glGetUniformLocation(mShaderProgram, name);
    glUniformMatrix4fv(uniform_id, 1, GL_FALSE, glm::value_ptr(value));
}

static GLuint compile_shader_source(const char * filename, GLenum shader_source_type)
{
    GLuint shader_id;
    GLchar info_log[512];
    GLchar shader_source[512];
    
    read_shader_file(filename, shader_source, sizeof(shader_source));
    
    shader_id = glCreateShader(shader_source_type);
    
    const GLchar * shader_source_ptr = shader_source;
    
    glShaderSource(shader_id, 1, &shader_source_ptr, NULL);
    glCompileShader(shader_id);
    
    GLint success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    
    if(0 == success)
    {
        glGetShaderInfoLog(shader_id, sizeof(info_log), NULL, info_log);
        printf("------------------------\n");
        printf("SHADER COMPILATION ERROR\n");
        printf("------------------------\n");
        printf("%s\n", info_log);
        exit(-1);
    }
    
    printf("Compile success!\n");
    return shader_id;
}

static void read_shader_file(const char * filename, char * buffer, size_t len)
{
    FILE * shader_file = fopen(filename, "r");
    assert(shader_file != NULL);
    
    memset(buffer, 0, len);
    
    size_t bytes_read = fread(buffer, len, 1, shader_file);
    
    assert(bytes_read <= len);
    
#if 0
    printf("--------------------\n");
    printf("SHADER FILE CONTENTS\n");
    printf("--------------------\n");
    printf("%s\n", buffer);
#endif
}

static GLuint link_shaders_to_program(GLuint vertex_shader, GLuint fragment_shader)
{
    GLuint shader_program;
    shader_program = glCreateProgram();
    
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    
    char info_log[512] = {0};
    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(0 == success)
    {
        glGetProgramInfoLog(shader_program, sizeof(info_log), NULL, info_log);
        printf("------------------\n");
        printf("PROGRAM LINK ERROR\n");
        printf("------------------\n");
        printf("%s\n", info_log);
        exit(-1);
    }
    
    printf("Program link success!\n");
    
    return shader_program;
}

