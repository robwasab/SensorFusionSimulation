//
//  OpenGLSquare.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/12/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLSquare_hpp
#define OpenGLSquare_hpp

#include "OpenGLCircle.hpp"
#include <vector>

class OpenGLSquare: public OpenGLCircle
{
public:
    OpenGLSquare(float rgb[3]=NULL,
                 const char texture_filename[]=NULL);

};

#endif /* OpenGLSquare_hpp */
