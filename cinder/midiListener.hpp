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

void CinderProjectApp::midiListener(midi::Message msg){
	switch (msg.status)
	{
	/*case MIDI_NOTE_ON:
	case MIDI_NOTE_OFF:
	case MIDI_PROGRAM_CHANGE:
	case MIDI_PITCH_BEND:
	case MIDI_AFTERTOUCH:
	case MIDI_POLY_AFTERTOUCH:
		msg.port;
		msg.channel;
		msg.timeStamp;
		msg.byteOne;
		msg.byteTwo;
		msg.pitch;
		msg.velocity;*/

	case MIDI_CONTROL_CHANGE:
		/******** Preset/pause: button group bottom right ********/
		if (msg.control == 92){
			if (msg.value == 0)
				nextpic.pause = false;

			if (msg.value == 127)
				nextpic.pause = true;

			return;
		}

		if (msg.control >= 89 && msg.control <= 91){
			mOutput.sendControlChange(1, 89, 0);
			mOutput.sendControlChange(1, 90, 0);
			mOutput.sendControlChange(1, 91, 0);

			switch (msg.control){
			case 89: // top left
				triggerPreset(2);
				break;

			case 91: // bottom left
				triggerPreset(3);
				break;

			case 90: // top right
			default:
				triggerPreset(1);
			}

			mOutput.sendControlChange(1, msg.control, 127);
			return;
		}

		mOutput.sendControlChange(1, 89, 0);
		mOutput.sendControlChange(1, 90, 0);
		mOutput.sendControlChange(1, 91, 0);
		processMidiControl(msg.control,msg.value);
		break;

	default:
		break;
	}
}

