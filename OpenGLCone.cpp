//
//  OpenGLCone.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/5/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLCone.hpp"
#include "OpenGLConehat.hpp"
#include "OpenGLCircle.hpp"

OpenGLCone::OpenGLCone(int pnts_per_revolution, float rgb[3], const char texture_filename[])
{
    OpenGLConehat * hat = new OpenGLConehat(pnts_per_revolution, rgb, texture_filename);
    OpenGLCircle * base = new OpenGLCircle(pnts_per_revolution, rgb, texture_filename);
    
    addDrawable(hat);
    addDrawable(base);
}
