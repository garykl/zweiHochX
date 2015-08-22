# Copyright (c) 2015, Martin Bock, Gary Klindt, Annelene Wittenfeld
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are 
# met:
# 
# 1. Redistributions of source code must retain the above copyright 
# notice, this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright 
# notice, this list of conditions and the following disclaimer in the 
# documentation and/or other materials provided with the distribution.
# 
# 3. Neither the name of the copyright holder nor the names of its 
# contributors may be used to endorse or promote products derived from 
# this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS 
# IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED 
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A 
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
# HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED 
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

CPP = g++ -Wall -pedantic --std=c++11

INC = -I./include -I./src
LNK = -lopencv_core -lopencv_highgui -lopencv_imgproc -lboost_system



.PHONY: videoloop
videoloop: src/main.cpp ${OBJ}
	${CPP} ${INC} src/main.cpp -o $@ ${LNK}


.PHONY: debug
debug: src/main.cpp
	${CPP} -g ${INC} src/main.cpp -o $@ ${LNK}



.PHONY: doxy
doxy:
	doxygen



.PHONY: tst
tst: hpptest

HPPOBJ = include/AnalyseImage.o include/ExponentialControl.o include/Filter.o include/FiltDerivatives.o include/FiltFourier.o include/FiltThreshold.o include/FourierMask.o include/NextPicNum.o include/TimeFourier.o include/VideoLoop.o

.PHONY: hpptest
hpptest:  ${HPPOBJ}
	rm -f include/*.o

include/%.o: include/%.hpp
	${CPP} ${INC} -c $< -o $@

