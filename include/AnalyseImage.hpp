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

#include <iostream>
#include <sstream>
#include <opencv2/core/core.hpp>

#define LISBOX 86 //size of square box for averaging lbx = 2*LISBOX+1
#define LISDT 0.025f
#define LISOM1 M_PI
#define LISOM2 std::exp(1)
#define LISANGDIFF M_PI/2.0

/// Extract observable values from image and write them to stdout.

class AnalyseImage {

    private:

        const unsigned int rows, cols;
        const float shiftx, shifty;
        const unsigned int lbx;

        float x, y;
        float t = 0;

		std::vector<double> imgDesc;

        void analyse(const cv::Mat& A)
        {
            const float dt = LISDT;
            const float omega1 = LISOM1;
            const float omega2 = LISOM2;
            const float phi0 = LISANGDIFF;
            int ampl = (int)((A.rows - 2 * lbx - 1) / 2);

            x += dt * ampl*omega1 * cos(omega1*t + phi0);
            y += dt * ampl*omega2 * cos(omega2*t);
            t += dt;

            unsigned int rowmax = rows - lbx - 2;
            unsigned int colmax = cols - lbx - 2;
            unsigned int xint = x + shiftx;
            unsigned int yint = y + shifty;

            if (xint < lbx){
                xint = lbx;
            } else if (xint > rowmax) {
                xint = rowmax;
            }
            if (yint < lbx){
                yint = lbx; 
            } else if (yint > colmax) {
                yint = colmax;
            }

            cv::Mat tm = A;
            tm = tm(cv::Rect(xint - lbx, yint - lbx, 2*lbx + 1, 2*lbx + 1));

            cv::Scalar meaN, var, meA, vaA;
            cv::meanStdDev(tm, meaN, var);
            cv::meanStdDev(A, meA, vaA);

            imgDesc = {meaN[0], var[0], meA[0], vaA[0]};
        }


    public:

        AnalyseImage(unsigned int r, unsigned int c) :
            rows(r),
            cols(c),
            shiftx((float)(r / 2)), 
            shifty((float)(c / 2)),
            lbx(LISBOX),
			imgDesc(4)
        {
            x = shiftx - (float)(lbx + 1);
            y = -18.0f;
        }


        void operator()(const cv::Mat& A)
        {
            analyse(A);

            std::stringstream analysis;
            for (std::vector<double>::iterator i = imgDesc.begin(); i != imgDesc.end(); ++i){
                analysis << (*i) << " ";
            }
            analysis << std::endl;

            //std::cout << analysis.str();
        }

};

