//
//  TimeSeries.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/16/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef TimeSeries_hpp
#define TimeSeries_hpp

#include <stdio.h>
#include <fftw3.h>

class TimeSeries
{
private:
    size_t mDataLen;
    float * mTimeData;
    float * mFFTOutputData;
    float * mData;
    
    // fft var
    fftwf_plan fwrd;
    
    bool mLoggingEnabled;
    
    void calculate_fft(void);
    
public:
    TimeSeries(int pow2);
    
    ~TimeSeries();
    
    // Incomming data point
    void updateWithNewSample(float sample);
    
    // Return data length
    size_t getDataSize(void);
    
    // Get the data at index
    float getDataAtIndex(size_t index);
    
    // Data is read back as FFT magnitude
    void setFFTMode(bool logging_enabled=false);
    
    // Data is read back as Time series
    void setTimeMode(void);
    
};

#endif /* TimeSeries.hpp */
