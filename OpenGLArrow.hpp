//
//  OpenGLArrow.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/7/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLArrow_hpp
#define OpenGLArrow_hpp

#include <stdio.h>
#include <glm/glm.hpp>

#include "OpenGLComposite.hpp"
#include "OpenGLCone.hpp"
#include "OpenGLPrismCylinder.hpp"

class OpenGLArrow: public OpenGLComposite
{
private:
    OpenGLCone * mArrowHead;
    OpenGLPrismCylinder * mArrowBody;
    
    glm::mat4 mVectorTranslation;
    glm::mat4 mVectorTransformation;
    
public:
    OpenGLArrow(float rgb[3], float arrow_head_to_body_ratio=0.3);
    ~OpenGLArrow();
    
    virtual void setPosition(float x, float y, float z) override;
    virtual void addPosition(float x, float y, float z) override;
    virtual glm::mat4 getTransformationMatrix(void) override;
    
    void setVectorDimensions(float x, float y, float z);
    
};

#endif /* OpenGLArrow_hpp */
