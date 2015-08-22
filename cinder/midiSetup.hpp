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

	mInput.listPorts();
	for (int j = 0; j < mInput.getNumPorts(); j++){
		console() << "Input Port " << j << " is " << mInput.getPortName(j) << std::endl;
		if (mInput.getPortName(j).compare("BCF2000"))
			mInput.openPort(j);
	}
	mInput.midiSignal.connect(boost::bind(&CinderProjectApp::midiListener, this, boost::arg<1>::arg()));
	for (int i = 0; i < 127; i++)
	{
		notes.push_back(0);
		cc.push_back(0);
	}

	for (int j = 0; j < mOutput.getNumPorts(); j++){
		console() << "Output Port " << j << " is " << mOutput.getPortName(j) << std::endl;
		if (mOutput.getPortName(j).compare("BCF2000"))
			mOutput.openPort(j);
	}