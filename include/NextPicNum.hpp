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

#ifndef NEXTPICNUM_HPP
#define NEXTPICNUM_HPP

/// Logic for continuous play of movie, forward backward forward etc.

class NextPicNum
{
    private:

        bool forward = true;
        unsigned int impr; // max # of pix, 0-based
        unsigned int nr=0;

    public:

        bool pause = false;

        NextPicNum(unsigned int maxpicnr):
            impr(maxpicnr-1) {}

        int operator()(){
            if (pause)
                return nr;

            if (forward){
                if (nr >= impr) {
                    forward = false;
                    return(impr); //last pic 2x
                }

                nr += 1;
                return(nr);
            }

            // backward
            if (nr == 0){
                forward = true;
                return(0); //first pic 2x
            }

            nr -= 1;
            return(nr);
        }
};

#endif

