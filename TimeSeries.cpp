//
//  OpenGLGraphData.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/16/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//
#include <stdlib.h>

#include "OpenGLGraphData.hpp"

OpenGLGraphData::OpenGLGraphData(size_t len, size_t fft_window_len)
{
    mTimeDataLen = len;
    mFFTDataLen = fft_window_len;
    
    float * mTimeData = new float[len];
    float * mFFTOutputData = new float[fft_window_len];
    float * mFFTStagedTimeData = new float[fft_window_len];
    float * mData = mTimeData;
    
    
}

~OpenGLGraphData();

// Incomming data point
void updateWithNewSample(float sample);

// Get the data to be plotted
void getDataAndLength(const float * data, size_t * len);

// Data is read back as FFT magnitude
void setFFTMode(void);

// Data is read back as Time series
void setTimeMode(void);
