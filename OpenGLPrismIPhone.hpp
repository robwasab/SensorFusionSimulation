//
//  OpenGLIPhonePrism.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/13/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLPrismIPhone_hpp
#define OpenGLPrismIPhone_hpp

#include <stdio.h>

#include "OpenGLPrism.hpp"

class OpenGLPrismIPhone: public OpenGLPrism
{
public:
    OpenGLPrismIPhone(glm::vec3 extrude_direction=glm::vec3(0.0, 1.0, 0.0),
                      float height=0.075f);
};
#endif /* OpenGLIPhonePrism_hpp */
