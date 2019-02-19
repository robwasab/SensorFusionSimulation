//
//  OpenGLText.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/26/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLText_hpp
#define OpenGLText_hpp

#include <stdio.h>
#include <glm/glm.hpp>
#include <glad/glad.h>

class OpenGLText
{
private:
    glm::mat4 mProjection;
    GLuint mVAO;
    GLuint mVBO;
    float * mRgb;
    float mFontHeight;
    float mOriginX;
    float mOriginY;
    float mWindowDimX;
    float mWindowDimY;
    
    static OpenGLText * mOpenGLText;
    
    // Get OpenGLText object from static method
    OpenGLText(float window_size_x, float window_size_y);
    
public:
    static OpenGLText * getOpenGLTextWithWindowDimensions(float window_size_x, float window_size_y);
    
    // because text drawing is based on parameters related to the screen, such
    // as pixels, you have to tell OpenGLText about the window dimensions to
    // calculate the internal orthographic transform.
    void windowResizedTo(float size_x, float size_y);
    
    void setColor(float * rgb);
    
    void setFontHeight(float height);
    
    void setBottomLeftCornerOrigin(float xorigin, float yorigin);
    
    float calculateStringWidth(const char text[]);
    
    void drawStringWithTransform(const char text[], glm::mat4 transform);
    
    ~OpenGLText();
};

#endif /* OpenGLText_hpp */
