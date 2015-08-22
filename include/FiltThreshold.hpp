/*
Copyright (c) 2015, Martin Bock, Gary Klindt, Annelene Wittenfeld
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are 
met:

1. Redistributions of source code must retain the above copyright 
notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright 
notice, this list of conditions and the following disclaimer in the 
documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its 
contributors may be used to endorse or promote products derived from 
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS 
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED 
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef FILTTHRESHOLD_HPP
#define FILTTHRESHOLD_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Filter.hpp"
#include "helper.hpp"
#include "ExponentialControl.hpp"

enum ThresholdOutput { picBeforeThres, picAfterThres };

/// Bandpass in pixel intensity.

class FiltThreshold : public Filter
{
    private:

        signed char controlIntensityLevel = 64;
        signed char controlIntensitySpread = 32;
        float intensityLevel = 0.5f;
        float intensitySpread = 0.25f;
        float minIntensity = 0.25f;
        float maxIntensity = 0.75f;

        ThresholdOutput output = picBeforeThres;

        ExponentialControl exponentialControl;

        void setMinMaxIntensity(){
            minIntensity = limitRange(intensityLevel - intensitySpread, 1.0);
            maxIntensity = limitRange(intensityLevel + intensitySpread, 1.0);
        }

        void apply(const cv::Mat&A, cv::Mat &B)
        {
            cv::Mat C,D;

            switch (output){
                case picAfterThres:
                    cv::threshold(A, C, minIntensity, 1, cv::THRESH_BINARY);
                    cv::threshold(A, D, maxIntensity, 1, cv::THRESH_BINARY_INV);
                    B = C.mul(D);
                    break;

                case picBeforeThres:
                default:
                    B = A;
            }
        }

    public:

        const signed char initControlIntensityLevel = 64;
        const signed char initControlIntensitySpread = 32;

        FiltThreshold(unsigned int r, unsigned int c) :
            Filter(r, c) {
                controlIntensityLevel = initControlIntensityLevel;
                controlIntensitySpread = initControlIntensitySpread;
                setIntensityLevel(controlIntensityLevel);
                setIntensitySpread(controlIntensitySpread);
            }

        void reset(){
            setIntensityLevel(initControlIntensityLevel);
            setIntensitySpread(initControlIntensitySpread);
            setOutput(picBeforeThres);
        }

        void setOutput(ThresholdOutput n){
            output = n;
        }

        signed char getControlIntensityLevel(){
            return(controlIntensityLevel);
        }
        void setIntensityLevel(signed char lvl){
            controlIntensityLevel = lvl;
            intensityLevel = limitRange( lvl / 127.0f, 1.0f);
            setMinMaxIntensity();
        }
        void increaseIntensityLevel(){
            if (controlIntensityLevel<127)
                setIntensityLevel(controlIntensityLevel + 1);
        }
        void decreaseIntensityLevel(){
            if (controlIntensityLevel>0)
                setIntensityLevel(controlIntensityLevel - 1);
        }

        signed char getControlIntensitySpread(){
            return(controlIntensitySpread);
        }
        void setIntensitySpread(signed char sprd){
            controlIntensitySpread = sprd;
            intensitySpread = limitRange( exponentialControl(sprd) / 127.0f / 2.0f, 0.5);
            setMinMaxIntensity();
        }
        void increaseIntensitySpread(){
            if (controlIntensitySpread<127)
                setIntensitySpread(controlIntensitySpread + 1);
        }
        void decreaseIntensitySpread(){
            if (controlIntensitySpread>0)
                setIntensitySpread(controlIntensitySpread - 1);
        }
};

#endif

