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
#include "OpenGLComposite.hpp"
#include "OpenGLCone.hpp"
#include "OpenGLPrismCylinder.hpp"

class OpenGLArrow: public OpenGLComposite
{
private:
    OpenGLCone * mArrowHead;
    OpenGLPrismCylinder * mArrowBody;
    
public:
    OpenGLArrow(float rgb[3], float arrow_head_to_body_ratio=0.3);
    ~OpenGLArrow();
};

#endif /* OpenGLArrow_hpp */
