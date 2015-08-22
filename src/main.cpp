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

#include <chrono>

#include <opencv2/highgui/highgui.hpp>

#include "VideoLoop.hpp"

#define FPS 12.0f

class FrameDelay
{
    private:

        const float fps;
        const float targetDt;
        std::vector<float> previousDt;
        std::chrono::high_resolution_clock::time_point lastTime;
        float Dt;
	
	float dt=1.0/fps;
	float om=5.0/fps/dt;
	float oms=om*om;
	float dDt=10.0;

        unsigned int iter = 0;

        float meanDt()
        {
            float totalDt = 0.0f;
            for (std::vector<float>::iterator i = previousDt.begin(); 
                                              i != previousDt.end(); i++){
                totalDt += (*i);
            }

            return totalDt / previousDt.size();
        }

    public:

        FrameDelay(const unsigned int N) :
            fps(FPS),
            targetDt(1000.0 / fps),
            previousDt(N)
        {
            for (std::vector<float>::iterator i = previousDt.begin(); 
                                              i != previousDt.end(); i++){
                (*i) = targetDt;
            }
            lastTime = std::chrono::high_resolution_clock::now();
            Dt = targetDt;
        }

        float operator()()
        {
            iter += 1;

            auto currentTime = std::chrono::high_resolution_clock::now();
            previousDt.at(iter%previousDt.size()) = std::chrono::duration<float, std::milli>(currentTime - lastTime).count();
            lastTime = currentTime;	    

	    dDt = (oms*targetDt-2.0*om*dDt-oms*meanDt())*dt;
	    Dt  = Dt+dDt*dt;
	    
	    if(Dt<1.0)
	      Dt=1.0f;

            return Dt;
        }
};

int main() {

    VideoLoop loop(GRIDDIM,NRPIC);
    FrameDelay delay(FPS);

    while (true) {
        cv::imshow(std::string("zweiHochX"), loop());
        float del = delay();
        cv::waitKey(del);
    }
}

