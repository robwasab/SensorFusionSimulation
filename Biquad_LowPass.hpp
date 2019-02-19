//
//  Biquad_LowPass.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/16/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef Biquad_LowPass_hpp
#define Biquad_LowPass_hpp

#include <stdio.h>

class Biquad_LowPass
{
public:
    Biquad_LowPass(double Q, double fc, double fs);
    float work(float sig);
    void reset();
    
private:
    void init(double A,
              double omega,
              double sn,
              double cs,
              double alpha,
              double beta);
    float a0;
    float a1;
    float a2;
    float b0;
    float b1;
    float b2;
    float x1;
    float x2;
    float y1;
    float y2;
};

#endif /* Biquad_LowPass_hpp */
