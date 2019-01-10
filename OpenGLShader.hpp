//
//  OpenGLShader.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 12/24/18.
//  Copyright © 2018 Robby Tong. All rights reserved.
//

#ifndef OpenGLShader_hpp
#define OpenGLShader_hpp

#include <stdio.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class OpenGLShader
{
public:
    OpenGLShader(const char vertex_shader_filename[], const char fragment_shader_filename[]);
    ~OpenGLShader();
    
    void use(void);
    void setBool (const char name[], bool  value) const;
    void setInt  (const char name[], int   value) const;
    void setFloat(const char name[], float value) const;
    void setVec3 (const char name[], float x, float y, float z) const;
    void setMat4 (const char name[], glm::mat4 & value) const;
private:
    GLuint mShaderProgram;
};

#endif /* OpenGLShader_hpp */
