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

#ifndef FILTDERIVATIVES_HPP
#define FILTDERIVATIVES_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Filter.hpp"

enum DerivOutput { picDx, picDy, picDr, picDphi, picDxx, picDxy, picDyy, picEVSmall, picEVLarge, picDerivInput };

/// Image filtering based on Sobel derivatives.

class FiltDerivatives : public Filter
{
    private:

        unsigned int kernel;
        DerivOutput output = picDerivInput;

        cv::Mat grad_x, grad_y;
        cv::Mat d_xx, d_yy, d_xy, largeC, smallC;
        cv::Mat fd_xx, fd_yy, fd_xy;
        cv::Mat diskr,root;

        int scale = 1;
        int delta = 0;
        int ddepth = -1; //source and destination of sobel have identical type

        void apply(const cv::Mat& A, cv::Mat &B)
        {
            Sobel(A, grad_x, ddepth, 1, 0, kernel, scale, delta, cv::BORDER_DEFAULT);
            Sobel(A, grad_y, ddepth, 0, 1, kernel, scale, delta, cv::BORDER_DEFAULT);

            Sobel(grad_x, d_xx, ddepth, 1, 0, kernel, scale, delta, cv::BORDER_DEFAULT);
            Sobel(grad_x, d_xy, ddepth, 0, 1, kernel, scale, delta, cv::BORDER_DEFAULT);
            Sobel(grad_y, d_yy, ddepth, 0, 1, kernel, scale, delta, cv::BORDER_DEFAULT);

            diskr = (((d_xx - d_yy) / 2.0).mul(((d_xx - d_yy) / 2.0)) + d_xy.mul(d_xy));
            sqrt(diskr, root);
            largeC = (d_xx + d_yy) / 2.0 + root;
            smallC = (d_xx + d_yy) / 2.0 - root;

            switch (output)
            {
                case picDx:
                    normalize(grad_x, B, 0, 1, CV_MINMAX);
                    break;
                case picDy:
                    normalize(grad_y, B, 0, 1, CV_MINMAX);
                    break;
                case picDxx:
                    normalize(d_xx, B, 0, 1, CV_MINMAX);
                    break;
                case picDxy:
                    normalize(d_xy, B, 0, 1, CV_MINMAX);
                    break;
                case picDyy:
                    normalize(d_yy, B, 0, 1, CV_MINMAX);
                    break;
                case picEVSmall:
                    normalize(smallC, B, 0, 1, CV_MINMAX);
                    break;
                case picEVLarge:
                    normalize(largeC, B, 0, 1, CV_MINMAX);
                    break;
                case picDerivInput:
                default:
                    B = A;
                    break;
            }
        }

    public:

        FiltDerivatives(unsigned int r, unsigned int c, unsigned int k) :
            Filter(r, c),
            kernel(k) {}

        void reset(){
            setOutput(picDerivInput);
        }

        void setOutput(DerivOutput n)
        {
            output = n;
        }
};

#endif

