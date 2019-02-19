//
//  OpenGLPrismCylinder.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/7/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLPrismCylinder_hpp
#define OpenGLPrismCylinder_hpp

#include <stdio.h>
#include "OpenGLPrism.hpp"

class OpenGLPrismCylinder: public OpenGLPrism
{
private:
    float mThickness;
    glm::mat4 mVectorTranslation;
    glm::mat4 mVectorTransformation;

public:
    OpenGLPrismCylinder(float rgb[3]=NULL);
    
    OpenGLPrismCylinder(glm::vec3 extrude_direction=glm::vec3(0.0, 1.0, 0.0),
                        float height=1.0f,
                        float rgb[3]=NULL,
                        const char texture_filename[]=NULL);
    
    virtual void setPosition(float x, float y, float z) override;
    virtual void addPosition(float x, float y, float z) override;
    glm::mat4 getTransformationMatrix(void) override;

    void setThickness(float thickness);
    void setVectorDimensions(float x, float y, float z);
};

#endif /* OpenGLPrismCylinder_hpp */
