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

#ifndef VIDEOLOOP_HPP
#define VIDEOLOOP_HPP

#include <opencv2/core/core.hpp>

#include "TimeFourier.hpp"
#include "FiltFourier.hpp"
#include "FiltDerivatives.hpp"
#include "FiltThreshold.hpp"
//#include "ParaSendSocket.hpp"
#include "AnalyseImage.hpp"

#define GRIDDIM 16
#define NRPIC 358

#define DER_KERNEL 3 //odd number

/// Display movie frame after filtering.

class VideoLoop {

    private:

        TimeFourier timeFourier;
        cv::Mat A;
        unsigned int rows;
        unsigned int cols;
        cv::Mat B;

        FiltFourier filtFourier;
        FiltDerivatives filtDeriv;
        FiltThreshold filtThreshold;
        AnalyseImage analyseImage;
        //ParaSendSocket spock;

    public:

        VideoLoop(unsigned int gd, unsigned int np):
            timeFourier(TimeFourier(gd, np)),
            A(timeFourier()),
            rows(A.rows),
            cols(A.cols),
            B(cv::Mat::zeros(rows, cols, CV_32FC1)),
            filtFourier(FiltFourier(rows, cols)),
            filtDeriv(FiltDerivatives(rows, cols, DER_KERNEL)),
            filtThreshold(FiltThreshold(rows, cols)),
            analyseImage(AnalyseImage(rows, cols)) {}
            //spock(ParaSendSocket(rows, cols)) {}

        cv::Mat& operator()()
        {

            A = timeFourier();
            filtFourier(A, B);
            filtDeriv(B, B);
            filtThreshold(B, B);

            analyseImage(B);
            //spock(B);

            return B;
        }

};

#endif

