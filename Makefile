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

