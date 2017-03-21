////////////////////////////////////////////////////////////////////////////////
// Reservoir UGen Class file

BufReservoir : MultiOutUGen {
	// *ar { arg bufW, bufWloc, insize = 1, in = 0,
	// 	wsize = 300, g = 0.99, tau = 0.1, mul=1.0, add=0.0;
	*ar { arg buf, in = 0, insize = 1,
		wsize = 300, g = 0.99, tau = 0.1, mul=1.0, add=0.0;

		^this.multiNew('audio', buf, in, insize, 
			wsize, g, tau).madd( mul, add )
	}
	// init { arg wsize ... theInputs;
	// init { arg buf, in, insize, wsize ... theInputs;
	init { arg ... theInputs;
		inputs = theInputs;
		// ^this.initOutputs(wsize, rate);
		^this.initOutputs(2, rate);
	}

	// *kr { arg buf, ..., mul=1.0, add=0.0;
	// 	^this.multiNew('audio', buf, in1, in2, m_amp, tau).madd( mul, add )
	// }
}
