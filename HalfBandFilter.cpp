//
//  HalfBandFilter.cpp
//  OpenGLTutorial
//
//  Created by Robby Tong on 2/16/19.
//  Copyright © 2019 Robby Tong. All rights reserved.
//

#include "HalfBandFilter.hpp"

struct AllPassFilterCoef
{
    double gain;
    double zero;
    double pole;
};

static AllPassFilterCoef gBankACoefs[] =
{
    {0.553294427114, 1.807355995279, 0.553294427114},
    {0.917921907998, 1.089417292786, 0.917921907998},
    {0.992207182754, 1.007854022206, 0.992207182754},
};

static AllPassFilterCoef gBankBCoefs[] =
{
    {0.198759060944, 5.031217169427, 0.198759060944},
    {0.798334921039, 1.252607112187, 0.798334921039},
    {0.969249791891, 1.031725782525, 0.969249791891},
};


#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

HalfBandFilter::HalfBandFilter(void)
{
    mFilterBankALen = ARR_SIZE(gBankACoefs);
    mFilterBankBLen = ARR_SIZE(gBankBCoefs);
    
    mFilterBankA = new AllPassRef[mFilterBankALen];
    mFilterBankB = new AllPassRef[mFilterBankBLen];
    
    for(size_t k = 0; k < mFilterBankALen; k++)
    {
        mFilterBankA[k] = new AllPass(gBankACoefs[k].gain,
                                      gBankACoefs[k].zero,
                                      gBankACoefs[k].pole);
    }
    
    for(size_t k = 0; k < mFilterBankBLen; k++)
    {
        mFilterBankB[k] = new AllPass(gBankBCoefs[k].gain,
                                      gBankBCoefs[k].zero,
                                      gBankBCoefs[k].pole);
    }
    mLastInput = 0;
}


HalfBandFilter::~HalfBandFilter()
{
    for(size_t k = 0; k < mFilterBankALen; k++)
    {
        delete mFilterBankA[k];
    }
    
    for(size_t k = 0; k < mFilterBankBLen; k++)
    {
        delete mFilterBankB[k];
    }
    
    delete [] mFilterBankA;
    delete [] mFilterBankB;
}


float HalfBandFilter::work(float sample)
{
    float bank_a_out = mLastInput;
    float bank_b_out = sample;
    
    mLastInput = sample;
    
    for(size_t k = 0; k < mFilterBankALen; k++)
    {
        bank_a_out = mFilterBankA[k]->work(bank_a_out);
    }
    
    for(size_t k = 0; k < mFilterBankBLen; k++)
    {
        bank_b_out = mFilterBankB[k]->work(bank_b_out);
    }
    
    return bank_a_out + bank_b_out;
}

void HalfBandFilter::impulseResponse(float data[], size_t len)
{
    data[0] = work(1.0f);
    
    for(size_t k = 1; k < len; k++)
    {
        data[k] = work(0.0);
    }
}

