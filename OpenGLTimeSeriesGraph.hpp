//
//  OpenGLTimeSeriesGraph.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/16/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLTimeSeriesGraph_hpp
#define OpenGLTimeSeriesGraph_hpp

#include <stdio.h>
#include "OpenGLGraph.hpp"
#include "TimeSeries.hpp"

class OpenGLTimeSeriesGraph: public OpenGLGraph
{
private:
    TimeSeries * mDataSeries;
    char * mTitle;
    bool mFFTMode;
public:
    
    OpenGLTimeSeriesGraph(int pow2, const char title[]);
    
    ~OpenGLTimeSeriesGraph();
    
    void updateWithNewSample(float sample);
    
    virtual void updatePlotData(Point * data, size_t len) override;
    virtual const char * getTitle(void) override;
    
    void setFFTMode(bool enable_logging=false) {
        mFFTMode = true;
        mDataSeries->setFFTMode(enable_logging);
    }
    
    void setTimeMode(void) {
        mFFTMode = false;
        mDataSeries->setTimeMode();
    }
};


#endif /* OpenGLTimeSeriesGraph_hpp */
