//
//  Integrator.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/16/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef Integrator_h
#define Integrator_h

class Integrator
{
public:
    Integrator(double fs) {
        lasty = 0.0;
        twofs = 2.0 * fs;
    }
    
    double value() {
        return (double) (lasty/twofs);
    }
    
    double work(double input) {
        long double out = input + lasty;
        lasty = input + out;
        return out/twofs;
    }
    
    void reset() {
        lasty = 0;
    }
    
private:
    long double lasty;
    double twofs;
};

#endif /* Integrator_h */
