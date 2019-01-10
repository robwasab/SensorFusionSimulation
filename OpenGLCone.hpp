//
//  OpenGLCone.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/5/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLCone_hpp
#define OpenGLCone_hpp

#include <stdio.h>

#include "OpenGLComposite.hpp"

class OpenGLCone: public OpenGLComposite
{
private:
public:
    OpenGLCone(int pnts_per_revolution=50, float rgb[3]=NULL, const char texture_filename[]=NULL);
};

#endif /* OpenGLCone_hpp */
