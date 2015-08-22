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

#ifndef FOURIERMASK_HPP
#define FOURIERMASK_HPP

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "helper.hpp"
#include "ExponentialControl.hpp"

/// Constructs the mask employed in Fourier bandpass.

class FourierMask
{
    private:

        bool updateFlag;
        unsigned int rows, cols;
        unsigned int maxWaveNum = (unsigned int) ceil(sqrt(rows*rows+cols*cols));
        cv::Mat waveNum;
        cv::Mat wavePhi;

        cv::Mat mask = cv::Mat::ones(rows, cols, CV_32FC1);

        int lim = 0;

        signed char controlR0 = 64;
        signed char controlDR = 127;
        float R0 = maxWaveNum / 2.0f; // aka NLevel
        float DR = maxWaveNum / 2.0f; // aka Nspread

        float nmin = 0.0f;
        float nmax = 10.0f;

        signed char controlPhiM = 64;
        signed char controlPhiD = 127;
        float phi_M = M_PI / 2.0f;
        float phi_D = M_PI / 2.0f;

        ExponentialControl exponentialControl;

        void setNminNmax(float Nlevel, float Nspread){
            set_nmin(Nlevel - Nspread);
            set_nmax(Nlevel + Nspread);
        }

        void set_nmin(float n){
            updateFlag = true;
            nmin = limitRange(n, maxWaveNum);
        }

        void set_nmax(float n){
            updateFlag = true;
            nmax = limitRange(n, maxWaveNum);
        }

        void computeWaveData()
        {
            cv::Mat X = cv::Mat::zeros(rows, cols, CV_32FC1);
            cv::Mat Y = cv::Mat::zeros(rows, cols, CV_32FC1);

            float rh = (float)(rows / 2); //rows-half
            float ch = (float)(cols / 2); //cols-half

            for (unsigned int i = 0; i < rows; i++){
                for (unsigned int j = 0; j < cols; j++){
                    if (i < rows / 2)
                        X.at<float>(i, j) = -(float)(i - rh);
                    else
                        X.at<float>(i, j) = -(float)(i + 1 - rh);

                    if (j < cols / 2)
                        Y.at<float>(i, j) = (float)(j - ch);
                    else
                        Y.at<float>(i, j) = (float)(j + 1 - ch);
                }
            }
            cartToPolar(Y, X, waveNum, wavePhi);
        }

        cv::Mat computeMask(){
            cv::Mat circle = cv::Mat::zeros(rows, cols, CV_32FC1);
            cv::Mat outside = cv::Mat::zeros(rows, cols, CV_32FC1);

            cv::threshold(waveNum, circle, nmax, 1, cv::THRESH_BINARY_INV);
            cv::threshold(waveNum, outside, nmin, 1, cv::THRESH_BINARY);
            cv::Mat ring = circle.mul(outside);

            //upper half-image of angular mask
            cv::Mat smaller = cv::Mat::zeros(rows, cols, CV_32FC1);
            cv::Mat larger = cv::Mat::zeros(rows, cols, CV_32FC1);
            cv::Mat angle;
            if (phi_M - phi_D < 0){ // winding number tricky
                cv::threshold(wavePhi, smaller, phi_M + phi_D, 1, cv::THRESH_BINARY_INV);
                cv::threshold(wavePhi, larger, M_PI + (phi_M - phi_D), 1, cv::THRESH_BINARY);
                angle = larger + smaller;
            }
            else if (phi_M + phi_D > M_PI){
                cv::threshold(wavePhi, smaller, phi_M + phi_D - M_PI, 1, cv::THRESH_BINARY_INV);
                cv::threshold(wavePhi, larger, phi_M - phi_D, 1, cv::THRESH_BINARY);
                angle = larger + smaller;
            }
            else{ //winding number 0
                cv::threshold(wavePhi, smaller, phi_M + phi_D, 1, cv::THRESH_BINARY_INV);
                cv::threshold(wavePhi, larger, phi_M - phi_D, 1, cv::THRESH_BINARY);
                angle = larger.mul(smaller);
            }

            //lower half-image of angular mask, point-mirrored from upper
            for (unsigned int i = 0; i < rows / 2; i++){
                for (unsigned int j = 0; j < cols; j++){
                    angle.at<float>(rows - 1 - i, cols - 1 - j) = angle.at<float>(i, j);
                }
            }

            double blurWidth = 7.0;
            if (DR / 4 < blurWidth)
                blurWidth = DR / 4;

            if (DR < 1e-1)
                blurWidth = 1e-2;

            cv::GaussianBlur(ring, mask, cv::Size(0, 0), blurWidth, blurWidth, cv::BORDER_DEFAULT);
            return(mask.mul(angle));
        }

    public:

        FourierMask(unsigned int r, unsigned int c) :
            updateFlag(true),
            rows(r),
            cols(c) {
                controlR0 = initControlR0;
                controlDR = initControlDR;
                setBandpassR0(controlR0);
                setBandpassDR(controlDR);

                controlPhiM = initControlPhiM;
                controlPhiD = initControlPhiD;
                set_phi_M(controlPhiM);
                set_phi_D(controlPhiD);

                computeWaveData();
                cv::Mat mask = get();
            }

        const signed char initControlR0 = 96; // center of interval in exponential control
        const signed char initControlDR = 127;

        const signed char initControlPhiM = 64;
        const signed char initControlPhiD = 127;

        void reset(){
            setBandpassR0(initControlR0);
            setBandpassDR(initControlDR);
            set_phi_M(initControlPhiM);
            set_phi_D(initControlPhiD);
        }

        void setBandpassR0(signed char r0){
            controlR0 = r0;
            R0 = limitRange(exponentialControl(r0) / 127.0f * maxWaveNum, maxWaveNum);
            setNminNmax(R0, DR); // sets updateFlag
        }
        void incrementBandpassR0(){
            if (controlR0 < 127)
                setBandpassR0(controlR0 + 1);
        }
        void decrementBandpassR0(){
            if (controlR0 > 0)
                setBandpassR0(controlR0 - 1);
        }

        void setBandpassDR(signed char dr){
            controlDR = dr;
            DR = limitRange(exponentialControl(dr) / 127.0f * maxWaveNum / 2.0f, maxWaveNum / 2.0f);
            setNminNmax(R0, DR); // sets updateFlag
        }
        void incrementBandpassDR(){
            if (controlDR < 127)
                setBandpassDR(controlDR + 1);
        }
        void decrementBandpassDR(){
            if (controlDR > 0)
                setBandpassDR(controlDR - 1);
        }

        void set_phi_M(signed char phi){
            controlPhiM = phi;
            updateFlag = true;
            phi_M = limitRange(phi / 127.0f * M_PI, M_PI);
        }
        void increment_phi_M(){
            if (controlPhiM < 127)
                set_phi_M(controlPhiM + 1);
        }
        void decrement_phi_M(){
            if (controlPhiM>0)
                set_phi_M(controlPhiM - 1);
        }

        void set_phi_D(signed char phi){
            controlPhiD = phi;
            updateFlag = true;
            phi_D = limitRange(exponentialControl(phi) / 127.0f * M_PI / 2.0, M_PI / 2.0f);
        }
        void increment_phi_D(){
            if (controlPhiD < 127)
                set_phi_D(controlPhiD + 1);
        }
        void decrement_phi_D(){
            if (controlPhiD > 0)
                set_phi_D(controlPhiD - 1);
        }

        cv::Mat get(){
            if (updateFlag == true){
                mask = computeMask();
                updateFlag = false;
            }
            return(mask);
        }
};

#endif

