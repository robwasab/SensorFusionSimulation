//
//  OpenGLIPhoneBack.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/12/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLIPhoneBack.hpp"

OpenGLIPhoneBack::OpenGLIPhoneBack():
OpenGLSquare(OpenGLPrimitive::COLOR_GREY, TEXTURE_IPHONE_BACK)
{
    addOrientation(180, ROTATION_YAXIS);
}