void CinderProjectApp::processMidiControl(signed char msgcontrol, signed char msgvalue){
	/******** Fourier buttons in columns 1-3 ********/
	if ((msgcontrol >= 65 && msgcontrol <= 67)
		|| (msgcontrol >= 73 && msgcontrol <= 75)){
		mOutput.sendControlChange(1, 65, 0);
		mOutput.sendControlChange(1, 66, 0);
		mOutput.sendControlChange(1, 67, 0);
		mOutput.sendControlChange(1, 73, 0);
		mOutput.sendControlChange(1, 74, 0);
		mOutput.sendControlChange(1, 75, 0);

		switch (msgcontrol){
		case 65:
			filt_fourier.setOutput(picFourierInput);
			break;
		case 66:
			filt_fourier.setOutput(picLength);
			break;
		case 67:
			filt_fourier.setOutput(picInverseReal);
			break;
		case 73:
			filt_fourier.setOutput(picMask);
			break;
		case 74:
			filt_fourier.setOutput(picAngle);
			break;
		case 75:
			filt_fourier.setOutput(picInverseImag);
			break;
		default:
			filt_fourier.setOutput(picFourierInput);
		}
		mOutput.sendControlChange(1, msgcontrol, 127);
	}

	/******** Sobel buttons in columns 4-7 ********/
	if ((msgcontrol >= 68 && msgcontrol <= 71)
		|| (msgcontrol >= 76 && msgcontrol <= 79)){
		mOutput.sendControlChange(1, 68, 0);
		mOutput.sendControlChange(1, 69, 0);
		mOutput.sendControlChange(1, 70, 0);
		mOutput.sendControlChange(1, 71, 0);
		mOutput.sendControlChange(1, 76, 0);
		mOutput.sendControlChange(1, 77, 0);
		mOutput.sendControlChange(1, 78, 0);
		mOutput.sendControlChange(1, 79, 0);

		switch (msgcontrol){
		case 68:
			filt_deriv.setOutput(picDerivInput);
			break;
		case 69:
			filt_deriv.setOutput(picDxx);
			break;
		case 70:
			filt_deriv.setOutput(picEVLarge);
			break;
		case 71:
			filt_deriv.setOutput(picDx);
			break;
		case 76:
			filt_deriv.setOutput(picDxy);
			break;
		case 77:
			filt_deriv.setOutput(picDyy);
			break;
		case 78:
			filt_deriv.setOutput(picEVSmall);
			break;
		case 79:
			filt_deriv.setOutput(picDy);
			break;
		default:
			filt_deriv.setOutput(picDerivInput);
		}
		mOutput.sendControlChange(1, msgcontrol, 127);
	}

	/******** Intensity buttons in column 8 ********/
	if (msgcontrol == 72){
		filt_threshold.setOutput(picBeforeThres);
		mOutput.sendControlChange(1, 80, 0);
		mOutput.sendControlChange(1, msgcontrol, 127);
		mOutput.sendControlChange(1, 87, 0); // move sliders to zero
		mOutput.sendControlChange(1, 88, 0); // move sliders to zero
	}
	if (msgcontrol == 80){
		filt_threshold.setOutput(picAfterThres);
		mOutput.sendControlChange(1, 72, 0);
		mOutput.sendControlChange(1, msgcontrol, 127);
		mOutput.sendControlChange(1, 87, filt_threshold.getControlIntensityLevel()); // restore previous value
		mOutput.sendControlChange(1, 88, filt_threshold.getControlIntensitySpread()); // restore previous value
	}

	/******* Fourier f,df: slider & rotary controller 1,2 ********/
	if (msgcontrol == 81){ // slider
		setFourierFLevel(msgvalue);
		mOutput.sendControlChange(1, 1, msgvalue);
		mOutput.sendControlChange(1, 9, msgvalue);
		mOutput.sendControlChange(1, 17, msgvalue);
		mOutput.sendControlChange(1, 25, msgvalue);
	}
	if (msgcontrol == 1 || msgcontrol == 9
		|| msgcontrol == 17 || msgcontrol == 25){ // knob
		setFourierFLevel(msgvalue);
		mOutput.sendControlChange(1, 81, msgvalue);
	}

	if (msgcontrol == 82){ // slider
		setFourierFSpread(msgvalue);
		mOutput.sendControlChange(1, 2, msgvalue);
		mOutput.sendControlChange(1, 10, msgvalue);
		mOutput.sendControlChange(1, 18, msgvalue);
		mOutput.sendControlChange(1, 26, msgvalue);
	}
	if (msgcontrol == 2 || msgcontrol == 10
		|| msgcontrol == 18 || msgcontrol == 26){ // knob
		setFourierFSpread(msgvalue);
		mOutput.sendControlChange(1, 82, msgvalue);
	}

	/******* Fourier N,dN: slider & rotary controller 3,4 ********/
	if (msgcontrol == 83){ // slider
		filt_fourier.fourmask.setBandpassR0(msgvalue);
		mOutput.sendControlChange(1, 3, msgvalue);
		mOutput.sendControlChange(1, 11, msgvalue);
		mOutput.sendControlChange(1, 19, msgvalue);
		mOutput.sendControlChange(1, 27, msgvalue);
	}
	if (msgcontrol == 3 || msgcontrol == 11
		|| msgcontrol == 19 || msgcontrol == 27){ // knob
		filt_fourier.fourmask.setBandpassR0(msgvalue);
		mOutput.sendControlChange(1, 83, msgvalue);
	}

	if (msgcontrol == 84){ // slider
		filt_fourier.fourmask.setBandpassDR(msgvalue);
		mOutput.sendControlChange(1, 4, msgvalue);
		mOutput.sendControlChange(1, 12, msgvalue);
		mOutput.sendControlChange(1, 20, msgvalue);
		mOutput.sendControlChange(1, 28, msgvalue);
	}
	if (msgcontrol == 4 || msgcontrol == 12
		|| msgcontrol == 20 || msgcontrol == 28){ // knob
		filt_fourier.fourmask.setBandpassDR(msgvalue);
		mOutput.sendControlChange(1, 84, msgvalue);
	}

	/******* Fourier theta,dtheta: slider & knob 5,6 ********/
	if (msgcontrol == 85){ // slider
		filt_fourier.fourmask.set_phi_M(msgvalue);
		mOutput.sendControlChange(1, 5, msgvalue);
		mOutput.sendControlChange(1, 13, msgvalue);
		mOutput.sendControlChange(1, 21, msgvalue);
		mOutput.sendControlChange(1, 29, msgvalue);
	}
	if (msgcontrol == 5 || msgcontrol == 13
		|| msgcontrol == 21 || msgcontrol == 29){ // knob
		filt_fourier.fourmask.set_phi_M(msgvalue);
		mOutput.sendControlChange(1, 85, msgvalue);
	}

	if (msgcontrol == 86){ // slider
		filt_fourier.fourmask.set_phi_D(msgvalue);
		mOutput.sendControlChange(1, 6, msgvalue);
		mOutput.sendControlChange(1, 14, msgvalue);
		mOutput.sendControlChange(1, 22, msgvalue);
		mOutput.sendControlChange(1, 30, msgvalue);
	}
	if (msgcontrol == 6 || msgcontrol == 14
		|| msgcontrol == 22 || msgcontrol == 30){ // knob
		filt_fourier.fourmask.set_phi_D(msgvalue);
		mOutput.sendControlChange(1, 86, msgvalue);
	}

	/******* Threshold: slider & rotary controller 7,8 ********/
	if (msgcontrol == 87){ // slider
		filt_threshold.setIntensityLevel(msgvalue);
		mOutput.sendControlChange(1, 7, msgvalue);
		mOutput.sendControlChange(1, 15, msgvalue);
		mOutput.sendControlChange(1, 23, msgvalue);
		mOutput.sendControlChange(1, 31, msgvalue);
	}
	if (msgcontrol == 7 || msgcontrol == 15
		|| msgcontrol == 23 || msgcontrol == 31){ // knob
		filt_threshold.setIntensityLevel(msgvalue);
		mOutput.sendControlChange(1, 87, msgvalue);
	}

	if (msgcontrol == 88){ // slider
		filt_threshold.setIntensitySpread(msgvalue);
		mOutput.sendControlChange(1, 8, msgvalue);
		mOutput.sendControlChange(1, 16, msgvalue);
		mOutput.sendControlChange(1, 24, msgvalue);
		mOutput.sendControlChange(1, 32, msgvalue);
	}
	if (msgcontrol == 8 || msgcontrol == 16
		|| msgcontrol == 24 || msgcontrol == 32){ // knob
		filt_threshold.setIntensitySpread(msgvalue);
		mOutput.sendControlChange(1, 88, msgvalue);
	}
}

