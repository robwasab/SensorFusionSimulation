//
//  OpenGLPrismCylinder.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/7/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLPrismCylinder.hpp"
#include "OpenGLCircle.hpp"

static OpenGLPrimitive * cylinder_face_generator(float rgb[3], const char texture_filename[])
{
    return new OpenGLCircle(10, rgb, texture_filename);
}


OpenGLPrismCylinder::OpenGLPrismCylinder(glm::vec3 extrude_direction,
                                         float height,
                                         float rgb[3],
                                         const char texture_filename[]):
OpenGLPrism(cylinder_face_generator,
            extrude_direction,
            height,
            rgb,
            texture_filename)
{
}
