//
//  Average.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/3/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef Average_h
#define Average_h

class Average
{
private:
    float mAlpha;
    float mLastOut;
public:
    Average(float fs, float window_time)
    {
        float k = window_time * fs;
        mAlpha = (k - 1.0f) / k;
        mLastOut = 0.0;
    }
    
    float work(float input)
    {
        float out;
        
        out = mAlpha * mLastOut + (1.0f - mAlpha) * input;
        
        mLastOut = out;
        
        return out;
    }
    
};

#endif /* Average_h */
