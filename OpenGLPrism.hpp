//
//  OpenGLPrism.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/5/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLPrism_hpp
#define OpenGLPrism_hpp

#include <stdio.h>
#include "OpenGLComposite.hpp"
#include "OpenGLPrismWall.hpp"

class OpenGLPrism: public OpenGLComposite
{
private:
    OpenGLPrismWall * mWall;
    OpenGLPrimitive * mBottomFace;
    OpenGLPrimitive * mTopFace;
    float mRgb[3];
    
public:
    class PrismFaceGenerator
    {
    public:
        virtual ~PrismFaceGenerator()
        {
        }
        virtual OpenGLPrimitive * getNextFace(float rgb[3], const char texture_filename[]) = 0;
    };
    
    OpenGLPrism(PrismFaceGenerator * generator,
                glm::vec3 extrude_direction=glm::vec3(0.0, 1.0, 0.0),
                float height=1.0f,
                float rgb[3]=NULL,
                const char texture_filename[]=NULL);
    
    ~OpenGLPrism();    
};

#endif /* OpenGLPrism_hpp */
