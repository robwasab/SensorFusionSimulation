//
//  OpenGLTimeSeriesGraph.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/16/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLTimeSeriesGraph.hpp"

#include <stdlib.h>
#include <string.h>

OpenGLTimeSeriesGraph::OpenGLTimeSeriesGraph(int pow2, const char title[]):
OpenGLGraph(1 << pow2)
{
    assert(title != NULL);
    mTitle = new char[strlen(title) + 1];
    memcpy(mTitle, title, strlen(title) + 1);
    
    mDataSeries = new TimeSeries(pow2);
    mFFTMode = false;
}


OpenGLTimeSeriesGraph::~OpenGLTimeSeriesGraph()
{
    delete [] mTitle;
    delete mDataSeries;
}

void OpenGLTimeSeriesGraph::updateWithNewSample(float sample)
{
    mDataSeries->updateWithNewSample(sample);
}

void OpenGLTimeSeriesGraph::updatePlotData(Point * data, size_t len)
{
    assert(len == mDataSeries->getDataSize());
    
    for(size_t k = 0; k < len; k++)
    {
        float x;
        float y;
        
        x = (2.0f * k)/len - 1.0f;
        
        if(mFFTMode)
        {
            float mag_db = 10.0f * log10(mDataSeries->getDataAtIndex(k/2));
            
            float max_db =  3.0f;
            float min_db = -60.0f;
            
            // hard limit
            mag_db = mag_db < min_db ? min_db : mag_db;
            mag_db = mag_db > max_db ? max_db : mag_db;
            
            float graph_max =  1.0f;
            float graph_min = -1.0f;
            
            float m = (graph_max - graph_min) / (max_db - min_db);
            float b = graph_min - m * min_db;
            
            y = m * mag_db + b;
        } else {
            y = mDataSeries->getDataAtIndex(k);
        }
        
        data[k].x = x;
        data[k].y = y;
    }
}

const char * OpenGLTimeSeriesGraph::getTitle()
{
    return mTitle;
}
