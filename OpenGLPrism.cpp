//
//  OpenGLPrism.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/5/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLPrism.hpp"

OpenGLPrism::OpenGLPrism(PrismFaceGenerator generator,
                         glm::vec3 extrude_direction,
                         float height,
                         float rgb[3],
                         const char texture_filename[]):
OpenGLComposite()
{
    assert(generator != NULL);
    
    mBottomFace = generator(rgb, texture_filename);
    mTopFace = generator(rgb, texture_filename);
    
    glm::vec3 translateUp = extrude_direction * height/2.0f;
    glm::vec3 translateDown = extrude_direction * -height/2.0f;
    
    mWall = new OpenGLPrismWall(mBottomFace,
                                extrude_direction,
                                height,
                                rgb,
                                texture_filename);
    
    mTopFace->setPosition   (translateUp.x,   translateUp.y,   translateUp.z);
    mBottomFace->setPosition(translateDown.x, translateDown.y, translateDown.z);
    mWall->setPosition      (translateDown.x, translateDown.y, translateDown.z);
    
    addDrawable(mBottomFace);
    addDrawable(mTopFace);
    addDrawable(mWall);
}


OpenGLPrism::~OpenGLPrism()
{
    delete mBottomFace;
    delete mTopFace;
    delete mWall;
}
