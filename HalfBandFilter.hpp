//
//  HalfBandFilter.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/16/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef HalfBandFilter_hpp
#define HalfBandFilter_hpp

#include <stdio.h>
#include "AllPass.hpp"

class HalfBandFilter
{
private:
    typedef AllPass * AllPassRef;
    
    AllPassRef * mFilterBankA;
    AllPassRef * mFilterBankB;
    
    size_t mFilterBankALen;
    size_t mFilterBankBLen;
    
    double mLastInput;
    
public:
    HalfBandFilter(void);
    ~HalfBandFilter();
    
    float work(float sample);
    
    void impulseResponse(float data[], size_t len);
};

#endif /* HalfBandFilter_hpp */
