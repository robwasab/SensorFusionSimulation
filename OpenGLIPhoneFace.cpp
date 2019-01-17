//
//  OpenGLIPhoneFace.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/12/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLIPhoneFace.hpp"

OpenGLIPhoneFace::OpenGLIPhoneFace():
OpenGLSquare(OpenGLPrimitive::COLOR_GREY, TEXTURE_IPHONE_FACE)
{
    setOrientation(180, ROTATION_ZAXIS);
    addOrientation(180, ROTATION_YAXIS);
}
