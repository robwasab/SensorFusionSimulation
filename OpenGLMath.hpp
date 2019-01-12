//
//  OpenGLMath.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/11/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLMath_hpp
#define OpenGLMath_hpp

#include <stdio.h>
#include <glm/glm.hpp>

class OpenGLMath
{
public:
    static glm::mat3 generateCrossProductMatrix(glm::vec3 k);
    static glm::mat3 calculateRotationMatrix(glm::vec3 a, glm::vec3 b);
    
    static glm::mat3 calculateRotationMatrix2(glm::vec3 a, glm::vec3 b);
    
};

#endif /* OpenGLMath_hpp */
