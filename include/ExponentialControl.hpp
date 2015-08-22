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

#ifndef EXPONENTIALCONTROL_HPP
#define EXPONENTIALCONTROL_HPP

#include <vector>

/// Nonlinear mapping to improve control response.
/**
 * The mapping [0,127] -> [0,127] is nonlinear, whereby the spacing 
 * between adjacent control values increases exponentially.
 */

class ExponentialControl
{
    private:

        std::vector<float> expcon = std::initializer_list<float>({ 0.000f, 0.228f, 0.461f, 0.698f,
        0.940f, 1.187f, 1.438f, 1.695f, // 5
        1.956f, 2.223f, 2.495f, 2.773f, // 9
        3.055f, 3.344f, 3.638f, 3.938f, // 13
        4.244f, 4.556f, 4.874f, 5.199f, // 17
        5.530f, 5.867f, 6.211f, 6.562f, // 21
        6.920f, 7.284f, 7.657f, 8.036f, // 25
        8.423f, 8.817f, 9.220f, 9.630f, // 29
        10.048f, 10.475f, 10.910f, 11.354f, // 33
        11.806f, 12.268f, 12.738f, 13.218f, // 37
        13.707f, 14.206f, 14.715f, 15.234f, // 41
        15.763f, 16.303f, 16.853f, 17.414f, // 45
        17.986f, 18.570f, 19.165f, 19.772f, // 49
        20.391f, 21.022f, 21.665f, 22.321f, // 53
        22.990f, 23.673f, 24.369f, 25.078f, // 57
        25.802f, 26.540f, 27.292f, 28.060f, // 61
        28.842f, 29.640f, 30.454f, 31.284f, // 65
        32.130f, 32.993f, 33.873f, 34.771f, // 69
        35.686f, 36.619f, 37.571f, 38.541f, // 73
        39.531f, 40.540f, 41.569f, 42.619f, // 77
        43.689f, 44.780f, 45.893f, 47.028f, // 81
        48.186f, 49.366f, 50.569f, 51.797f, // 85
        53.048f, 54.325f, 55.626f, 56.953f, // 89
        58.307f, 59.687f, 61.094f, 62.530f, // 93
        63.993f, 65.486f, 67.008f, 68.560f, // 97
        70.143f, 71.757f, 73.403f, 75.081f, // 101
        76.793f, 78.539f, 80.318f, 82.134f, // 105
        83.985f, 85.872f, 87.797f, 89.760f, // 109
        91.762f, 93.803f, 95.884f, 98.007f, // 113
        100.172f, 102.379f, 104.630f, 106.926f, // 117
        109.266f, 111.654f, 114.088f, 116.570f, // 121
        119.102f, 121.683f, 124.316f, 127.000f }); // 125

    public:

        float operator()(signed char i){
            return expcon.at(i);
        }
};

#endif

