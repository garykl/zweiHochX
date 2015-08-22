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

#ifndef TIMEFOURIER_HPP
#define TIMEFOURIER_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "NextPicNum.hpp"
#include "helper.hpp"

/// Fourier bandpass in time/frequency, implemented as lookup table.

class TimeFourier {

    private:

        unsigned int griddim;
        const signed char initControlFLevel = 96;
        const signed char initControlFSpread = 127;
        signed char controlFLevel = 64;
        signed char controlFSpread = 64;
        int fLevel = griddim / 2;
        int fSpread = griddim / 4;
        int fidx1 = 0;
        int fidx2 = 3 * griddim / 4;

        char completepath[1000];

        cv::Mat G;
        cv::Mat A;

        NextPicNum nextpic;

    public:

        TimeFourier(unsigned int gridsize, unsigned int numberOfPics):
            griddim(gridsize), nextpic(NextPicNum(numberOfPics)) {}

        void setFourierFLevel(signed char fl){
            controlFLevel = fl;
            fLevel = limitRange(fl / 127.0f * (griddim - 1.0f), griddim - 1.0f);
            setFidx();
        }

        void incrementFourierFLevel(){
            if (controlFLevel < 127)
                setFourierFLevel(controlFLevel + 1);
        }

        void decrementFourierFLevel(){
            if (controlFLevel > 0)
                setFourierFLevel(controlFLevel - 1);
        }

        void setFourierFSpread(signed char fs){
            controlFSpread = fs;
            fSpread = limitRange( fs / 127.0f * griddim / 2.0f, griddim/2.0f );
            setFidx();
        }

        void incrementFourierFSpread(){
            if (controlFSpread < 127)
                setFourierFSpread(controlFSpread + 1);
        }

        void decrementFourierFSpread(){
            if (controlFSpread > 0)
                setFourierFSpread(controlFSpread - 1);
        }

        void setFidx(){
            fidx1 = (int) limitRange(fLevel - fSpread, griddim - 1.0f);
            fidx2 = (int) limitRange(fLevel + fSpread, griddim - 1.0f);
        }

        void resetFourierTime(){
            setFourierFLevel(initControlFLevel);
            setFourierFSpread(initControlFSpread);
        }

        cv::Mat operator()() {
            unsigned int k = nextpic();
            sprintf(completepath, "data/min%02imax%02i/%03i.png", fidx1 + 1, fidx2 + 1, k+1);
            G = cv::imread(completepath, CV_LOAD_IMAGE_GRAYSCALE);
            G.convertTo(A, CV_32FC1);
            normalize(A, A, 0, 1.0, CV_MINMAX);
            return A;
        }
};

#endif

