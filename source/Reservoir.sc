////////////////////////////////////////////////////////////////////////////////
// Reservoir UGen Class file

BufReservoir : MultiOutUGen {
	*ar { arg bufW, bufWloc, insize = 1, in = 0,
		wsize = 300, g = 0.99, tau = 0.1, mul=1.0, add=0.0;

		^this.multiNew('audio', bufW, bufWloc, insize, in,
			wsize, g, tau).madd( mul, add )
	}
	init { arg ... theInputs;
		inputs = theInputs;
		^this.initOutputs(wsize, rate);
	}

	// *kr { arg buf, ..., mul=1.0, add=0.0;
	// 	^this.multiNew('audio', buf, in1, in2, m_amp, tau).madd( mul, add )
	// }
}
