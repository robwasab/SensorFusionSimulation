//
//  OpenGLGraphGrid.hpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/27/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#ifndef OpenGLGraphGrid_hpp
#define OpenGLGraphGrid_hpp

#include <stdio.h>
#include "OpenGLGraph.hpp"
#include "OpenGLText.hpp"

class OpenGLGraphGrid
{
private:
    struct TitleLocation
    {
        float origin_x;
        float origin_y;
        float height;
        float max_width;
    };
    
    size_t mRows;
    size_t mCols;
    float mWindowWidth;
    float mWindowHeight;
    
    typedef OpenGLGraph* OpenGLGraphRef;
    OpenGLGraphRef * mGraphs;
    TitleLocation * mTitleLocations;
    OpenGLText * mTextDrawer;
    
    size_t mGraphsLen;
    glm::mat4 * mGraphTransforms;
    glm::mat4 mProjection;
    
public:
    OpenGLGraphGrid(size_t rows, size_t cols, float window_width, float window_height);
    ~OpenGLGraphGrid();
    
    void windowResizedTo(float size_x, float size_y);
    
    void setGraphWithRowAndCol(OpenGLGraph * graph, int row, int col);
    
    void drawWithTransform(glm::mat4 transform);
};

#endif /* OpenGLGraphGrid_hpp */
