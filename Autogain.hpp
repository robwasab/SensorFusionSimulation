//
//  Autogain.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/4/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef Autogain_hpp
#define Autogain_hpp

#include <stdio.h>
#include <math.h>

class Autogain
{
private:
    double last_x;
    double last_y;
    double ALPHA = 0.99;
    double MAX_GAIN = 1.0E2;
public:
    
    double mGain;
    
    Autogain()
    {
        last_x = 0.0;
        last_y = 0.0;
        mGain = 1.0;
    }
    
    float work(float x)
    {
        double xsq = x * x;
        
        /* short-time signal variance */
        double ysq = last_x * (1 - ALPHA) + last_y * ALPHA;
        
        last_y = ysq;
        last_x = xsq;
        
        double y = sqrt(ysq);
        
        double gain;
        
        if(y >= (1.0 / MAX_GAIN))
        {
            gain = 1.0 / y;
        }
        else
        {
            gain = MAX_GAIN;
        }
        mGain = gain;
        
        return (float) (x * gain/2.0f);
    }
};

#endif /* Autogain_hpp */
