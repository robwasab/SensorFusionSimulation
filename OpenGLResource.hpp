//
//  OpenGLResource.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/11/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLResource_hpp
#define OpenGLResource_hpp

#include <stdio.h>
#include <typeinfo>

class OpenGLResource
{
private:
public:
    bool isResourceCached(std::type_info object);
    
};

#endif /* OpenGLResource_hpp */
