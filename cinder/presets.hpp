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

void CinderProjectApp::triggerPreset(int presetNum){
	console() << "triggerPreset # " << presetNum << std::endl;

	switch (presetNum){
	case 2:
		// Fourier time, sliders 1,2
		setFourierFLevel(72); // contains all frequencies
		setFourierFSpread(70);
#ifdef MIDI_CODE_INCLUDED
		processMidiControl(1, 72);
		processMidiControl(2, 70);
		processMidiControl(81, 72);
		processMidiControl(82, 70);
#endif

		// Fourier space, sliders 3,4,5,6
		filt_fourier.fourmask.setBandpassR0(30);
		filt_fourier.fourmask.setBandpassDR(14);
		filt_fourier.fourmask.set_phi_M(64);
		filt_fourier.fourmask.set_phi_D(127);
#ifdef MIDI_CODE_INCLUDED
		processMidiControl(3, 30);
		processMidiControl(4, 14);
		processMidiControl(5, 64);
		processMidiControl(6, 127);
		processMidiControl(83, 30);
		processMidiControl(84, 14);
		processMidiControl(85, 64);
		processMidiControl(86, 127);
#endif

		// Threshold, sliders 7,8
		filt_threshold.setIntensityLevel(64);
		filt_threshold.setIntensitySpread(32);
#ifdef MIDI_CODE_INCLUDED
		processMidiControl(7, 64);
		processMidiControl(8, 32);
		processMidiControl(87, 64);
		processMidiControl(88, 32);
#endif

		// picture selection
#ifdef MIDI_CODE_INCLUDED
		processMidiControl(67, 127); // Fourier = picInverseReal of full spectrum = original
		processMidiControl(70, 127); // Sobel Derivative = picEVLarge
		processMidiControl(72, 127); // Threshold output = picBeforeThres
#else
		filt_fourier.setOutput(picInverseReal);
		filt_deriv.setOutput(picEVLarge);
		filt_threshold.setOutput(picBeforeThres);
#endif
		break;

	case 3:
		// Fourier time, sliders 1,2
		setFourierFLevel(40); // contains all frequencies
		setFourierFSpread(68);
#ifdef MIDI_CODE_INCLUDED
		processMidiControl(1, 40);
		processMidiControl(2, 68);
		processMidiControl(81, 40);
		processMidiControl(82, 68);
#endif
		// Fourier space, sliders 3,4,5,6
		filt_fourier.fourmask.setBandpassR0(28);
		filt_fourier.fourmask.setBandpassDR(2);
		filt_fourier.fourmask.set_phi_M(26);
		filt_fourier.fourmask.set_phi_D(103);
#ifdef MIDI_CODE_INCLUDED
		processMidiControl(3, 28);
		processMidiControl(4, 2);
		processMidiControl(5, 26);
		processMidiControl(6, 103);
		processMidiControl(83, 28);
		processMidiControl(84, 2);
		processMidiControl(85, 26);
		processMidiControl(86, 103);
#endif

		// Threshold, sliders 7,8
		filt_threshold.setIntensityLevel(37);
		filt_threshold.setIntensitySpread(14);
#ifdef MIDI_CODE_INCLUDED
		processMidiControl(7, 37);
		processMidiControl(8, 14);
		processMidiControl(87, 37);
		processMidiControl(88, 14);
#endif

		// picture selection
#ifdef MIDI_CODE_INCLUDED
		processMidiControl(75, 127); // Fourier = picInverseImag
		processMidiControl(70, 127); // Sobel Derivative = Ev1
		processMidiControl(80, 127); // Threshold output = picAfterThres
#else
		filt_fourier.setOutput(picInverseImag);
		filt_deriv.setOutput(picEVLarge);
		filt_threshold.setOutput(picAfterThres);
#endif
		break;

	case 1:
	default:
		// Fourier time, sliders 1,2
		setFourierFLevel(initControlFLevel); // contains all frequencies
		setFourierFSpread(initControlFSpread);
#ifdef MIDI_CODE_INCLUDED
		processMidiControl(1, initControlFLevel);
		processMidiControl(2, initControlFSpread);
		processMidiControl(81, initControlFLevel);
		processMidiControl(82, initControlFSpread);
#endif

		// Fourier space, sliders 3,4,5,6
		filt_fourier.fourmask.setBandpassR0(filt_fourier.fourmask.initControlR0);
		filt_fourier.fourmask.setBandpassDR(filt_fourier.fourmask.initControlDR);
		filt_fourier.fourmask.set_phi_M(filt_fourier.fourmask.initControlPhiM);
		filt_fourier.fourmask.set_phi_D(filt_fourier.fourmask.initControlPhiD);
#ifdef MIDI_CODE_INCLUDED
		processMidiControl(3, filt_fourier.fourmask.initControlR0);
		processMidiControl(4, filt_fourier.fourmask.initControlDR);
		processMidiControl(5, filt_fourier.fourmask.initControlPhiM);
		processMidiControl(6, filt_fourier.fourmask.initControlPhiD);
		processMidiControl(83, filt_fourier.fourmask.initControlR0);
		processMidiControl(84, filt_fourier.fourmask.initControlDR);
		processMidiControl(85, filt_fourier.fourmask.initControlPhiM);
		processMidiControl(86, filt_fourier.fourmask.initControlPhiD);
#endif

		// Threshold, sliders 7,8
		filt_threshold.setIntensityLevel(filt_threshold.initControlIntensityLevel);
		filt_threshold.setIntensitySpread(filt_threshold.initControlIntensitySpread);
#ifdef MIDI_CODE_INCLUDED
		processMidiControl(7, filt_threshold.initControlIntensityLevel);
		processMidiControl(8, filt_threshold.initControlIntensitySpread);
		processMidiControl(87, filt_threshold.initControlIntensityLevel);
		processMidiControl(88, filt_threshold.initControlIntensitySpread);
#endif

		// picture selection
#ifdef MIDI_CODE_INCLUDED
		processMidiControl(67, 127); // Fourier = picInverseReal of full spectrum = original
		processMidiControl(68, 127); // Sobel Derivative = picDerivInput
		processMidiControl(72, 127); // Threshold output = picBeforeThres
#else
		filt_fourier.setOutput(picInverseReal);
		filt_deriv.setOutput(picDerivInput);
		filt_threshold.setOutput(picBeforeThres);
#endif
	}
}