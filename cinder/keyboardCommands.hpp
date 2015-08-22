/*
Copyright (C) 2015 Martin Bock, Gary Klindt, Annelene Wittenfeld

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

void CinderProjectApp::keyDown(KeyEvent event)
{
	switch (event.getChar())
	{
	case '0':
		setFullScreen(!isFullScreen());
		break;
	case '9':
		spock.reset();
		break;

	case 'p': // preset 1
		triggerPreset(1);
		break; 
	case 'o': // preset 2
		triggerPreset(2);
		break;
	case 'l': // preset 3
		triggerPreset(3);
		break;

	case 'ö':
		nextpic.pause = !nextpic.pause;
		break;

	case'1':
		filt_fourier.setOutput(picFourierInput);
		break;
	case 'q':
		filt_fourier.setOutput(picMask);
		break;
	case 'a':
		incrementFourierFLevel();
		break;
	case 'y':
		decrementFourierFLevel();
		break;

	case '2':
		filt_fourier.setOutput(picLength);
		break;
	case 'w':
		filt_fourier.setOutput(picAngle);
		break; 
	case 's':
		incrementFourierFSpread();
		break;
	case 'x':
		decrementFourierFSpread();
		break;
	
	case '3':
		filt_fourier.setOutput(picInverseReal);
		break;
	case 'e':
		filt_fourier.setOutput(picInverseImag);
		break;
	case 'd':
		filt_fourier.fourmask.incrementBandpassR0();
		break;
	case 'c':
		filt_fourier.fourmask.decrementBandpassR0();
		break;
	
	case '4':
		filt_deriv.setOutput(picDerivInput);
		break;
	case 'r':
		filt_deriv.setOutput(picDxy);
		break;
	case 'f':
		filt_fourier.fourmask.incrementBandpassDR();
		break;
	case 'v':
		filt_fourier.fourmask.decrementBandpassDR();
		break;
	
	case '5':
		filt_deriv.setOutput(picDxx);
		break;
	case 't':
		filt_deriv.setOutput(picDyy);
		break;
	case 'g':
		filt_fourier.fourmask.increment_phi_M();
		break;
	case 'b':
		filt_fourier.fourmask.decrement_phi_M();
		break;
	
	case '6':
		filt_deriv.setOutput(picEVLarge);
		break;
	case 'z':
		filt_deriv.setOutput(picEVSmall);
		break;
	case 'h':
		filt_fourier.fourmask.increment_phi_D();
		break;
	case 'n':
		filt_fourier.fourmask.decrement_phi_D();
		break;
	
	case '7':
		filt_deriv.setOutput(picDx);
		break;
	case 'u':
		filt_deriv.setOutput(picDy);
		break;
	case 'j':
		filt_threshold.increaseIntensityLevel();
		break;
	case 'm':
		filt_threshold.decreaseIntensityLevel();
		break;
	
	case '8':
		filt_threshold.setOutput(picBeforeThres);
		break;
	case 'i':
		filt_threshold.setOutput(picAfterThres);
		break;
	case 'k':
		filt_threshold.increaseIntensitySpread();
		break;
	case ',':
		filt_threshold.decreaseIntensitySpread();
		break;
	}
}