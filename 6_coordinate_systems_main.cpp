//
//  6_coordinate_systems_main.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 12/30/18.
//  Copyright © 2018 Robby Tong. All rights reserved.
//

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLShader.hpp"
#include "utility.hpp"
#include "stb_image.hpp"
#include "OpenGLMeshSquare.hpp"
#include "OpenGLMeshTriangle.hpp"
#include "OpenGLMeshCube.hpp"
#include "OpenGLSphere.hpp"
#include "OpenGLCone.hpp"
#include "OpenGLPrismCylinder.hpp"
#include "OpenGLArrow.hpp"
#include "OpenGLAxis.hpp"
#include "OpenGLMath.hpp"

class MyOpenGLDelegate: public OpenGLDelegate
{
private:
    OpenGLMeshSquare * mSquare;
    OpenGLMeshCube * mCube;
    OpenGLSphere * mSphere;
    OpenGLCone * mCone;
    OpenGLPrismCylinder * mCylinder;
    OpenGLArrow * mArrow;
    OpenGLAxis * mAxis;
    
    OpenGLSphere * mLightSource;
public:
    
    MyOpenGLDelegate():
    OpenGLDelegate(800, 800)
    {
        
    }
    
    const char * windowName(void)
    {
        return "Textures";
    }
    
    void windowClearColor(float * red, float * green, float * blue, float * alpha)
    {
        *red   = 0.2f;
        *green = 0.3f;
        *blue  = 0.3f;
        *alpha = 1.0f;
    }
    
    void initialized(GLFWwindow * window)
    {
        mSquare = new OpenGLMeshSquare();
        mSquare->setScale(0.5);
        
        mCube = new OpenGLMeshCube();
        mCube->setScale(0.5);
        
        mSphere = new OpenGLSphere(30, OpenGLPrimitive::COLOR_BLUE, TEXTURE_EARTH);
        mSphere->setScale(0.5);
        
        mCone = new OpenGLCone(50, OpenGLPrimitive::COLOR_PURPLE, TEXTURE_WOODEN_CONTAINER);
        mCone->setScale(0.5);
        
        glm::vec3 extrude_direction = glm::vec3(0.0f, 1.0f, 0.0f);
        float height = 1.0f;
        
        mCylinder = new OpenGLPrismCylinder(extrude_direction, height, OpenGLPrimitive::COLOR_GREEN, TEXTURE_WOODEN_CONTAINER);
        mCylinder->setScale(0.25, 0.5, 0.25);
        
        mArrow = new OpenGLArrow(OpenGLPrimitive::COLOR_RED);
        mArrow->setScale(0.25f, 1.0f, 0.25f);
        
        mAxis = new OpenGLAxis();
        mAxis->setScale(0.75);
        
        mLightSource = new OpenGLSphere(20, OpenGLPrimitive::COLOR_YELLOW);
        mLightSource->setScale(0.10);
        mLightSource->setPosition(2, 1, -4);
        
        mArrow->setVectorDimensions(2, 1, -4);
    }
    
    void windowFrameBufferResized(GLFWwindow * window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }
    
    void updateFrame(GLFWwindow * window, double cursor_posx, double cursor_posy)
    {
        // move back 5 units
        //setCameraOrientation(30, glm::vec3(1.0, 0.0, 0.0));
        
        glm::mat4 view_transform = getViewTransform();
        
        mLightSource->drawWithShaderAndTransform(view_transform);
        
        //mSquare->drawWithShaderAndTransform(view_transform);
        //mCube->drawWithShaderAndTransform(view_transform);
        mSphere->drawWithShaderAndTransform(view_transform);
        mSphere->drawNormalsWithTransform(view_transform);
        
        //mCone->drawWithShaderAndTransform(view_transform);
        //mCylinder->drawWithShaderAndTransform(view_transform);
        //mArrow->drawWithShaderAndTransform(view_transform);
        //mAxis->drawWithShaderAndTransform(view_transform);
    }
};


int main(int argc, const char * argv[])
{
    MyOpenGLDelegate delegate;
    
    Utility_startOpenglWithDelegate(&delegate);
    return 0;
}
