//
//  OpenGLMath.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/11/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLMath.hpp"

using namespace glm;

mat3 OpenGLMath::generateCrossProductMatrix(glm::vec3 k)
{
    vec3 col0 = vec3( 0.0f,  k.z, -k.y);
    vec3 col1 = vec3( -k.z, 0.0f,  k.x);
    vec3 col2 = vec3(  k.y, -k.x, 0.0f);
    
    mat3 cross_product = mat3(col0, col1, col2);
    
    return cross_product;
}

mat3 OpenGLMath::calculateRotationMatrix2(vec3 a, vec3 b)
{
    vec3 a_cross_b = cross(a, b);
    float a_dot_b = dot(a, b);
    
    mat3 K = generateCrossProductMatrix(a_cross_b);
    
    mat3 R = mat3(1.0f) + K + (K * K * (1.0f / (1 + a_dot_b)));
    
    return R;
}

mat3 OpenGLMath::calculateRotationMatrix(vec3 a, vec3 b)
{
    vec3 a_cross_b = cross(a, b);
    float a_dot_b = dot(a, b);
    
    vec3 k = a_cross_b / length(a_cross_b);
    
    //float cross_theta = asin(length(a_cross_b) / (length(a) * length(b)));
    float dot_theta = acos(a_dot_b / (length(a) * length(b)));
    
    mat3 K = generateCrossProductMatrix(k);
    
    
    mat3 matrix_a = mat3(1.0f) + sin( dot_theta) * K + (1 - cos( dot_theta)) * K * K;
    mat3 matrix_b = mat3(1.0f) + sin(-dot_theta) * K + (1 - cos(-dot_theta)) * K * K;
    
    vec3 out_a = a*matrix_a;
    vec3 out_b = a*matrix_b;
    
    printf("out_a: [%.3f   %.3f   %.3f]\n", out_a.x, out_a.y, out_a.z);
    printf("out_b: [%.3f   %.3f   %.3f]\n", out_b.x, out_b.y, out_b.z);
    
    return matrix_a;
}
