//
//  OpenGLPrismCylinder.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/7/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLPrismCylinder.hpp"
#include "OpenGLCircle.hpp"


class CylinderFaceGenerator: public OpenGLPrism::PrismFaceGenerator
{
public:
    OpenGLPrimitive * getNextFace(float rgb[3], const char texture_filename[])
    {
        return new OpenGLCircle(6, rgb, texture_filename);
    }
};

OpenGLPrismCylinder::OpenGLPrismCylinder(glm::vec3 extrude_direction,
                                         float height,
                                         float rgb[3],
                                         const char texture_filename[]):
OpenGLPrism(new CylinderFaceGenerator(),
            extrude_direction,
            height,
            rgb,
            texture_filename)
{
}
