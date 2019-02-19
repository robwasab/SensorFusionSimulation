//
//  OpenGLGraphGrid.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 1/27/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "OpenGLGraphGrid.hpp"

OpenGLGraphGrid::OpenGLGraphGrid(size_t rows, size_t cols, float window_width, float window_height)
{
    assert(rows >= 1);
    assert(cols >= 1);
    assert(window_width >= 0);
    assert(window_height >= 0);
    
    mRows = rows;
    mCols = cols;
    mWindowWidth = window_width;
    mWindowHeight = window_height;
    
    mGraphsLen = rows * cols;
    mGraphs = new OpenGLGraphRef[mGraphsLen];
    mGraphTransforms = new glm::mat4[mGraphsLen];
    mTitleLocations = new TitleLocation[mGraphsLen];
    
    mProjection = glm::ortho(0.0f, mWindowWidth, 0.0f, mWindowHeight);
    
    windowResizedTo(mWindowWidth, mWindowHeight);
    
    for(size_t k = 0; k < mGraphsLen; k++)
    {
        mGraphs[k] = NULL;
    }
    
    mTextDrawer = OpenGLText::getOpenGLTextWithWindowDimensions(window_width, window_height);
}

OpenGLGraphGrid::~OpenGLGraphGrid()
{
    for(size_t k = 0; k < mGraphsLen; k++)
    {
        delete mGraphs[k];
    }
    delete [] mGraphs;
    delete [] mGraphTransforms;
    delete [] mTitleLocations;
}

void OpenGLGraphGrid::windowResizedTo(float size_x, float size_y)
{
    assert(size_x >= 0);
    assert(size_y >= 0);
    
    mWindowWidth = size_x;
    mWindowHeight = size_y;
    
    mProjection = glm::ortho(0.0f, mWindowWidth, 0.0f, mWindowHeight);

    float cell_width = mWindowWidth / mCols;
    float cell_height = mWindowHeight / mRows;
    
    for(size_t k = 0; k < mGraphsLen; k++)
    {
        size_t row = k / mCols;
        size_t col = k % mCols;
        
        assert(row < mRows);
        assert(col < mCols);
        
        float origin_x = cell_width * (col + 0.5f);
        float origin_y = cell_height * (row + 0.5f);
        
        glm::mat4 scale(1.0f);
        glm::mat4 trans(1.0f);
        
        scale = glm::scale(scale, glm::vec3(cell_width/2.0, cell_height/2.0 * 0.9f, 1.0f));
        trans = glm::translate(trans, glm::vec3(origin_x, origin_y, 0.0f));
        
        glm::mat4 cell_transform = trans * scale;
        
        //glm::vec4 in = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        //glm::vec4 out = cell_transform * in;
        
        //printf("k: %zu [%zu, %zu]\n", k, row, col);
        //printf("x: %.3f -> %.3f\n", in.x, out.x);
        //printf("y: %.3f -> %.3f\n", in.y, out.y);
        //printf("z: %.3f -> %.3f\n", in.z, out.z);
        
        mGraphTransforms[k] = cell_transform;
        
        mTitleLocations[k].height = cell_height/2.0 * 0.1f;
        mTitleLocations[k].max_width = cell_width;
        mTitleLocations[k].origin_x = cell_width * col;
        mTitleLocations[k].origin_y = cell_height * row;
    }
}

void OpenGLGraphGrid::setGraphWithRowAndCol(OpenGLGraph * graph, int row, int col)
{
    assert(row < mRows);
    assert(col < mCols);
    
    // check to see if graph is already added
    for(size_t k = 0; k < mGraphsLen; k++) {
        assert(mGraphs[k] != graph);
    }
    
    size_t index = row * mCols + col;
    
    // check to see if another graph is in the target slot
    if(mGraphs[index] != NULL)
    {
        // delete...
        delete mGraphs[index];
    }
    
    mGraphs[index] = graph;
}

void OpenGLGraphGrid::drawWithTransform(glm::mat4 transform)
{
    glm::mat4 identity(1.0f);
    
    mTextDrawer->windowResizedTo(mWindowWidth, mWindowHeight);

    for(size_t k = 0; k < mGraphsLen; k++)
    {
        if(mGraphs[k] == NULL) {
            continue;
        }
        
        glm::mat4 transform = mProjection * mGraphTransforms[k];
        
        mGraphs[k]->drawWithTransform(transform);
        
        mTextDrawer->setFontHeight(mTitleLocations[k].height);
        mTextDrawer->setColor(OpenGLPrimitive::COLOR_ORANGE);
        
        float text_width = mTextDrawer->calculateStringWidth(mGraphs[k]->getTitle());
        float text_start_x = mTitleLocations[k].origin_x + mTitleLocations[k].max_width/2.0 - text_width/2.0;
        
        mTextDrawer->setBottomLeftCornerOrigin(text_start_x,
                                               mTitleLocations[k].origin_y);
        
        mTextDrawer->drawStringWithTransform(mGraphs[k]->getTitle(), identity);
    }
}
