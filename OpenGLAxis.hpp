//
//  OpenGLAxis.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/7/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLAxis_hpp
#define OpenGLAxis_hpp

#include <stdio.h>
#include "OpenGLComposite.hpp"
#include "OpenGLArrow.hpp"

class OpenGLAxis: public OpenGLComposite
{
private:
    OpenGLArrow * mXAxis;
    OpenGLArrow * mYAxis;
    OpenGLArrow * mZAxis;
    
public:
    
    OpenGLAxis();
    ~OpenGLAxis();
};

#endif /* OpenGLAxis_hpp */
