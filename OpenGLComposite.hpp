//
//  OpenGLComposite.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/1/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLComposite_hpp
#define OpenGLComposite_hpp

#include <vector>
#include <stdio.h>
#include "OpenGLDrawable.hpp"

class OpenGLComposite: public OpenGLDrawable
{
private:
    std::vector<OpenGLDrawable *> * mDrawables;
public:
    OpenGLComposite();
    ~OpenGLComposite();
    
    void addDrawable(OpenGLDrawable * drawable);
    void drawWithShaderAndTransform(glm::mat4 transform);
};

#endif /* OpenGLComposite_hpp */
