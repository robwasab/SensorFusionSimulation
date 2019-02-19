//
//  OpenGLGraphData.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/16/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "TimeSeries.hpp"

TimeSeries::TimeSeries(int pow2)
{
    mDataLen = (1 << pow2);
    mTimeData = new float[mDataLen];
    mFFTOutputData = new float[mDataLen];
    mData = mTimeData;
    
    memset(mTimeData, 0, sizeof(float) * mDataLen);
    memset(mFFTOutputData, 0, sizeof(float) * mDataLen);
    
    fwrd = fftwf_plan_r2r_1d((int)mDataLen, mTimeData, mFFTOutputData, FFTW_R2HC, FFTW_MEASURE);
    
    mLoggingEnabled = false;
}

TimeSeries::~TimeSeries()
{
    delete [] mTimeData;
    delete [] mFFTOutputData;
}

// Incomming data point
void TimeSeries::updateWithNewSample(float sample)
{
    // left shift the time data and update the last element with the sample
    for(size_t k = 0; k < mDataLen - 1; k++)
    {
        mTimeData[k] = mTimeData[k+1];
    }
    mTimeData[mDataLen - 1] = sample;
 
    if(mData == mFFTOutputData) {
        calculate_fft();
    }
}

void TimeSeries::calculate_fft(void)
{
    // if FFT mode, update the FFT
    if(mData == mFFTOutputData)
    {
        // calc fft
        fftwf_execute(fwrd);
        
        // normalize
        for(size_t k = 0; k < mDataLen; k++)
        {
            mFFTOutputData[k] /= (float) mDataLen/2.0;
        }
        
        // determine maximum magnitude frequency
        float max_magn = -1.0f;
        float max_freq = -1.0f;
        size_t max_index = 0xffff;
        float real;
        float imag;
        for(size_t k = 0; k < mDataLen/2; k++)
        {
            if(k == 0) {
                real = mFFTOutputData[0];
                imag = 0;
            } else {
                real = mFFTOutputData[k];
                imag = mFFTOutputData[mDataLen-k];
            }
            
            float mag = real * real + imag * imag;
            
            if(mag > max_magn) {
                //printf("new max at index %zu\n", k);
                max_magn = mag;
                
                // calculate the frequency
                max_freq = (1.0f * k) / mDataLen;
                max_index = k;
            }
        }
        
        if(mLoggingEnabled) {
            float max_magn_db = 10.0 * log10(max_magn);
            printf("[%zu] %3.3fdB at %1.6f\n", max_index, max_magn_db, max_freq);
        }
    }
}

size_t TimeSeries::getDataSize(void)
{
    return mDataLen;
}

// Get the data to be plotted
float TimeSeries::getDataAtIndex(size_t index)
{
    assert(index < mDataLen);
    
    if(mData == mTimeData)
    {
        return mData[index];
    }
    else if(mData == mFFTOutputData)
    {
        if(index == 0)
        {
            return mFFTOutputData[0];
        }
        if(index < mDataLen/2)
        {
            float real = mFFTOutputData[index];
            float imag = mFFTOutputData[mDataLen - index];
            
            float mag = sqrt(real * real + imag * imag);
            return mag;
        }
        else
        {
            return 0.0f;
        }
    }
    else
    {
        // shouldn't get here
        assert(false);
        return 0.0f;
    }
}

// Data is read back as FFT magnitude
void TimeSeries::setFFTMode(bool logging_enabled)
{
    mLoggingEnabled = logging_enabled;
    mData = mFFTOutputData;
    calculate_fft();
}

// Data is read back as Time series
void TimeSeries::setTimeMode(void)
{
    mData = mTimeData;
}
