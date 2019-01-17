//
//  OpenGLIPhonePrism.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/13/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLPrismIPhone.hpp"
#include "OpenGLIPhoneFace.hpp"
#include "OpenGLIPhoneBack.hpp"

class IPhoneFaceGenerator: public OpenGLPrism::PrismFaceGenerator
{
    int mCounter;
public:
    IPhoneFaceGenerator()
    {
        mCounter = 0;
    }
    
    OpenGLPrimitive * getNextFace(float rgb[3], const char texture_filename[])
    {
        switch (mCounter)
        {
            case 0:
                mCounter = 1;
                return new OpenGLIPhoneBack();
                
                
            case 1:
                mCounter = 0;
                return new OpenGLIPhoneFace();
                
            default:
                assert(false);
        }
    }
};

OpenGLPrismIPhone::OpenGLPrismIPhone(glm::vec3 extrude_direction,
                                     float height):
OpenGLPrism(new IPhoneFaceGenerator(),
            extrude_direction,
            height,
            OpenGLPrimitive::COLOR_GREY,
            NULL)
{
    setScale(1, 2.0, 2.0);
}
