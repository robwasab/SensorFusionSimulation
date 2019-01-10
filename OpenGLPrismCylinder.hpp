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
    
public:
    OpenGLPrismCylinder(glm::vec3 extrude_direction=glm::vec3(0.0, 1.0, 0.0),
                        float height=1.0f,
                        float rgb[3]=NULL,
                        const char texture_filename[]=NULL);
};

#endif /* OpenGLPrismCylinder_hpp */
