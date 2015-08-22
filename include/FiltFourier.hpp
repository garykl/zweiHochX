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

#ifndef FILTFOURIER_HPP
#define FILTFOURIER_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Filter.hpp"
#include "FourierMask.hpp"
#include "helper.hpp"

enum FourierOutput { picFourierInput, picMask, picLength, picAngle, picInverseReal, picInverseImag };

/// Spatial Fourier bandpass, both in wavelength and angle.

class FiltFourier : public Filter
{
    private:

        FourierOutput output = picInverseReal;

        void shift(cv::Mat &B){
            int cx = B.cols / 2;
            int cy = B.rows / 2;

            cv::Mat q0(B, cv::Rect(0, 0, cx, cy));   // Top-Left - Create a ROI per quadrant
            cv::Mat q1(B, cv::Rect(cx, 0, cx, cy));  // Top-Right
            cv::Mat q2(B, cv::Rect(0, cy, cx, cy));  // Bottom-Left
            cv::Mat q3(B, cv::Rect(cx, cy, cx, cy)); // Bottom-Right

            cv::Mat tmp;    // swap quadrants (Top-Left with Bottom-Right)
            q0.copyTo(tmp);
            q3.copyTo(q0);
            tmp.copyTo(q3);

            q1.copyTo(tmp); // swap quadrant (Top-Right with Bottom-Left)
            q2.copyTo(q1);
            tmp.copyTo(q2);
        }

        void apply(const cv::Mat& A, cv::Mat &Output)
        {
            if (output == picFourierInput){
                Output = A;
                return;
            }

            cv::Mat Mask = fourmask.get();
            if (output == picMask){
                Output = cv::Mat::ones(rows, cols, CV_32FC1);
                Output = Output.mul(Mask);
                return;
            }

            cv::Mat Sandwich[] = { A, cv::Mat::zeros(rows, cols, CV_32FC1) };
            cv::Mat SpectrumParts[] = { cv::Mat::zeros(rows, cols, CV_32FC1), cv::Mat::zeros(rows, cols, CV_32FC1) };

            cv::Mat angI = cv::Mat::zeros(rows, cols, CV_32FC1);
            cv::Mat lenI = cv::Mat::zeros(rows, cols, CV_32FC1);
            cv::Mat S0 = cv::Mat::zeros(rows, cols, CV_32FC1);
            cv::Mat S1 = cv::Mat::zeros(rows, cols, CV_32FC1);

            cv::Mat Complexum, ComplexSpectrum;
            cv::merge(Sandwich, 2, Complexum);
            cv::dft(Complexum, ComplexSpectrum);

            split(ComplexSpectrum, SpectrumParts); // SpectrumParts[0] = Re(DFT(I), Spectrumparts[1] = Im(DFT(I))
            S0 = SpectrumParts[0];
            S1 = SpectrumParts[1];
            shift(S0);
            shift(S1);
            cv::cartToPolar(S0, S1, lenI, angI);

            if (output == picLength){
                lenI += 1.0f;
                cv::log(lenI, Output);
                cv::normalize(Output, Output, 0, 1, CV_MINMAX);
                return;
            }

            if (output == picAngle){
                cv::normalize(angI, Output, 0, 1, CV_MINMAX);
                return;
            }

            cv::Mat reaL = cv::Mat::zeros(rows, cols, CV_32FC1);
            cv::Mat iMag = cv::Mat::zeros(rows, cols, CV_32FC1);
            reaL = S0.mul(Mask);
            iMag = S1.mul(Mask);
            shift(reaL);
            shift(iMag);

            cv::Mat S2[] = { reaL, iMag };
            cv::Mat maskedSpectrum;
            cv::merge(S2, 2, maskedSpectrum);

            cv::dft(maskedSpectrum, Complexum, cv::DFT_INVERSE | cv::DFT_COMPLEX_OUTPUT); //inverse Fouriertrafo
            cv::split(Complexum, Sandwich);

            if (output == picInverseImag)
                cv::normalize(Sandwich[1], Output, 0, 1, CV_MINMAX);

            else // if output==picInverseReal
                cv::normalize(Sandwich[0], Output, 0, 1, CV_MINMAX);
        }

    public:

        FourierMask fourmask;

        FiltFourier(unsigned int r, unsigned int c) :
            Filter(r, c),
            fourmask(FourierMask(r, c)){}

        void reset()
        {
            fourmask.reset();
            setOutput(picFourierInput);
        }

        void setOutput(FourierOutput n)
        {
            output = n;
        }
};

#endif

