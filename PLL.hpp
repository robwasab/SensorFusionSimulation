//
//  PLL.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/4/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef PLL_hpp
#define PLL_hpp

#include <stdio.h>
#include <math.h>

#include "Integrator.hpp"
#include "Biquad_LowPass.hpp"

class PLL
{
private:
    double mFs;
    double mFc;
    double mGain;
    double mVcoInc;
    Integrator mVco;
    
    Biquad_LowPass mInphaseLpf;
    Biquad_LowPass mQuadratLpf;
    
    void error_detector(float input,
                        float * error,
                        float * in_phase,
                        float * qu_phase,
                        float * lock,
                        float * in_phase_vco,
                        float * qu_phase_vco);
    
public:
    static const int GAIN_DB = 0;

    PLL(double fs, double fc, double gain_db=GAIN_DB);

    float work(float x);
};

#endif /* PLL_hpp */
