//
//  OpenGLGraphData.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/16/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLGraphData_hpp
#define OpenGLGraphData_hpp

#include <stdio.h>

class OpenGLGraphData
{
private:
    size_t mDataLen;
    float * mTimeData;
    float * mFFTOutputData;
    float * mFFTStagedTimeData;
    float * mData;
    
public:
    OpenGLGraphData(size_t len);
    
    ~OpenGLGraphData();
    
    // Incomming data point
    void updateWithNewSample(float sample);
    
    // Get the data to be plotted
    void getDataAndLength(const float * data, size_t * len);
    
    // Data is read back as FFT magnitude
    void setFFTMode(void);
    
    // Data is read back as Time series
    void setTimeMode(void);
};

#endif /* OpenGLGraphData_hpp */
