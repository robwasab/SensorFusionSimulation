//
//  PLL.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/4/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "PLL.hpp"

PLL::PLL(double fs, double fc, double gain_db):
    mFs(fs),
    mFc(fc),
    mVco(fs),
    mInphaseLpf(1.0/sqrt(2.0) /* quality */, 0.75 /* fc */, fs /* fs */),
    mQuadratLpf(1.0/sqrt(2.0) /* quality */, 0.75 /* fc */, fs /* fs */)
{
    mGain = pow(10.0, gain_db / 10.0);
    mVcoInc = 2.0 * M_PI * fc;
}

void PLL::error_detector(float input,
                         float * error,
                         float * in_phase,
                         float * qu_phase,
                         float * lock,
                         float * in_phase_vco,
                         float * qu_phase_vco)
{
    double vco_phase;
    double cos_vco;
    double sin_vco;
    
    // Phase Generator
    // vco_phase is a constantly increasing phase value
    vco_phase = mVco.value();
    
    // Oscillator
    cos_vco = cos(vco_phase);
    sin_vco =-sin(vco_phase);
    
    // Error Generator
    *in_phase = 2.0 * mInphaseLpf.work(input * cos_vco);
    *qu_phase = 2.0 * mQuadratLpf.work(input * sin_vco);
    
    *error = *qu_phase;
    
    *in_phase_vco = cos_vco;
    *qu_phase_vco = sin_vco;
    
    // Lock Detector
    //*lock = this->lock_detector->work(*in_phase, *qu_phase);
}

float PLL::work(float x)
{
    float error;
    float in_phase;
    float qu_phase;
    float lock;
    float in_phase_vco;
    float qu_phase_vco;
    
    x = x > 1.0f ? 1.0f : x;
    x = x <-1.0f ?-1.0f : x;
    
    error_detector(x, &error, &in_phase, &qu_phase, &lock, &in_phase_vco, &qu_phase_vco);
    
    double s3 = mGain * error;
    
    mVco.work(mVcoInc + s3);
    
    return in_phase;
}
