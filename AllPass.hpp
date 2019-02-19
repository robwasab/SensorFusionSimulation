//
//  AllPass.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/16/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef AllPass_hpp
#define AllPass_hpp

#include <stdio.h>

class AllPass
{
private:
    double mGain;
    double mA;
    double mB;
    double mLastX[3];
    double mLastY[3];
public:
    AllPass(double gain, double zero, double pole);
    
    float work(float sample);
};

#endif /* AllPass_hpp */
