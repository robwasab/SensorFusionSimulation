//
//  OpenGLSquare.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/12/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLSquare.hpp"

OpenGLSquare::OpenGLSquare(float rgb[3],
                           const char texture_filename[]):
OpenGLCircle(4, rgb, texture_filename, false)
{
    finishInitializing();
}
