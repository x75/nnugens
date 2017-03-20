// Basic Genet
EvoPlast1 : MultiOutUGen {
	// *ar {|in1, in2, m_b(#[[2.135,  3.732, -2.0,  0.404]]), m_amp(1.0), tau(0.9), mul(1.0), add(0.0)|
	*ar {
		arg in1, in2, m_amp = 1.0, tau = 0.9, mul = 1.0, add = 0.0;
		// ^this.multiNew('audio', in1, in2);
		// ["m_b", m_b].postln;
		// ^this.multiNew('audio', in1, in2, m_b, m_amp, tau).madd(mul, add);
		^this.multiNew('audio', in1, in2, m_amp, tau).madd(mul, add);
	}

	init { arg ... theInputs;
		inputs = theInputs;
		^this.initOutputs(2, rate);
	}

	checkInputs {
		[0, 1].do {|i|
			(inputs[i].rate != 'audio').if {
				^"input % is not audio rate".format(i).throw;
			};
		};
		^this.checkValidInputs;
	}
	// FIXME: kr
}

// Genet with matrix params from buffer
BufEvoPlast1 : MultiOutUGen {
	*ar { arg buf, in1, in2, m_amp = 1.0, tau = 0.9, mul=1.0, add=0.0;
		^this.multiNew('audio', buf, in1, in2, m_amp, tau).madd( mul, add )
	}
	init { arg ... theInputs;
		inputs = theInputs;
		^this.initOutputs(2, rate);
	}

	// *kr { arg buf, ..., mul=1.0, add=0.0;
	// 	^this.multiNew('audio', buf, in1, in2, m_amp, tau).madd( mul, add )
	// }
}

// GenetPlast with matrix params from buffer
BufEvoPlast2 : MultiOutUGen {
	*ar { arg buf, in1, in2, m_amp = 1.0, tau = 0.9, mul=1.0, add=0.0;
		^this.multiNew('audio', buf, in1, in2, m_amp, tau).madd( mul, add )
	}
	init { arg ... theInputs;
		inputs = theInputs;
		^this.initOutputs(2, rate);
	}

	// *kr { arg buf, ..., mul=1.0, add=0.0;
	// 	^this.multiNew('audio', buf, in1, in2, m_amp, tau).madd( mul, add )
	// }
}
