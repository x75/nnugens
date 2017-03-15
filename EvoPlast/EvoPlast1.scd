s.quit

s.boot;
s.scope;

(
var m_amp = 1;
var m_b_a = #[[2.135,  3.732, -2.0,  0.404]];
{
	EvoPlast1.ar(
		in1: SinOsc.ar(freq: 440, phase: 0, mul: 0.5, add: 0),
		in2: SinOsc.ar(freq: 583, phase: 0, mul: 1.5, add: 0),
		// m_b: #[[2.135,  3.732, -2.0,  0.404]], // m_b_a,
		m_amp: m_amp, //SinOsc.ar(freq: 0.13, mul: m_amp, add: m_amp/2), // 25.0, //1.805, //
		tau: 0.5
	)
}.play;
)

{EvoPlast1.ar(in1: SinOsc.ar(freq: 440, phase: 0, mul: 0.5, add: 0), in2: SinOsc.ar(freq: 583, phase: 0, mul: 1.5, add: 0)) ! 2 * 1.0}.play;


{SinOsc.ar(freq: 440, phase: 0, mul: 1.0, add: 0)}.play

